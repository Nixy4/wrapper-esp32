#include <esp_lcd_ili9881c.h>
#include <esp_lcd_touch_gt911.h>
#include "board/m5stack/tab5.hpp"

namespace wrapper
{
  static const ili9881c_lcd_init_cmd_t m5tab5_disp_init_data[] = {
      // {cmd, { data }, data_size, delay}

      /**** CMD_Page 1 ****/
      {0xFF, (uint8_t[]){0x98, 0x81, 0x01}, 3, 0},
      {0xB7, (uint8_t[]){0x03}, 1, 0}, // set 2 lane

      /**** CMD_Page 3 ****/
      {0xFF, (uint8_t[]){0x98, 0x81, 0x03}, 3, 0},
      {0x01, (uint8_t[]){0x00}, 1, 0},
      {0x02, (uint8_t[]){0x00}, 1, 0},
      {0x03, (uint8_t[]){0x73}, 1, 0},
      {0x04, (uint8_t[]){0x00}, 1, 0},
      {0x05, (uint8_t[]){0x00}, 1, 0},
      {0x06, (uint8_t[]){0x08}, 1, 0},
      {0x07, (uint8_t[]){0x00}, 1, 0},
      {0x08, (uint8_t[]){0x00}, 1, 0},
      {0x09, (uint8_t[]){0x1B}, 1, 0},
      {0x0a, (uint8_t[]){0x01}, 1, 0},
      {0x0b, (uint8_t[]){0x01}, 1, 0},
      {0x0c, (uint8_t[]){0x0D}, 1, 0},
      {0x0d, (uint8_t[]){0x01}, 1, 0},
      {0x0e, (uint8_t[]){0x01}, 1, 0},
      {0x0f, (uint8_t[]){0x26}, 1, 0},
      {0x10, (uint8_t[]){0x26}, 1, 0},
      {0x11, (uint8_t[]){0x00}, 1, 0},
      {0x12, (uint8_t[]){0x00}, 1, 0},
      {0x13, (uint8_t[]){0x02}, 1, 0},
      {0x14, (uint8_t[]){0x00}, 1, 0},
      {0x15, (uint8_t[]){0x00}, 1, 0},
      {0x16, (uint8_t[]){0x00}, 1, 0},
      {0x17, (uint8_t[]){0x00}, 1, 0},
      {0x18, (uint8_t[]){0x00}, 1, 0},
      {0x19, (uint8_t[]){0x00}, 1, 0},
      {0x1a, (uint8_t[]){0x00}, 1, 0},
      {0x1b, (uint8_t[]){0x00}, 1, 0},
      {0x1c, (uint8_t[]){0x00}, 1, 0},
      {0x1d, (uint8_t[]){0x00}, 1, 0},
      {0x1e, (uint8_t[]){0x40}, 1, 0},
      {0x1f, (uint8_t[]){0x00}, 1, 0},
      {0x20, (uint8_t[]){0x06}, 1, 0},
      {0x21, (uint8_t[]){0x01}, 1, 0},
      {0x22, (uint8_t[]){0x00}, 1, 0},
      {0x23, (uint8_t[]){0x00}, 1, 0},
      {0x24, (uint8_t[]){0x00}, 1, 0},
      {0x25, (uint8_t[]){0x00}, 1, 0},
      {0x26, (uint8_t[]){0x00}, 1, 0},
      {0x27, (uint8_t[]){0x00}, 1, 0},
      {0x28, (uint8_t[]){0x33}, 1, 0},
      {0x29, (uint8_t[]){0x03}, 1, 0},
      {0x2a, (uint8_t[]){0x00}, 1, 0},
      {0x2b, (uint8_t[]){0x00}, 1, 0},
      {0x2c, (uint8_t[]){0x00}, 1, 0},
      {0x2d, (uint8_t[]){0x00}, 1, 0},
      {0x2e, (uint8_t[]){0x00}, 1, 0},
      {0x2f, (uint8_t[]){0x00}, 1, 0},
      {0x30, (uint8_t[]){0x00}, 1, 0},
      {0x31, (uint8_t[]){0x00}, 1, 0},
      {0x32, (uint8_t[]){0x00}, 1, 0},
      {0x33, (uint8_t[]){0x00}, 1, 0},
      {0x34, (uint8_t[]){0x00}, 1, 0},
      {0x35, (uint8_t[]){0x00}, 1, 0},
      {0x36, (uint8_t[]){0x00}, 1, 0},
      {0x37, (uint8_t[]){0x00}, 1, 0},
      {0x38, (uint8_t[]){0x00}, 1, 0},
      {0x39, (uint8_t[]){0x00}, 1, 0},
      {0x3a, (uint8_t[]){0x00}, 1, 0},
      {0x3b, (uint8_t[]){0x00}, 1, 0},
      {0x3c, (uint8_t[]){0x00}, 1, 0},
      {0x3d, (uint8_t[]){0x00}, 1, 0},
      {0x3e, (uint8_t[]){0x00}, 1, 0},
      {0x3f, (uint8_t[]){0x00}, 1, 0},
      {0x40, (uint8_t[]){0x00}, 1, 0},
      {0x41, (uint8_t[]){0x00}, 1, 0},
      {0x42, (uint8_t[]){0x00}, 1, 0},
      {0x43, (uint8_t[]){0x00}, 1, 0},
      {0x44, (uint8_t[]){0x00}, 1, 0},

      {0x50, (uint8_t[]){0x01}, 1, 0},
      {0x51, (uint8_t[]){0x23}, 1, 0},
      {0x52, (uint8_t[]){0x45}, 1, 0},
      {0x53, (uint8_t[]){0x67}, 1, 0},
      {0x54, (uint8_t[]){0x89}, 1, 0},
      {0x55, (uint8_t[]){0xab}, 1, 0},
      {0x56, (uint8_t[]){0x01}, 1, 0},
      {0x57, (uint8_t[]){0x23}, 1, 0},
      {0x58, (uint8_t[]){0x45}, 1, 0},
      {0x59, (uint8_t[]){0x67}, 1, 0},
      {0x5a, (uint8_t[]){0x89}, 1, 0},
      {0x5b, (uint8_t[]){0xab}, 1, 0},
      {0x5c, (uint8_t[]){0xcd}, 1, 0},
      {0x5d, (uint8_t[]){0xef}, 1, 0},

      {0x5e, (uint8_t[]){0x11}, 1, 0},
      {0x5f, (uint8_t[]){0x02}, 1, 0},
      {0x60, (uint8_t[]){0x00}, 1, 0},
      {0x61, (uint8_t[]){0x07}, 1, 0},
      {0x62, (uint8_t[]){0x06}, 1, 0},
      {0x63, (uint8_t[]){0x0E}, 1, 0},
      {0x64, (uint8_t[]){0x0F}, 1, 0},
      {0x65, (uint8_t[]){0x0C}, 1, 0},
      {0x66, (uint8_t[]){0x0D}, 1, 0},
      {0x67, (uint8_t[]){0x02}, 1, 0},
      {0x68, (uint8_t[]){0x02}, 1, 0},
      {0x69, (uint8_t[]){0x02}, 1, 0},
      {0x6a, (uint8_t[]){0x02}, 1, 0},
      {0x6b, (uint8_t[]){0x02}, 1, 0},
      {0x6c, (uint8_t[]){0x02}, 1, 0},
      {0x6d, (uint8_t[]){0x02}, 1, 0},
      {0x6e, (uint8_t[]){0x02}, 1, 0},
      {0x6f, (uint8_t[]){0x02}, 1, 0},
      {0x70, (uint8_t[]){0x02}, 1, 0},
      {0x71, (uint8_t[]){0x02}, 1, 0},
      {0x72, (uint8_t[]){0x02}, 1, 0},
      {0x73, (uint8_t[]){0x05}, 1, 0},
      {0x74, (uint8_t[]){0x01}, 1, 0},
      {0x75, (uint8_t[]){0x02}, 1, 0},
      {0x76, (uint8_t[]){0x00}, 1, 0},
      {0x77, (uint8_t[]){0x07}, 1, 0},
      {0x78, (uint8_t[]){0x06}, 1, 0},
      {0x79, (uint8_t[]){0x0E}, 1, 0},
      {0x7a, (uint8_t[]){0x0F}, 1, 0},
      {0x7b, (uint8_t[]){0x0C}, 1, 0},
      {0x7c, (uint8_t[]){0x0D}, 1, 0},
      {0x7d, (uint8_t[]){0x02}, 1, 0},
      {0x7e, (uint8_t[]){0x02}, 1, 0},
      {0x7f, (uint8_t[]){0x02}, 1, 0},
      {0x80, (uint8_t[]){0x02}, 1, 0},
      {0x81, (uint8_t[]){0x02}, 1, 0},
      {0x82, (uint8_t[]){0x02}, 1, 0},
      {0x83, (uint8_t[]){0x02}, 1, 0},
      {0x84, (uint8_t[]){0x02}, 1, 0},
      {0x85, (uint8_t[]){0x02}, 1, 0},
      {0x86, (uint8_t[]){0x02}, 1, 0},
      {0x87, (uint8_t[]){0x02}, 1, 0},
      {0x88, (uint8_t[]){0x02}, 1, 0},
      {0x89, (uint8_t[]){0x05}, 1, 0},
      {0x8A, (uint8_t[]){0x01}, 1, 0},

      /**** CMD_Page 4 ****/
      {0xFF, (uint8_t[]){0x98, 0x81, 0x04}, 3, 0},
      {0x38, (uint8_t[]){0x01}, 1, 0},
      {0x39, (uint8_t[]){0x00}, 1, 0},
      {0x6C, (uint8_t[]){0x15}, 1, 0},
      {0x6E, (uint8_t[]){0x1A}, 1, 0},
      {0x6F, (uint8_t[]){0x25}, 1, 0},
      {0x3A, (uint8_t[]){0xA4}, 1, 0},
      {0x8D, (uint8_t[]){0x20}, 1, 0},
      {0x87, (uint8_t[]){0xBA}, 1, 0},
      {0x3B, (uint8_t[]){0x98}, 1, 0},

      /**** CMD_Page 1 ****/
      {0xFF, (uint8_t[]){0x98, 0x81, 0x01}, 3, 0},
      {0x22, (uint8_t[]){0x0A}, 1, 0},
      {0x31, (uint8_t[]){0x00}, 1, 0},
      {0x50, (uint8_t[]){0x6B}, 1, 0},
      {0x51, (uint8_t[]){0x66}, 1, 0},
      {0x53, (uint8_t[]){0x73}, 1, 0},
      {0x55, (uint8_t[]){0x8B}, 1, 0},
      {0x60, (uint8_t[]){0x1B}, 1, 0},
      {0x61, (uint8_t[]){0x01}, 1, 0},
      {0x62, (uint8_t[]){0x0C}, 1, 0},
      {0x63, (uint8_t[]){0x00}, 1, 0},

      // Gamma P
      {0xA0, (uint8_t[]){0x00}, 1, 0},
      {0xA1, (uint8_t[]){0x15}, 1, 0},
      {0xA2, (uint8_t[]){0x1F}, 1, 0},
      {0xA3, (uint8_t[]){0x13}, 1, 0},
      {0xA4, (uint8_t[]){0x11}, 1, 0},
      {0xA5, (uint8_t[]){0x21}, 1, 0},
      {0xA6, (uint8_t[]){0x17}, 1, 0},
      {0xA7, (uint8_t[]){0x1B}, 1, 0},
      {0xA8, (uint8_t[]){0x6B}, 1, 0},
      {0xA9, (uint8_t[]){0x1E}, 1, 0},
      {0xAA, (uint8_t[]){0x2B}, 1, 0},
      {0xAB, (uint8_t[]){0x5D}, 1, 0},
      {0xAC, (uint8_t[]){0x19}, 1, 0},
      {0xAD, (uint8_t[]){0x14}, 1, 0},
      {0xAE, (uint8_t[]){0x4B}, 1, 0},
      {0xAF, (uint8_t[]){0x1D}, 1, 0},
      {0xB0, (uint8_t[]){0x27}, 1, 0},
      {0xB1, (uint8_t[]){0x49}, 1, 0},
      {0xB2, (uint8_t[]){0x5D}, 1, 0},
      {0xB3, (uint8_t[]){0x39}, 1, 0},

      // Gamma N
      {0xC0, (uint8_t[]){0x00}, 1, 0},
      {0xC1, (uint8_t[]){0x01}, 1, 0},
      {0xC2, (uint8_t[]){0x0C}, 1, 0},
      {0xC3, (uint8_t[]){0x11}, 1, 0},
      {0xC4, (uint8_t[]){0x15}, 1, 0},
      {0xC5, (uint8_t[]){0x28}, 1, 0},
      {0xC6, (uint8_t[]){0x1B}, 1, 0},
      {0xC7, (uint8_t[]){0x1C}, 1, 0},
      {0xC8, (uint8_t[]){0x62}, 1, 0},
      {0xC9, (uint8_t[]){0x1C}, 1, 0},
      {0xCA, (uint8_t[]){0x29}, 1, 0},
      {0xCB, (uint8_t[]){0x60}, 1, 0},
      {0xCC, (uint8_t[]){0x16}, 1, 0},
      {0xCD, (uint8_t[]){0x17}, 1, 0},
      {0xCE, (uint8_t[]){0x4A}, 1, 0},
      {0xCF, (uint8_t[]){0x23}, 1, 0},
      {0xD0, (uint8_t[]){0x24}, 1, 0},
      {0xD1, (uint8_t[]){0x4F}, 1, 0},
      {0xD2, (uint8_t[]){0x5F}, 1, 0},
      {0xD3, (uint8_t[]){0x39}, 1, 0},

      /**** CMD_Page 0 ****/
      {0xFF, (uint8_t[]){0x98, 0x81, 0x00}, 3, 0},
      {0x35, (uint8_t[]){0x00}, 0, 0},
      {0xFE, (uint8_t[]){0x00}, 0, 0},
      {0x29, (uint8_t[]){0x00}, 0, 0},
  };

  I2cBusConfig i2c_cfg(
      I2C_NUM_0,           // port
      GPIO_NUM_31,         // sda (M5TAB5_I2C_SDA)
      GPIO_NUM_32,         // scl (M5TAB5_I2C_SCL)
      I2C_CLK_SRC_DEFAULT, // clk_src
      7,                   // glitch_ignore_cnt
      0,                   // intr_priority
      0,                   // trans_queue_depth (使用默认值)
      true,                // enable_internal_pullup
      false                // allow_pd
  );

  // M5Stack Tab5 背光配置
  LedcTimerConfig ledc_timer_cfg(
      LEDC_LOW_SPEED_MODE, // speed_mode
      LEDC_TIMER_10_BIT,   // duty_resolution (0-1023)
      LEDC_TIMER_0,        // timer_num
      5000,                // freq_hz (5kHz)
      LEDC_AUTO_CLK        // clk_cfg
  );

  LedcChannelConfig ledc_channel_cfg(
      GPIO_NUM_22,         // gpio_num (M5TAB5_LCD_BACKLIGHT)
      LEDC_LOW_SPEED_MODE, // speed_mode
      LEDC_CHANNEL_0,      // channel
      LEDC_INTR_DISABLE,   // intr_type
      LEDC_TIMER_0,        // timer_sel
      0,                   // duty (初始为0)
      0                    // hpoint
  );

  LdoChannelConfig dsi_phy_ldo_cfg(
      3,     // chan_id (LDO3 for MIPI DSI PHY)
      2500,  // voltage_mv (2.5V)
      false, // adjustable
      false  // owned_by_hw
  );

  DsiBusConfig dsi_bus_cfg(
      0,                            // bus_id
      2,                            // num_data_lanes
      MIPI_DSI_PHY_CLK_SRC_DEFAULT, // phy_clk_src
      1000                          // lane_bit_rate_mbps
  );

  DsiDisplayConfig dsi_display_cfg(
      // DBI IO config parameters
      0, // dbi_virtual_channel
      8, // lcd_cmd_bits
      8, // lcd_param_bits
      // DPI Panel config parameters
      0,                                  // dpi_virtual_channel
      MIPI_DSI_DPI_CLK_SRC_DEFAULT,       // dpi_clk_src
      60,                                 // dpi_clock_freq_mhz
      LCD_COLOR_PIXEL_FORMAT_RGB565,      // pixel_format
      static_cast<lcd_color_format_t>(0), // in_color_format
      static_cast<lcd_color_format_t>(0), // out_color_format
      1,                                  // num_fbs (官方默认值)
      // video_timing nested struct members
      720,  // h_size (BSP_LCD_H_RES)
      1280, // v_size (BSP_LCD_V_RES)
      40,   // hsync_pulse_width
      140,  // hsync_back_porch
      40,   // hsync_front_porch
      4,    // vsync_pulse_width
      20,   // vsync_back_porch
      20,   // vsync_front_porch
      // flags nested struct members
      false, // use_dma2d (单帧缓冲区时禁用以避免状态机冲突)
      false, // disable_lp
      // Panel Device config parameters
      GPIO_NUM_NC,               // reset_gpio_num
      LCD_RGB_ELEMENT_ORDER_RGB, // rgb_ele_order
      LCD_RGB_DATA_ENDIAN_BIG,   // data_endian
      16,                        // bits_per_pixel
      false                      // reset_active_high
  );

  ili9881c_vendor_config_t dsi_vendor_cfg = {
      .init_cmds = m5tab5_disp_init_data,
      .init_cmds_size = sizeof(m5tab5_disp_init_data) / sizeof(m5tab5_disp_init_data[0]),
      .mipi_config = {
          .dsi_bus = nullptr,    /* Set at runtime */
          .dpi_config = nullptr, /* Set at runtime */
          .lane_num = 2,
      },
  };

  I2cTouchConfig gt911_touch_cfg(
      ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS_BACKUP,
      nullptr,
      nullptr,
      1,
      0,
      16,
      0,
      0,
      0,
      400000,
      1280,
      800,
      GPIO_NUM_NC,
      GPIO_NUM_NC,
      0,
      0,
      0,
      0,
      0,
      nullptr,
      nullptr);

  I2sBusConfig i2s_bus_cfg(
      I2S_NUM_0,
      I2S_ROLE_MASTER,
      6,
      256,
      true,
      false,
      0);

  I2sChanStdConfig tx_rx_std_cfg(
      // CLK config (5 params)
      48000,                 // sample_rate_hz
      I2S_CLK_SRC_DEFAULT,   // clk_src
      0,                     // ext_clk_freq_hz
      I2S_MCLK_MULTIPLE_256, // mclk_multiple
      8,                     // bclk_div
      // SLOT config (10 params)
      I2S_DATA_BIT_WIDTH_16BIT, // data_bit_width
      I2S_SLOT_BIT_WIDTH_AUTO,  // slot_bit_width
      I2S_SLOT_MODE_MONO,       // slot_mode
      I2S_STD_SLOT_BOTH,        // slot_mask
      16,                       // ws_width
      false,                    // ws_pol
      true,                     // bit_shift
      true,                     // msb_right
      false,                    // left_align
      false,                    // big_endian
      // GPIO config (8 params)
      GPIO_NUM_30, // mclk
      GPIO_NUM_27, // bclk
      GPIO_NUM_29, // ws
      GPIO_NUM_26, // dout
      GPIO_NUM_28, // din
      false,       // invert_mclk
      false,       // invert_bclk
      false        // invert_ws
  );

  LvglPortConfig lvgl_port_cfg(
      5,          // task_prio
      8192,       // stack_sz
      1,          // affinity
      20,         // max_sleep_ms
      MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT, // stack_caps
      25          // timer_ms
  );

  LvglDisplayConfig lvgl_display_cfg(
      720 * 50,   // buf_sz (BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT)
      true,       // double_buf
      0,          // trans_sz
      720,        // hor_res
      1280,       // ver_res
      false,      // mono
      false,      // swap_xy
      false,      // mirror_x
      false,      // mirror_y
      LV_COLOR_FORMAT_RGB565, // format
      true,       // buff_dma
      false,      // buff_spiram
      true,       // sw_rotate (官方默认启用)
      false,      // swap_bytes (BSP_LCD_BIGENDIAN=0)
      false,      // full_refresh
      false       // direct_mode
  );

  LvglTouchConfig lvgl_touch_cfg(
      0.0f,   // scale_x
      0.0f    // scale_y
  );

  LvglDisplayDsiConfig lvgl_dsi_cfg(
      false   // avoid_tearing (官方默认，需要num_fbs>1才能启用)
  );

  Logger lmain("Main");
  Logger li2c("Board", "I2C", "Bus");
  Logger lioexp0("Board", "I2C", "IoExpander0");
  Logger lioexp1("Board", "I2C", "IoExpander1");
  Logger lledc("Board", "LEDC");
  Logger lldo("Board", "LDO");
  Logger ldisp("Board", "Display");
  Logger ltouch("Board", "Touch");
  Logger laudio("Board", "Audio");
  Logger llvgl("Board", "LVGL");

  I2cBus i2c_bus(li2c);
  Pi4ioe5v6408 io_expander0(lioexp0); // 0x43
  Pi4ioe5v6408 io_expander1(lioexp1); // 0x44
  LedcTimer ledc_timer(lledc);
  LedcChannel ledc_channel(lledc);
  LdoRegulator dsi_phy_ldo(lldo);

  DsiBus dsi_bus(ldisp);
  DsiDisplay dsi_display(ldisp);
  I2cTouch gt911_touch(ltouch);

  I2sBus i2s_bus(laudio);
  LvglPort lvgl_port(llvgl);

  AudioCodec audio_codec(laudio);
  std::function<esp_err_t()> spk_codec_new_func = []() -> esp_err_t
  {
    es8388_codec_cfg_t spk_codec_cfg = {
        .ctrl_if = audio_codec.GetSpeakerCtrlInterface(),
        .gpio_if = audio_codec.GetGpioInterface(),
        .codec_mode = ESP_CODEC_DEV_WORK_MODE_DAC,
        .master_mode = false,
        .pa_pin = GPIO_NUM_NC,
        .pa_reverted = false,
        .hw_gain = {.pa_voltage = 5.0, .codec_dac_voltage = 3.3, .pa_gain = 0.0f},
    };

    const audio_codec_if_t *codec_if = es8388_codec_new(&spk_codec_cfg);
    if (codec_if == NULL)
    {
      return ESP_ERR_INVALID_STATE;
    }
    audio_codec.SetSpeakerCodecInterface(codec_if);

    esp_codec_dev_handle_t spk_codec_dev_handle = nullptr;
    esp_codec_dev_cfg_t spk_codec_dev_cfg = {
        .dev_type = ESP_CODEC_DEV_TYPE_OUT,
        .codec_if = codec_if,
        .data_if = audio_codec.GetDataInterface(),
    };
    spk_codec_dev_handle = esp_codec_dev_new(&spk_codec_dev_cfg);
    if (spk_codec_dev_handle == nullptr)
    {
      return ESP_ERR_INVALID_STATE;
    }
    audio_codec.SetSpeakerCodecDeviceHandle(spk_codec_dev_handle);
    return ESP_OK;
  };
  std::function<esp_err_t()> mic_codec_new_func = []() -> esp_err_t
  {
    es7210_codec_cfg_t mic_codec_cfg = {
        .ctrl_if = audio_codec.GetMicrophoneCtrlInterface(),
        .master_mode = false,
        .mic_selected = ES7210_SEL_MIC1,
        .mclk_src = ES7210_MCLK_FROM_PAD,
        .mclk_div = 0};

    const audio_codec_if_t *codec_if = es7210_codec_new(&mic_codec_cfg);
    if (codec_if == NULL)
    {
      return ESP_ERR_INVALID_STATE;
    }
    audio_codec.SetMicrophoneCodecInterface(codec_if);
    esp_codec_dev_handle_t mic_codec_dev_handle = nullptr;
    esp_codec_dev_cfg_t mic_codec_dev_cfg = {
        .dev_type = ESP_CODEC_DEV_TYPE_IN,
        .codec_if = codec_if,
        .data_if = audio_codec.GetDataInterface(),
    };
    mic_codec_dev_handle = esp_codec_dev_new(&mic_codec_dev_cfg);
    if (mic_codec_dev_handle == nullptr)
    {
      return ESP_ERR_INVALID_STATE;
    }
    audio_codec.SetMicrophoneCodecDeviceHandle(mic_codec_dev_handle);
    return ESP_OK;
  };

  bool M5StackTab5::Init()
  {
    // I2C Bus
    i2c_bus.Init(i2c_cfg);
    i2c_bus.Scan();

    // IO Expanders
    io_expander0.Init(i2c_bus, Pi4ioe5v6408::ADDR_LOW);  // 0x43
    io_expander1.Init(i2c_bus, Pi4ioe5v6408::ADDR_HIGH); // 0x44

    // IO Expander0 配置 (LCD, Touch, Speaker, Camera)
    io_expander0.SetDirection(IO_EXPANDER_PIN_NUM_4, IO_EXPANDER_OUTPUT); // LCD_EN
    io_expander0.SetLevel(IO_EXPANDER_PIN_NUM_4, 1);
    io_expander0.SetOutputMode(IO_EXPANDER_PIN_NUM_4, IO_EXPANDER_OUTPUT_MODE_PUSH_PULL);

    // 等待 LCD 电源稳定
    vTaskDelay(pdMS_TO_TICKS(1000));

    io_expander0.SetDirection(IO_EXPANDER_PIN_NUM_5, IO_EXPANDER_OUTPUT); // TOUCH_EN
    io_expander0.SetLevel(IO_EXPANDER_PIN_NUM_5, 1);

    io_expander0.SetDirection(IO_EXPANDER_PIN_NUM_1, IO_EXPANDER_OUTPUT); // SPEAKER_EN
    io_expander0.SetLevel(IO_EXPANDER_PIN_NUM_1, 1);

    io_expander1.SetDirection(IO_EXPANDER_PIN_NUM_3, IO_EXPANDER_OUTPUT); // USB_EN
    io_expander1.SetLevel(IO_EXPANDER_PIN_NUM_3, 1);

    io_expander1.SetDirection(IO_EXPANDER_PIN_NUM_0, IO_EXPANDER_OUTPUT); // WIFI_EN
    io_expander1.SetLevel(IO_EXPANDER_PIN_NUM_0, 1);

    // Backlight (LEDC)
    ledc_timer.Init(ledc_timer_cfg);
    ledc_channel.Init(ledc_channel_cfg);

    // DSI PHY Power (LDO)
    dsi_phy_ldo.Init(dsi_phy_ldo_cfg);

    // 等待 DSI PHY 电源稳定
    vTaskDelay(pdMS_TO_TICKS(100));

    dsi_bus.Init(dsi_bus_cfg);
    dsi_display.Init(
        dsi_bus,
        dsi_display_cfg,
        esp_lcd_new_panel_ili9881c,
        nullptr,
        &dsi_vendor_cfg,
        []()
        {
          dsi_vendor_cfg.mipi_config.dsi_bus = dsi_bus.GetHandle();
          dsi_vendor_cfg.mipi_config.dpi_config = &dsi_display_cfg.dpi_config;
        });

    // 官方 BSP 在初始化后调用 InvertColor(false)
    dsi_display.InvertColor(false);

    gt911_touch.Init(i2c_bus, gt911_touch_cfg, esp_lcd_touch_new_i2c_gt911);

    i2s_bus.Init(i2s_bus_cfg);
    i2s_bus.ConfigureTxChannel(tx_rx_std_cfg);
    i2s_bus.ConfigureRxChannel(tx_rx_std_cfg);
    i2s_bus.EnableTxChannel();
    i2s_bus.EnableRxChannel();

    audio_codec.Init(i2s_bus);
    audio_codec.AddSpeaker(i2c_bus, ES8388_CODEC_DEFAULT_ADDR, spk_codec_new_func);
    audio_codec.AddMicrophone(i2c_bus, ES7210_CODEC_DEFAULT_ADDR, mic_codec_new_func);

    lvgl_port.Init(lvgl_port_cfg);
    lvgl_port.AddDisplayDsi(dsi_display, lvgl_display_cfg, lvgl_dsi_cfg);
    lvgl_port.AddTouch(gt911_touch, lvgl_touch_cfg);

    // 开启背光
    SetDisplayBrightness(100);

    return true;
  }

  I2cBus& M5StackTab5::GetI2cBus()
  {
    return i2c_bus;
  }

  Pi4ioe5v6408& M5StackTab5::GetIoExpander0()
  {
    return io_expander0;
  }

  Pi4ioe5v6408& M5StackTab5::GetIoExpander1()
  {
    return io_expander1;
  }

  LedcTimer& M5StackTab5::GetLedcTimer()
  {
    return ledc_timer;
  }

  LedcChannel& M5StackTab5::GetLedcChannel()
  {
    return ledc_channel;
  }

  LdoRegulator& M5StackTab5::GetDsiPhyLdo()
  {
    return dsi_phy_ldo;
  }

  DsiBus& M5StackTab5::GetDsiBus()
  {
    return dsi_bus;
  }

  DsiDisplay& M5StackTab5::GetDsiDisplay()
  {
    return dsi_display;
  }

  I2cTouch& M5StackTab5::GetGt911Touch()
  {
    return gt911_touch;
  }

  I2sBus& M5StackTab5::GetI2sBus()
  {
    return i2s_bus;
  }

  AudioCodec& M5StackTab5::GetAudioCodec()
  {
    return audio_codec;
  }

  LvglPort& M5StackTab5::GetLvglPort()
  {
    return lvgl_port;
  }

  void M5StackTab5::SetDisplayBrightness(int percent)
  {
    if (percent < 0)
      percent = 0;
    if (percent > 100)
      percent = 100;
      
    // LEDC resolution set to 10bits (1023)
    uint32_t duty = (1023 * percent) / 100;
    ledc_channel.SetDutyAndUpdate(duty);
  }

  void M5StackTab5::SetDisplayBacklight(bool on)
  {
    SetDisplayBrightness(on ? 100 : 0);
  }

  void M5StackTab5::SetDisplayPower(bool on)
  {
    // IO_EXPANDER_PIN_NUM_4 is LCD_EN
    io_expander0.SetLevel(IO_EXPANDER_PIN_NUM_4, on ? 1 : 0);
  }
} // namespace wrapper