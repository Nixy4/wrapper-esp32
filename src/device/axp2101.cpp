#include "axp2101.hpp"

namespace WrapperEsp32
{

Axp2101::Axp2101(Logger& logger) : I2cDevice(logger)
{
}

Axp2101::~Axp2101()
{
}

} // namespace WrapperEsp32
