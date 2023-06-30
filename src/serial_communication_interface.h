/**
 * @file serial_comunication_interface.h
 * @author Tristan Ward
 * @brief Serial Comunication interface heather file to create selectable
 * dependencies
 *
 */

#pragma once
#include <stdint.h>

#include <array>
namespace binarx_serial_interface {

enum SerialStatus {
  Success,
  Error,
  Busy,
  Timeout,
};

class SerialCommunicationInterface {
 public:
  virtual SerialStatus Transmit(uint8_t *buffer, uint16_t size,
                                uint32_t timeout) = 0;
  virtual SerialStatus Receive(uint8_t *receive_buffer, uint16_t size,
                               uint32_t timeout) = 0;
  //   virtual SerialStatus TransmitReceive(
  //       std::array<uint8_t, kMaxPayloadDataLength> &transmit_buffer,
  //       std::array<uint8_t, kMaxPayloadDataLength> &receive_buffer,
  //       uint32_t timeout) = 0;
};
}  // namespace binarx_serial_interface