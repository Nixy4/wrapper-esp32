#include "wrapper/display-dsi.hpp"

namespace wrapper
{

// --- DsiBus ---

DsiBus::DsiBus(Logger& logger) : m_logger(logger), m_bus_handle(nullptr) {}

DsiBus::~DsiBus() {
  Deinit();
}

Logger& DsiBus::GetLogger() {
  return m_logger;
}

esp_lcd_dsi_bus_handle_t DsiBus::GetHandle() const {
  return m_bus_handle;
}

esp_err_t DsiBus::Init(const DsiBusConfig& config) {
  if (m_bus_handle != nullptr) {
    m_logger.Warning("Already initialized. Deinitializing first.");
    Deinit();
  }

  esp_err_t ret = esp_lcd_new_dsi_bus(&config, &m_bus_handle);
  if (ret == ESP_OK) {
    m_logger.Info("Initialized (Bus ID: %d, Lanes: %d, Rate: %lu Mbps)", 
                  config.bus_id, config.num_data_lanes, config.lane_bit_rate_mbps);
  } else {
    m_logger.Error("Failed to initialize: %s", esp_err_to_name(ret));
  }
  return ret;
}

esp_err_t DsiBus::Deinit() {
  if (m_bus_handle != nullptr) {
    esp_err_t ret = esp_lcd_del_dsi_bus(m_bus_handle);
    if (ret == ESP_OK) {
      m_logger.Info("Deinitialized");
      m_bus_handle = nullptr;
    } else {
      m_logger.Error("Failed to deinitialize: %s", esp_err_to_name(ret));
    }
    return ret;
  }
  return ESP_OK;
}

// --- DsiDisplay ---

DsiDisplay::DsiDisplay(Logger& logger) 
  : DisplayBase(nullptr, nullptr), m_logger(logger) {}

DsiDisplay::~DsiDisplay() {
  Deinit();
}

esp_err_t DsiDisplay::Init(
  const DsiBus& bus,
  const DsiDisplayConfig& config,
  std::function<esp_err_t(const esp_lcd_panel_io_handle_t, const esp_lcd_panel_dev_config_t*, esp_lcd_panel_handle_t*)> new_panel_func,
  std::function<esp_err_t(const esp_lcd_panel_io_handle_t)> custom_init_panel_func,
  void* vendor_config,
  std::function<void(void)> vendor_config_init_func)
{
  // 1. Initialize Panel IO (DBI)
  esp_err_t ret = InitIo(bus, config.dbi_config);
  if (ret != ESP_OK) return ret;

  // 2. Execute vendor config initialization callback if provided
  if (vendor_config_init_func) {
    vendor_config_init_func();
  }

  // 3. Create Panel handle with vendor_config
  esp_lcd_panel_dev_config_t panel_config = config.panel_config;
  panel_config.vendor_config = vendor_config;
  
  ret = new_panel_func(m_io_handle, &panel_config, &m_panel_handle);
  if (ret != ESP_OK) {
    m_logger.Error("Failed to create LCD panel handle: %s", esp_err_to_name(ret));
    return ret;
  }

  // 4. Initialize Panel (reset, init, turn on)
  return InitPanel(panel_config, custom_init_panel_func, vendor_config, vendor_config_init_func);
}

esp_err_t DsiDisplay::InitIo(const DsiBus& bus, const esp_lcd_dbi_io_config_t& config)
{
  if (m_io_handle != nullptr) {
    m_logger.Warning("Display IO already initialized. Deinitializing first.");
    Deinit();
  }

  if (bus.GetHandle() == nullptr) {
    m_logger.Error("DSI bus not initialized");
    return ESP_ERR_INVALID_STATE;
  }

  // Create DBI IO handle
  esp_err_t ret = esp_lcd_new_panel_io_dbi(bus.GetHandle(), &config, &m_io_handle);
  if (ret != ESP_OK) {
    m_logger.Error("Failed to create LCD DBI IO handle: %s", esp_err_to_name(ret));
    return ret;
  }

  m_logger.Info("LCD DBI IO initialized (VC: %d)", config.virtual_channel);
  return ESP_OK;
}

esp_err_t DsiDisplay::InitPanel(
  const esp_lcd_panel_dev_config_t& panel_config, 
  std::function<esp_err_t(const esp_lcd_panel_io_handle_t)> custom_init_panel_func,
  void* vendor_config,
  std::function<void(void)> vendor_config_init_func)
{
  if (m_io_handle == nullptr) {
    m_logger.Error("IO handle not initialized. Call Init first.");
    return ESP_ERR_INVALID_STATE;
  }

  if (m_panel_handle == nullptr) {
    m_logger.Error("Panel handle not created. Create it before calling InitPanel.");
    return ESP_ERR_INVALID_STATE;
  }

  // 1. Reset the display
  m_logger.Info("Resetting panel...");
  esp_err_t ret = esp_lcd_panel_reset(m_panel_handle);
  if (ret != ESP_OK) {
    m_logger.Error("Panel Reset Failed: %s", esp_err_to_name(ret));
    return ret;
  }

  // 2. Initialize the display
  m_logger.Info("Initializing panel...");
  if (custom_init_panel_func == nullptr) {
    ret = esp_lcd_panel_init(m_panel_handle);
  } else {
    ret = custom_init_panel_func(m_io_handle);
  }

  if (ret != ESP_OK) {
    m_logger.Error("Panel Init Failed: %s", esp_err_to_name(ret));
    return ret;
  }

  // 3. Set invert color (optional, typically false for most panels)
  m_logger.Info("Setting invert color...");
  ret = esp_lcd_panel_invert_color(m_panel_handle, false);
  if (ret != ESP_OK) {
    m_logger.Error("Failed to set invert color: %s", esp_err_to_name(ret));
    return ret;
  }

  m_logger.Info("Display initialization complete!");
  return ESP_OK;
}

esp_err_t DsiDisplay::Deinit()
{
  esp_err_t ret = ESP_OK;
  
  if (m_panel_handle != nullptr) {
    esp_err_t r = esp_lcd_panel_del(m_panel_handle);
    if (r != ESP_OK) {
      m_logger.Error("Failed to delete panel handle: %s", esp_err_to_name(r));
      ret = r;
    }
    m_panel_handle = nullptr;
  }
  
  if (m_io_handle != nullptr) {
    esp_err_t r = esp_lcd_panel_io_del(m_io_handle);
    if (r != ESP_OK) {
      m_logger.Error("Failed to delete IO handle: %s", esp_err_to_name(r));
      if (ret == ESP_OK) ret = r;
    }
    m_io_handle = nullptr;
  }
  
  if (ret == ESP_OK) {
    m_logger.Info("DSI Display deinitialized");
  }
  
  return ret;
}

} // namespace wrapper
