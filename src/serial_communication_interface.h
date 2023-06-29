/**
 * @file serial_comunication_interface.h
 * @author Tristan Ward
 * @brief Serial Comunication interface heather file to create selectable
 * dependencies
 *
 */

#pragma once
#include <stdint.h>
namespace binarx_serial_interface {

class SerialCommunicationInterface {
 public:
  virtual void Transmit(uint8_t *buffer) = 0;
  virtual uint8_t Receive(uint8_t *receive_buffer) = 0;
  virtual uint8_t TransmitReceive(uint8_t *transmit_buffer,
                                  uint8_t *receive_buffer) = 0;
};
}  // namespace binarx_serial_interface