/**
 * @file spi_impl.h
 * @author Tristan Ward
 * @brief Header file for the SPI communication implementation
 *
 */

#include "abstraction_layer/serial_communication_interface.h"

#pragma once

namespace binarx_serial_impl {

class SpiImpl : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  SpiImpl(){};
  binarx_serial_interface::SerialStatus Transmit(uint8_t *buffer, uint16_t size,
                                                 uint32_t timeout);
  binarx_serial_interface::SerialStatus Receive(uint8_t *receive_buffer,
                                                uint16_t size,
                                                uint32_t timeout);
};

class UartImpl : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  UartImpl(){};
  binarx_serial_interface::SerialStatus Transmit(uint8_t *buffer, uint16_t size,
                                                 uint32_t timeout);
  binarx_serial_interface::SerialStatus Receive(uint8_t *receive_buffer,
                                                uint16_t size,
                                                uint32_t timeout);
};

}  // namespace binarx_serial_impl
