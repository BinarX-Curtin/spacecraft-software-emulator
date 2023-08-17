/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator header file
 *
 */

#pragma once
#include <stdint.h>

#include "abstraction_layer/gpio_interface.h"
#include "abstraction_layer/serial_communication_interface.h"

namespace binarx::emulator_liason {

/**> The time to wait for a communication transaction in milliseconds*/
constexpr uint32_t kDefaultCommunicationDelay = 10000;

/**
 * @brief Binar X Emulator to implement the Emulator functions
 *
 */
class EmulatorLiason {
 public:
  /**
   * @brief Construct a new Binar X Emulator object with dependency injection
   *
   * @param spi_communication Pointer to the SPI implementation object
   * @param uart_communication Pointer to the UART implementation object
   * @param gpio_object Pointer to the GPIO implementation object
   */
  EmulatorLiason(binarx_serial_interface::SerialCommunicationInterface&
                     emulator_communication,
                 binarx_gpio_interface::GpioInterface& gpio_object)
      : emulator_communication_(emulator_communication),
        gpio_controller_(gpio_object),
        payload_status_(PayloadDataStatus::kPayloadReady){};

  void Transmit(uint8_t* buffer, uint16_t size);

 private:
  /**< Reference to the payload comunication implementation*/
  binarx_serial_interface::SerialCommunicationInterface&
      emulator_communication_;
  /**< Reference to the GPIO implementation*/
  binarx_gpio_interface::GpioInterface& gpio_controller_;

 protected:
  /**
   * @brief Enum that identifies the possible states of the payload as observed
   * from the emulator
   *
   * Enum that identifies the possible states of the payload as observed
   * from the emulator. The enum is only accessed by the emulator but it is
   * protected instead of private so it can be accessed from the testing class
   *
   */
  enum class PayloadDataStatus {
    kPayloadReady,
    kNumberOfPacketsSent,
    kTrasferCompleted,
  };

  /**
   * @brief Holds the status of the payload and is used to determine the state
   * of the emulator
   *
   */
  PayloadDataStatus payload_status_;
};

}  // namespace binarx::emulator_liason