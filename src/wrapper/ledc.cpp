#include "wrapper/ledc.hpp"

namespace wrapper
{

// --- LedcTimer ---

LedcTimer::LedcTimer(Logger &logger)
    : m_logger(logger), m_speed_mode(LEDC_LOW_SPEED_MODE), m_timer_num(LEDC_TIMER_0), m_initialized(false)
{
}

LedcTimer::~LedcTimer()
{
    Deinit();
}

esp_err_t LedcTimer::Init(const LedcTimerConfig &config)
{
    if (m_initialized)
    {
        m_logger.Warning("LEDC Timer already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_timer_config(&config);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to configure LEDC timer: %s", esp_err_to_name(ret));
        return ret;
    }

    m_speed_mode = config.speed_mode;
    m_timer_num = config.timer_num;
    m_initialized = true;

    m_logger.Info("LEDC Timer initialized (mode: %d, timer: %d, freq: %lu Hz)",
                  m_speed_mode, m_timer_num, config.freq_hz);
    return ESP_OK;
}

esp_err_t LedcTimer::Deinit()
{
    if (!m_initialized)
    {
        return ESP_OK;
    }

    // Use deconfigure flag to deinit timer
    ledc_timer_config_t deinit_config = {};
    deinit_config.speed_mode = m_speed_mode;
    deinit_config.timer_num = m_timer_num;
    deinit_config.deconfigure = true;

    esp_err_t ret = ledc_timer_config(&deinit_config);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to deinitialize LEDC timer: %s", esp_err_to_name(ret));
        return ret;
    }

    m_initialized = false;
    m_logger.Info("LEDC Timer deinitialized");
    return ESP_OK;
}

esp_err_t LedcTimer::Pause()
{
    if (!m_initialized)
    {
        m_logger.Error("LEDC Timer not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_timer_pause(m_speed_mode, m_timer_num);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to pause LEDC timer: %s", esp_err_to_name(ret));
        return ret;
    }

    m_logger.Debug("LEDC Timer paused");
    return ESP_OK;
}

esp_err_t LedcTimer::Resume()
{
    if (!m_initialized)
    {
        m_logger.Error("LEDC Timer not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_timer_resume(m_speed_mode, m_timer_num);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to resume LEDC timer: %s", esp_err_to_name(ret));
        return ret;
    }

    m_logger.Debug("LEDC Timer resumed");
    return ESP_OK;
}

esp_err_t LedcTimer::SetFreq(uint32_t freq_hz)
{
    if (!m_initialized)
    {
        m_logger.Error("LEDC Timer not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_set_freq(m_speed_mode, m_timer_num, freq_hz);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to set LEDC timer frequency to %lu Hz: %s", freq_hz, esp_err_to_name(ret));
        return ret;
    }

    m_logger.Debug("LEDC Timer frequency set to %lu Hz", freq_hz);
    return ESP_OK;
}

// --- LedcChannel ---

LedcChannel::LedcChannel(Logger &logger)
    : m_logger(logger), m_speed_mode(LEDC_LOW_SPEED_MODE), m_channel(LEDC_CHANNEL_0), m_initialized(false)
{
}

LedcChannel::~LedcChannel()
{
    Deinit();
}

esp_err_t LedcChannel::Init(const LedcChannelConfig &config)
{
    if (m_initialized)
    {
        m_logger.Warning("LEDC Channel already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_channel_config(&config);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to configure LEDC channel: %s", esp_err_to_name(ret));
        return ret;
    }

    m_speed_mode = config.speed_mode;
    m_channel = config.channel;
    m_initialized = true;

    m_logger.Info("LEDC Channel initialized (mode: %d, channel: %d, gpio: %d)",
                  m_speed_mode, m_channel, config.gpio_num);
    return ESP_OK;
}

esp_err_t LedcChannel::Deinit()
{
    if (!m_initialized)
    {
        return ESP_OK;
    }

    // Stop the channel before deinit
    esp_err_t ret = Stop(0);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to stop LEDC channel during deinit: %s", esp_err_to_name(ret));
        return ret;
    }

    m_initialized = false;
    m_logger.Info("LEDC Channel deinitialized");
    return ESP_OK;
}

esp_err_t LedcChannel::SetDuty(uint32_t duty)
{
    if (!m_initialized)
    {
        m_logger.Error("LEDC Channel not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_set_duty(m_speed_mode, m_channel, duty);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to set LEDC duty to %lu: %s", duty, esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}

esp_err_t LedcChannel::SetDutyAndUpdate(uint32_t duty)
{
    esp_err_t ret = SetDuty(duty);
    if (ret != ESP_OK)
    {
        return ret;
    }

    return UpdateDuty();
}

esp_err_t LedcChannel::UpdateDuty()
{
    if (!m_initialized)
    {
        m_logger.Error("LEDC Channel not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_update_duty(m_speed_mode, m_channel);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to update LEDC duty: %s", esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}

esp_err_t LedcChannel::Stop(uint32_t idle_level)
{
    if (!m_initialized)
    {
        m_logger.Error("LEDC Channel not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = ledc_stop(m_speed_mode, m_channel, idle_level);
    if (ret != ESP_OK)
    {
        m_logger.Error("Failed to stop LEDC channel: %s", esp_err_to_name(ret));
        return ret;
    }

    m_logger.Debug("LEDC Channel stopped");
    return ESP_OK;
}

} // namespace wrapper
