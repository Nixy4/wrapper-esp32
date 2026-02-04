#include <string>
#include <vector>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "wrapper/logger.hpp"
#include "wrapper/soc.hpp"
#include "wrapper/i2c.hpp"
#include "wrapper/wifi.hpp"
#include "board/m5stack/m5stack_core_s3.hpp"

using namespace wrapper;

M5StackCoreS3 &m5 = M5StackCoreS3::GetInstance();

static void board_init(void *arg)
{
  m5.InitBus(true, true, true);
  m5.InitDevice(true, true, true, true);
  m5.InitMiddleware(true);
  m5.GetLvglPort().Test();
  vTaskDelete(nullptr);
}

extern "C" void app_main()
{
  //nvs init
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  xTaskCreate(board_init, "board_init", 8192, nullptr, 5, nullptr);
};
