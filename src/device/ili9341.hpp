#pragma once
#include "wrapper/display.hpp"
#include "esp_lcd_ili9341.h"

namespace WrapperEsp32 {

class Ili9341 : public SpiDisplay {
public:
    using SpiDisplay::SpiDisplay; // Inherit constructor

    bool Init(const SpiDisplayConfig& config);
};

} // namespace WrapperEsp32
