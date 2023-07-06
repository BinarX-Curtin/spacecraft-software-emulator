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

/**
 * @brief The Serial status is the possible return states from a Serial
 * Comunication
 *
 */
enum SerialStatus {
  Success,
  Error,
  Busy,
  Timeout,
};

/**
 * @brief The Serial Comunication interface to allow for dependency injection
 * and testing
 *
 */
class SerialCommunicationInterface {
 public:
  /**
   * @brief Transmit the data through serial
   *
   * @param buffer A pointer to the start of the buffer
   * @param size the size of the data
   * @param timeout The time the Serial will wait for the transaction to start
   * @return SerialStatus The status of the transaction
   */
  virtual SerialStatus Transmit(uint8_t *buffer, uint16_t size,
                                uint32_t timeout) = 0;
  /**
   * @brief Receive the data through serial
   *
   * @param receive_buffer A pointer to the start of the receiving buffer
   * @param size the size of the data to receive
   * @param timeout The time the Serial will wait for the transaction to start
   * @return SerialStatus The status of the transaction
   */
  virtual SerialStatus Receive(uint8_t *receive_buffer, uint16_t size,
                               uint32_t timeout) = 0;
};
}  // namespace binarx_serial_interface