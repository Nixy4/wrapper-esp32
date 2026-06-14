#include "tca8418.hpp"

namespace wrapper
{

Tca8418::Tca8418(Logger& logger) : logger_(logger), device_(logger) {}

bool Tca8418::Init(const I2cBus& bus, const I2cDeviceConfig& config, uint8_t rows, uint8_t cols)
{
    if (rows == 0 || rows > 8 || cols == 0 || cols > 10)
    {
        logger_.Error("Invalid matrix size %u x %u", rows, cols);
        return false;
    }

    if (!device_.Init(bus, config))
    {
        logger_.Error("Failed to initialize I2C device");
        return false;
    }

    rows_ = rows;
    cols_ = cols;

    // --- Configure rows as keypad pins ---
    // TCA8418 rows are R0-R7 (mapped to port 1 bits 0-7).
    // KP_GPIO1 bits: 1 = keypad, 0 = GPIO. Rows 0..rows_-1 → set to keypad.
    uint8_t row_mask = static_cast<uint8_t>((1u << rows_) - 1u);
    if (!device_.WriteReg8(REG_KP_GPIO1, row_mask, -1))
    {
        logger_.Error("Failed to set KP_GPIO1 (row mask 0x%02x)", row_mask);
        return false;
    }

    // --- Configure columns as keypad pins ---
    // Columns C0-C9 span KP_GPIO2 (bits 0-7 = C0-C7) and KP_GPIO3 (bit 0 = C8, bit 1 = C9).
    uint8_t kp_gpio2 = 0xFF;  // C0-C7
    uint8_t kp_gpio3 = 0x00;
    if (cols_ > 8)
    {
        uint8_t extra = static_cast<uint8_t>((1u << (cols_ - 8u)) - 1u);
        kp_gpio3 = extra;
    }
    if (!device_.WriteReg8(REG_KP_GPIO2, kp_gpio2, -1) ||
        !device_.WriteReg8(REG_KP_GPIO3, kp_gpio3, -1))
    {
        logger_.Error("Failed to set KP_GPIO2/3");
        return false;
    }

    // --- Enable key event interrupt, auto-increment OFF ---
    if (!device_.WriteReg8(REG_CFG, CFG_KE_IEN, -1))
    {
        logger_.Error("Failed to set CFG register");
        return false;
    }

    // Clear any stale interrupt
    ClearInterrupt();

    logger_.Info("TCA8418 initialized: %u rows x %u cols (addr: 0x%02x)", rows_, cols_,
                 config.device_address);
    return true;
}

bool Tca8418::Deinit() { return device_.Deinit(); }

bool Tca8418::EventAvailable()
{
    uint8_t stat = 0;
    if (!device_.ReadReg8(REG_INT_STAT, stat, -1))
        return false;
    return (stat & INT_K_INT) != 0;
}

bool Tca8418::GetKeyEvent(uint8_t& key_code, bool& pressed)
{
    // Check event count
    uint8_t ec = 0;
    if (!device_.ReadReg8(REG_KEY_LCK_EC, ec, -1))
        return false;

    ec &= 0x0F;  // lower 4 bits = event count
    if (ec == 0)
        return false;

    uint8_t ev = 0;
    if (!device_.ReadReg8(REG_KEY_EVENT_A, ev, -1))
        return false;

    pressed = (ev & 0x80) != 0;
    key_code = ev & 0x7F;

    // Clear K_INT after reading
    if (!device_.WriteReg8(REG_INT_STAT, INT_K_INT, -1))
        logger_.Warning("Failed to clear K_INT");

    return true;
}

bool Tca8418::ClearInterrupt()
{
    uint8_t stat = 0;
    if (!device_.ReadReg8(REG_INT_STAT, stat, -1))
        return false;
    return device_.WriteReg8(REG_INT_STAT, stat, -1);
}

}  // namespace wrapper
