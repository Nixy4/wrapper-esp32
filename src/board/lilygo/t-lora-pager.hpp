#pragma once

#include "sdkconfig.h"
#include "wrapper/logger.hpp"
#include "wrapper/i2c.hpp"
#include "wrapper/spi.hpp"
#include "wrapper/i2s.hpp"
#include "wrapper/ledc.hpp"
#include "wrapper/display.hpp"
#include "wrapper/audio.hpp"
#include "wrapper/lvgl.hpp"
#include "device/xl9555.hpp"
#include "device/st7796.hpp"
#include "device/tca8418.hpp"
#include "device/bq25896.hpp"

namespace wrapper
{

/**
 * @brief LilyGo T-LoraPager board support class (ESP32-S3)
 *
 * Singleton that owns and initialises all on-board peripherals:
 *   - I²C bus (SDA=3, SCL=2) — shared by XL9555, BQ25896, TCA8418, ES8311
 *   - Shared SPI bus (MOSI=34, MISO=33, SCK=35) — ST7796, LoRa, NFC, SD
 *   - I²S bus (MCLK=10, SCK=11, WS=18, Dout=45, Din=17) — ES8311 codec
 *   - XL9555 16-bit IO expander (addr=0x20) — peripheral power control
 *   - ST7796 SPI LCD (480×222, CS=38, DC=37, BL=42)
 *   - LVGL port backed by the ST7796
 *   - ES8311 audio codec via AudioCodec
 *   - TCA8418 keyboard matrix controller (addr=0x34, INT=GPIO6)
 *   - BQ25896 battery charger (addr=0x6B)
 *
 * Usage:
 * @code
 *   auto& board = wrapper::LilyGoLoraPager::GetInstance();
 *   board.InitCoreBusAndIoExpander();
 *   board.InitDisplay();
 *   board.GetLvglPort().SetRotation(LV_DISPLAY_ROTATION_0);
 *   board.InitAudio();
 *   board.InitKeyboard();
 * @endcode
 */
class LilyGoLoraPager
{
   private:
    LilyGoLoraPager() = default;
    ~LilyGoLoraPager() = default;

    LilyGoLoraPager(const LilyGoLoraPager&) = delete;
    LilyGoLoraPager& operator=(const LilyGoLoraPager&) = delete;
    LilyGoLoraPager(LilyGoLoraPager&&) = delete;
    LilyGoLoraPager& operator=(LilyGoLoraPager&&) = delete;

   public:
    static LilyGoLoraPager& GetInstance()
    {
        static LilyGoLoraPager instance;
        return instance;
    }

    // -----------------------------------------------------------------------
    // Initialisation — call in order
    // -----------------------------------------------------------------------

    /** @brief Init I²C bus + XL9555 IO expander + BQ25896 charger */
    bool InitCoreBusAndIoExpander();

    /**
     * @brief Init shared SPI bus + ST7796 display + LEDC backlight + LVGL.
     *
     * Also configures LoRa / NFC / SD chip-select GPIOs to HIGH so they
     * do not interfere with SPI bus initialisation.
     */
    bool InitDisplay();

    /** @brief Init I²S bus + ES8311 audio codec (speaker + microphone) */
    bool InitAudio();

    /** @brief Init TCA8418 keyboard matrix controller (4 rows × 10 cols) */
    bool InitKeyboard();

    // -----------------------------------------------------------------------
    // Getters
    // -----------------------------------------------------------------------

    I2cBus& GetI2cBus();
    SpiBus& GetSpiBus();
    I2sBus& GetI2sBus();
    Xl9555& GetIoExpander();
    St7796& GetDisplay();
    LedcTimer& GetLedcTimer();
    LedcChannel& GetLedcChannel();
    AudioCodec& GetAudioCodec();
    LvglPort& GetLvglPort();
    Tca8418& GetKeyboard();
    Bq25896& GetPmu();

    // -----------------------------------------------------------------------
    // Convenience helpers
    // -----------------------------------------------------------------------

    /** @brief Set display backlight 0–100 % */
    void SetDisplayBrightness(int percent);

    /** @brief Turn backlight on / off */
    void SetDisplayBacklight(bool on);

    /**
     * @brief Control a peripheral power rail via XL9555.
     *
     * @param xl9555_pin  IO expander pin number (0-15). Use the kXl9555Pin*
     *                    constants defined in t-lora-pager.cpp once the
     *                    schematic has been verified.
     * @param enable      true = HIGH (enable), false = LOW (disable)
     */
    bool SetPeripheralPower(uint32_t xl9555_pin, bool enable);
};

}  // namespace wrapper
