/**
 * @file        AP_STM32Slave.h
 * @brief       SPI communication test driver.
 * @author      David Rodriguez Ferrero
 */

#pragma once

#include <AP_HAL/AP_HAL.h>

class AP_STM32Slave {
public:
    AP_STM32Slave();

    /* Do not allow copies */
    CLASS_NO_COPY(AP_STM32Slave);

    // Initialize the SPI device handle and set up periodic timer
    void init();

    // Manual update function (used if periodic callback is disabled)
    void update();

private:
    // Device handle returned by hal.spi->get_device()
    AP_HAL::OwnPtr<AP_HAL::SPIDevice> _dev;

    // Internal callback function invoked periodically by the SPI thread
    void _timer_tick();

    // Transmit and receive buffers
    uint8_t _tx_buf[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t _rx_buf[4] = {0};

    // Tracking state
    bool _initialised = false;
    uint32_t _last_transfer_ms = 0;
};