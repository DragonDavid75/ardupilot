#include <AP_HAL/AP_HAL.h>

extern const AP_HAL::HAL& hal;

class AP_STM32Slave {
public:
    void init() {
        // Retrieve hardware device handle defined in hwdef.dat
        _dev = hal.spi->get_device("stm32_slave");
        if (!_dev) {
            hal.console->printf("Failed to find stm32_slave SPI device\n");
            return;
        }

        // Set bus clock frequency (e.g., 2 MHz)
        _dev->set_speed(AP_HAL::Device::SPEED_LOW);
    }

    void transfer_data() {
        if (!_dev) return;

        uint8_t tx_buf[4] = {0x01, 0x02, 0x03, 0x04};
        uint8_t rx_buf[4] = {0};

        // Obtain bus semaphore and execute full-duplex transfer
        WITH_SEMAPHORE(_dev->get_semaphore());
        
        // Lowers CS line, transfers bytes, and raises CS line
        _dev->transfer(tx_buf, sizeof(tx_buf), rx_buf, sizeof(rx_buf));

        hal.console->printf("Received: 0x%02X 0x%02X 0x%02X 0x%02X\n",
                            rx_buf[0], rx_buf[1], rx_buf[2], rx_buf[3]);
    }

private:
    AP_HAL::OwnPtr<AP_HAL::SPIDevice> _dev;
};