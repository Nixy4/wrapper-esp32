#include "aw9523.hpp"

namespace WrapperEsp32
{

Aw9523::Aw9523(Logger& logger) : I2cDevice(logger)
{
}

Aw9523::~Aw9523()
{
}

} // namespace WrapperEsp32
