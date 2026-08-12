#include "AP_STM32Slave.h"

extern const AP_HAL::HAL& hal;

AP_STM32Slave::AP_STM32Slave() {}

void AP_STM32Slave::init() {
    // "stm32_slave" must match the label defined in hwdef.dat
    _dev = hal.spi->get_device("stm32_slave");
    if (!_dev) {
        hal.console->printf("AP_STM32Slave: Device 'stm32_slave' not found\n");
        return;
    }

    // Set bus speed (SPEED_LOW is typically ~1-2 MHz)
    _dev->set_speed(AP_HAL::Device::SPEED_LOW);

    // Register a 100 Hz periodic callback (10,000 microseconds = 10 ms)
    // SPIDevice executes this on a dedicated bus thread
    _dev->register_periodic_callback(10000, FUNCTOR_BIND_MEMBER(&AP_STM32Slave::_timer_tick, void));

    _initialised = true;
    hal.console->printf("AP_STM32Slave: Initialized successfully\n");
}

void AP_STM32Slave::_timer_tick() {
    if (!_dev) {
        return;
    }

    // Full-duplex transfer
    // Note: SPIDevice automatically manages the bus semaphore inside register_periodic_callback
    if (_dev->transfer(_tx_buf, sizeof(_tx_buf), _rx_buf, sizeof(_rx_buf))) {
        _last_transfer_ms = AP_HAL::millis();
    }
}

void AP_STM32Slave::update() {
    if (!_initialised) {
        return;
    }

    // Print received data every second for debugging
    static uint32_t last_print_ms = 0;
    uint32_t now = AP_HAL::millis();

    if (now - last_print_ms >= 1000) {
        last_print_ms = now;
        hal.console->printf("RX: 0x%02X 0x%02X 0x%02X 0x%02X (Last transfer: %u ms ago)\n",
                            _rx_buf[0], _rx_buf[1], _rx_buf[2], _rx_buf[3],
                            (unsigned)(now - _last_transfer_ms));
    }
}