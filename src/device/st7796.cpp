#include "st7796.hpp"

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_check.h"

static const char* TAG_ST7796 = "St7796";

// ---------------------------------------------------------------------------
// ST7796 panel implementation (ported from LilyGoLib bsp_lcd/esp_lcd_st7796.c)
// ---------------------------------------------------------------------------

typedef struct
{
    esp_lcd_panel_t base;
    esp_lcd_panel_io_handle_t io;
    int reset_gpio_num;
    bool reset_level;
    int x_gap;
    int y_gap;
    uint8_t fb_bits_per_pixel;
    uint8_t madctl_val;
    uint8_t colmod_val;
} st7796_panel_t;

typedef struct
{
    uint8_t cmd;
    uint8_t data[16];
    uint8_t data_bytes;  // 0xFF = end of table
} st7796_init_cmd_t;

static const st7796_init_cmd_t k_vendor_init_cmds[] = {
    {0xf0, {0xc3}, 1},
    {0xf0, {0x96}, 1},
    {0xb4, {0x01}, 1},
    {0xb7, {0xc6}, 1},
    {0xe8, {0x40, 0x8a, 0x00, 0x00, 0x29, 0x19, 0xa5, 0x33}, 8},
    {0xc1, {0x06}, 1},
    {0xc2, {0xa7}, 1},
    {0xc5, {0x18}, 1},
    {0xe0,
     {0xf0, 0x09, 0x0b, 0x06, 0x04, 0x15, 0x2f, 0x54, 0x42, 0x3c, 0x17, 0x14, 0x18, 0x1b},
     14},
    {0xe1,
     {0xf0, 0x09, 0x0b, 0x06, 0x04, 0x03, 0x2d, 0x43, 0x42, 0x3b, 0x16, 0x14, 0x17, 0x1b},
     14},
    {0xf0, {0x3c}, 1},
    {0xf0, {0x69}, 1},
    {0x21, {0x00}, 1},  // Display inversion on (matches LilyGo init)
    {0x00, {0x00}, 0xff},
};

static esp_err_t panel_st7796_del(esp_lcd_panel_t* panel)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    if (ctx->reset_gpio_num >= 0)
        gpio_reset_pin(static_cast<gpio_num_t>(ctx->reset_gpio_num));
    free(ctx);
    return ESP_OK;
}

static esp_err_t panel_st7796_reset(esp_lcd_panel_t* panel)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;

    if (ctx->reset_gpio_num >= 0)
    {
        gpio_set_level(static_cast<gpio_num_t>(ctx->reset_gpio_num), ctx->reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(static_cast<gpio_num_t>(ctx->reset_gpio_num), !ctx->reset_level);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    else
    {
        esp_lcd_panel_io_tx_param(io, LCD_CMD_SWRESET, nullptr, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    return ESP_OK;
}

static esp_err_t panel_st7796_init(esp_lcd_panel_t* panel)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;

    esp_lcd_panel_io_tx_param(io, LCD_CMD_SLPOUT, nullptr, 0);
    vTaskDelay(pdMS_TO_TICKS(120));

    esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]){ctx->madctl_val}, 1);
    esp_lcd_panel_io_tx_param(io, LCD_CMD_COLMOD, (uint8_t[]){ctx->colmod_val}, 1);

    for (int i = 0; k_vendor_init_cmds[i].data_bytes != 0xff; ++i)
    {
        esp_lcd_panel_io_tx_param(io, k_vendor_init_cmds[i].cmd, k_vendor_init_cmds[i].data,
                                  k_vendor_init_cmds[i].data_bytes);
    }

    esp_lcd_panel_io_tx_param(io, LCD_CMD_DISPON, nullptr, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    return ESP_OK;
}

static esp_err_t panel_st7796_draw_bitmap(
    esp_lcd_panel_t* panel, int x_start, int y_start, int x_end, int y_end, const void* color_data)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;

    x_start += ctx->x_gap;
    x_end += ctx->x_gap;
    y_start += ctx->y_gap;
    y_end += ctx->y_gap;

    esp_lcd_panel_io_tx_param(io, LCD_CMD_CASET,
                              (uint8_t[]){
                                  (uint8_t)((x_start >> 8) & 0xFF),
                                  (uint8_t)(x_start & 0xFF),
                                  (uint8_t)(((x_end - 1) >> 8) & 0xFF),
                                  (uint8_t)((x_end - 1) & 0xFF),
                              },
                              4);
    esp_lcd_panel_io_tx_param(io, LCD_CMD_RASET,
                              (uint8_t[]){
                                  (uint8_t)((y_start >> 8) & 0xFF),
                                  (uint8_t)(y_start & 0xFF),
                                  (uint8_t)(((y_end - 1) >> 8) & 0xFF),
                                  (uint8_t)((y_end - 1) & 0xFF),
                              },
                              4);
    size_t len =
        (size_t)(x_end - x_start) * (size_t)(y_end - y_start) * (size_t)ctx->fb_bits_per_pixel / 8;
    esp_lcd_panel_io_tx_color(io, LCD_CMD_RAMWR, color_data, len);
    return ESP_OK;
}

static esp_err_t panel_st7796_invert_color(esp_lcd_panel_t* panel, bool invert)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;
    esp_lcd_panel_io_tx_param(io, invert ? LCD_CMD_INVON : LCD_CMD_INVOFF, nullptr, 0);
    return ESP_OK;
}

static esp_err_t panel_st7796_mirror(esp_lcd_panel_t* panel, bool mirror_x, bool mirror_y)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;

    if (mirror_x)
        ctx->madctl_val |= LCD_CMD_MX_BIT;
    else
        ctx->madctl_val &= ~LCD_CMD_MX_BIT;

    if (mirror_y)
        ctx->madctl_val |= LCD_CMD_MY_BIT;
    else
        ctx->madctl_val &= ~LCD_CMD_MY_BIT;

    esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]){ctx->madctl_val}, 1);
    return ESP_OK;
}

static esp_err_t panel_st7796_swap_xy(esp_lcd_panel_t* panel, bool swap)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;

    if (swap)
        ctx->madctl_val |= LCD_CMD_MV_BIT;
    else
        ctx->madctl_val &= ~LCD_CMD_MV_BIT;

    esp_lcd_panel_io_tx_param(io, LCD_CMD_MADCTL, (uint8_t[]){ctx->madctl_val}, 1);
    return ESP_OK;
}

static esp_err_t panel_st7796_set_gap(esp_lcd_panel_t* panel, int x_gap, int y_gap)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    ctx->x_gap = x_gap;
    ctx->y_gap = y_gap;
    return ESP_OK;
}

static esp_err_t panel_st7796_disp_on_off(esp_lcd_panel_t* panel, bool on_off)
{
    st7796_panel_t* ctx = __containerof(panel, st7796_panel_t, base);
    esp_lcd_panel_io_handle_t io = ctx->io;
    esp_lcd_panel_io_tx_param(io, on_off ? LCD_CMD_DISPON : LCD_CMD_DISPOFF, nullptr, 0);
    return ESP_OK;
}

static esp_err_t esp_lcd_new_panel_st7796(const esp_lcd_panel_io_handle_t io,
                                          const esp_lcd_panel_dev_config_t* panel_dev_config,
                                          esp_lcd_panel_handle_t* ret_panel)
{
    esp_err_t ret = ESP_OK;
    st7796_panel_t* ctx = nullptr;

    ESP_GOTO_ON_FALSE(io && panel_dev_config && ret_panel, ESP_ERR_INVALID_ARG, err, TAG_ST7796,
                      "invalid argument");
    ctx = static_cast<st7796_panel_t*>(calloc(1, sizeof(st7796_panel_t)));
    ESP_GOTO_ON_FALSE(ctx, ESP_ERR_NO_MEM, err, TAG_ST7796, "no mem for st7796 panel");

    if (panel_dev_config->reset_gpio_num >= 0)
    {
        gpio_config_t io_conf = {};
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = 1ULL << panel_dev_config->reset_gpio_num;
        ESP_GOTO_ON_ERROR(gpio_config(&io_conf), err, TAG_ST7796, "configure RST GPIO failed");
    }

    // MADCTL: BGR bit
    ctx->madctl_val = LCD_CMD_BGR_BIT;

    switch (panel_dev_config->bits_per_pixel)
    {
        case 16:
            ctx->colmod_val = 0x55;
            ctx->fb_bits_per_pixel = 16;
            break;
        case 18:
            ctx->colmod_val = 0x66;
            ctx->fb_bits_per_pixel = 24;
            break;
        default:
            ESP_GOTO_ON_FALSE(false, ESP_ERR_NOT_SUPPORTED, err, TAG_ST7796,
                              "unsupported pixel width");
            break;
    }

    ctx->io = io;
    ctx->reset_gpio_num = panel_dev_config->reset_gpio_num;
    ctx->reset_level = panel_dev_config->flags.reset_active_high;
    ctx->base.del = panel_st7796_del;
    ctx->base.reset = panel_st7796_reset;
    ctx->base.init = panel_st7796_init;
    ctx->base.draw_bitmap = panel_st7796_draw_bitmap;
    ctx->base.invert_color = panel_st7796_invert_color;
    ctx->base.set_gap = panel_st7796_set_gap;
    ctx->base.mirror = panel_st7796_mirror;
    ctx->base.swap_xy = panel_st7796_swap_xy;
    ctx->base.disp_on_off = panel_st7796_disp_on_off;
    *ret_panel = &ctx->base;

    ESP_LOGD(TAG_ST7796, "new st7796 panel @%p", ctx);
    return ESP_OK;

err:
    if (ctx)
    {
        if (panel_dev_config->reset_gpio_num >= 0)
            gpio_reset_pin(static_cast<gpio_num_t>(panel_dev_config->reset_gpio_num));
        free(ctx);
    }
    return ret;
}

// ---------------------------------------------------------------------------
// wrapper::St7796
// ---------------------------------------------------------------------------

namespace wrapper
{

bool St7796::Init(const SpiBus& bus, const SpiDisplayConfig& config)
{
    return SpiDisplay::Init(bus, config, esp_lcd_new_panel_st7796);
}

}  // namespace wrapper
