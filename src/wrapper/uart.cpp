#include "wrapper/uart.hpp"

using namespace wrapper;

UartPort::UartPort(Logger &logger) : logger_(logger), port_(UART_NUM_0), installed_(false), event_queue_(nullptr)
{
}

UartPort::~UartPort()
{
    Deinit();
}

Logger &UartPort::GetLogger()
{
    return logger_;
}

uart_port_t UartPort::GetPort() const
{
    return port_;
}

QueueHandle_t UartPort::GetEventQueue() const
{
    return event_queue_;
}

bool UartPort::Init(uart_port_t port,
                    const UartConfig &config,
                    int tx_pin,
                    int rx_pin,
                    int rts_pin,
                    int cts_pin,
                    int rx_buffer_size,
                    int tx_buffer_size,
                    int event_queue_size)
{
    if (installed_) {
        logger_.Warning("Already initialized. Deinitializing first.");
        Deinit();
    }

    port_ = port;

    // Step 1: Configure UART parameters
    esp_err_t ret = uart_param_config(port_, &config);
    if (ret != ESP_OK) {
        logger_.Error("Failed to configure parameters: %s", esp_err_to_name(ret));
        return false;
    }

    // Step 2: Set UART pins
    ret = uart_set_pin(port_, tx_pin, rx_pin, rts_pin, cts_pin);
    if (ret != ESP_OK) {
        logger_.Error("Failed to set pins: %s", esp_err_to_name(ret));
        return false;
    }

    // Step 3: Install UART driver
    QueueHandle_t *queue_ptr = (event_queue_size > 0) ? &event_queue_ : nullptr;
    ret = uart_driver_install(port_, rx_buffer_size, tx_buffer_size, event_queue_size, queue_ptr, 0);
    if (ret != ESP_OK) {
        logger_.Error("Failed to install driver: %s", esp_err_to_name(ret));
        return false;
    }

    installed_ = true;
    logger_.Info("Initialized (Port: %d, TX: %d, RX: %d, Baud: %d)",
                 port_, tx_pin, rx_pin, config.baud_rate);
    return true;
}

bool UartPort::Deinit()
{
    if (!installed_) {
        return true;
    }

    esp_err_t ret = uart_driver_delete(port_);
    if (ret != ESP_OK) {
        logger_.Error("Failed to delete driver: %s", esp_err_to_name(ret));
        return false;
    }

    installed_ = false;
    event_queue_ = nullptr;
    logger_.Info("Deinitialized (Port: %d)", port_);
    return true;
}

int UartPort::Write(const uint8_t *data, size_t len)
{
    return uart_write_bytes(port_, data, len);
}

int UartPort::Write(const std::vector<uint8_t> &data)
{
    return uart_write_bytes(port_, data.data(), data.size());
}

int UartPort::Read(uint8_t *buf, size_t len, int timeout_ms)
{
    return uart_read_bytes(port_, buf, len, pdMS_TO_TICKS(timeout_ms));
}

int UartPort::Read(std::vector<uint8_t> &buf, size_t len, int timeout_ms)
{
    buf.resize(len);
    int read = uart_read_bytes(port_, buf.data(), len, pdMS_TO_TICKS(timeout_ms));
    if (read >= 0) {
        buf.resize(read);
    } else {
        buf.clear();
    }
    return read;
}

int UartPort::ReadAvailable(std::vector<uint8_t> &buf, int timeout_ms)
{
    size_t available = 0;
    esp_err_t ret = uart_get_buffered_data_len(port_, &available);
    if (ret != ESP_OK) {
        logger_.Error("ReadAvailable: failed to get buffer length: %s", esp_err_to_name(ret));
        return -1;
    }
    if (available == 0) {
        // 等待首个字节到达
        buf.resize(1);
        int n = uart_read_bytes(port_, buf.data(), 1, pdMS_TO_TICKS(timeout_ms));
        if (n <= 0) {
            buf.clear();
            return n;
        }
        // 再次查询剩余数据
        uart_get_buffered_data_len(port_, &available);
        if (available > 0) {
            buf.resize(1 + available);
            uart_read_bytes(port_, buf.data() + 1, available, 0);
        }
        return static_cast<int>(buf.size());
    }
    buf.resize(available);
    int n = uart_read_bytes(port_, buf.data(), available, pdMS_TO_TICKS(timeout_ms));
    if (n >= 0) {
        buf.resize(n);
    } else {
        buf.clear();
    }
    return n;
}

int UartPort::ReadByte(uint8_t &data, int timeout_ms)
{
    return uart_read_bytes(port_, &data, 1, pdMS_TO_TICKS(timeout_ms));
}

bool UartPort::ReadLine(std::string &line, char delimiter, int timeout_ms)
{
    line.clear();
    uint8_t ch;
    while (true) {
        int n = uart_read_bytes(port_, &ch, 1, pdMS_TO_TICKS(timeout_ms));
        if (n <= 0) {
            return false;
        }
        if (ch == static_cast<uint8_t>(delimiter)) {
            return true;
        }
        line.push_back(static_cast<char>(ch));
    }
}

bool UartPort::Flush()
{
    esp_err_t ret = uart_flush(port_);
    if (ret != ESP_OK) {
        logger_.Error("Failed to flush: %s", esp_err_to_name(ret));
        return false;
    }
    return true;
}

bool UartPort::FlushInput()
{
    esp_err_t ret = uart_flush_input(port_);
    if (ret != ESP_OK) {
        logger_.Error("Failed to flush input: %s", esp_err_to_name(ret));
        return false;
    }
    return true;
}

bool UartPort::WaitTxDone(int timeout_ms)
{
    esp_err_t ret = uart_wait_tx_done(port_, pdMS_TO_TICKS(timeout_ms));
    if (ret != ESP_OK) {
        logger_.Error("Failed to wait TX done: %s", esp_err_to_name(ret));
        return false;
    }
    return true;
}

int UartPort::GetBufferedDataLen()
{
    size_t len = 0;
    esp_err_t ret = uart_get_buffered_data_len(port_, &len);
    if (ret != ESP_OK) {
        logger_.Error("Failed to get buffered data length: %s", esp_err_to_name(ret));
        return -1;
    }
    return static_cast<int>(len);
}

bool UartPort::SetBaudrate(uint32_t baudrate)
{
    esp_err_t ret = uart_set_baudrate(port_, baudrate);
    if (ret != ESP_OK) {
        logger_.Error("Failed to set baudrate: %s", esp_err_to_name(ret));
        return false;
    }
    logger_.Info("Baudrate set to %lu", baudrate);
    return true;
}

bool UartPort::GetBaudrate(uint32_t &baudrate)
{
    esp_err_t ret = uart_get_baudrate(port_, &baudrate);
    if (ret != ESP_OK) {
        logger_.Error("Failed to get baudrate: %s", esp_err_to_name(ret));
        return false;
    }
    return true;
}
