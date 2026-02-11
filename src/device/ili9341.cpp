#include "ili9341.hpp"

namespace WrapperEsp32 {

bool Ili9341::Init(const SpiDisplayConfig& config) {
    return SpiDisplay::Init(config, esp_lcd_new_panel_ili9341);
}

} // namespace WrapperEsp32
