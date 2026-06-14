#include "xl9555.hpp"

namespace wrapper
{

Xl9555::Xl9555(Logger& logger) : logger_(logger), device_(logger) {}

bool Xl9555::Init(const I2cBus& bus, const I2cDeviceConfig& config)
{
    if (!device_.Init(bus, config))
    {
        logger_.Error("Failed to initialize I2C device");
        return false;
    }

    // Configure all pins as outputs (0x00), drive all HIGH (0xFF) by default
    if (!device_.WriteReg8(REG_OUTPUT_P0, output_p0_, -1) ||
        !device_.WriteReg8(REG_OUTPUT_P1, output_p1_, -1) ||
        !device_.WriteReg8(REG_CONFIG_P0, 0x00, -1) || !device_.WriteReg8(REG_CONFIG_P1, 0x00, -1))
    {
        logger_.Error("Failed to configure XL9555 registers");
        return false;
    }
    config_p0_ = 0x00;
    config_p1_ = 0x00;

    logger_.Info("XL9555 initialized (addr: 0x%02x)", config.device_address);
    return true;
}

bool Xl9555::Deinit() { return device_.Deinit(); }

bool Xl9555::SetDirection(uint32_t io_num, uint32_t direction)
{
    if (io_num > 15)
    {
        logger_.Error("Invalid pin number %u (max 15)", io_num);
        return false;
    }

    if (io_num < 8)
    {
        uint8_t mask = static_cast<uint8_t>(1u << io_num);
        if (direction == DIR_INPUT)
            config_p0_ |= mask;
        else
            config_p0_ &= ~mask;
        if (!device_.WriteReg8(REG_CONFIG_P0, config_p0_, -1))
        {
            logger_.Error("Failed to write CONFIG_P0");
            return false;
        }
    }
    else
    {
        uint8_t mask = static_cast<uint8_t>(1u << (io_num - 8));
        if (direction == DIR_INPUT)
            config_p1_ |= mask;
        else
            config_p1_ &= ~mask;
        if (!device_.WriteReg8(REG_CONFIG_P1, config_p1_, -1))
        {
            logger_.Error("Failed to write CONFIG_P1");
            return false;
        }
    }
    return true;
}

bool Xl9555::SetLevel(uint32_t io_num, uint32_t level)
{
    if (io_num > 15)
    {
        logger_.Error("Invalid pin number %u (max 15)", io_num);
        return false;
    }

    if (io_num < 8)
    {
        uint8_t mask = static_cast<uint8_t>(1u << io_num);
        if (level)
            output_p0_ |= mask;
        else
            output_p0_ &= ~mask;
        if (!device_.WriteReg8(REG_OUTPUT_P0, output_p0_, -1))
        {
            logger_.Error("Failed to write OUTPUT_P0");
            return false;
        }
    }
    else
    {
        uint8_t mask = static_cast<uint8_t>(1u << (io_num - 8));
        if (level)
            output_p1_ |= mask;
        else
            output_p1_ &= ~mask;
        if (!device_.WriteReg8(REG_OUTPUT_P1, output_p1_, -1))
        {
            logger_.Error("Failed to write OUTPUT_P1");
            return false;
        }
    }
    return true;
}

bool Xl9555::GetLevel(uint32_t io_num, uint32_t* level)
{
    if (io_num > 15 || level == nullptr)
    {
        logger_.Error("Invalid arguments");
        return false;
    }

    uint8_t reg = (io_num < 8) ? REG_INPUT_P0 : REG_INPUT_P1;
    uint8_t shift = (io_num < 8) ? static_cast<uint8_t>(io_num) : static_cast<uint8_t>(io_num - 8);

    uint8_t val = 0;
    if (!device_.ReadReg8(reg, val, -1))
    {
        logger_.Error("Failed to read input register");
        return false;
    }
    *level = (val >> shift) & 0x01;
    return true;
}

bool Xl9555::SetAllLevels(uint16_t value)
{
    output_p0_ = static_cast<uint8_t>(value & 0xFF);
    output_p1_ = static_cast<uint8_t>((value >> 8) & 0xFF);

    if (!device_.WriteReg8(REG_OUTPUT_P0, output_p0_, -1) ||
        !device_.WriteReg8(REG_OUTPUT_P1, output_p1_, -1))
    {
        logger_.Error("Failed to write output registers");
        return false;
    }
    return true;
}

bool Xl9555::SetAllDirections(uint16_t config)
{
    config_p0_ = static_cast<uint8_t>(config & 0xFF);
    config_p1_ = static_cast<uint8_t>((config >> 8) & 0xFF);

    if (!device_.WriteReg8(REG_CONFIG_P0, config_p0_, -1) ||
        !device_.WriteReg8(REG_CONFIG_P1, config_p1_, -1))
    {
        logger_.Error("Failed to write config registers");
        return false;
    }
    return true;
}

}  // namespace wrapper
