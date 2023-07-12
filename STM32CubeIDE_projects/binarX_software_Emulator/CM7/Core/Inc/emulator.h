/**
 * @file emulator.h
 * @author Tristan Ward
 * @brief Binar X Emulator header file
 *
 */

#pragma once
#include <stdint.h>

#include "gpio_interface.h"
#include "serial_communication_interface.h"
#include "time_interface.h"

namespace binarx_emulator {

/**> Max payload data size allowed */
constexpr uint16_t kMaxPayloadDataLength = 200;
/**> The time the Payload is allowed to be on for in milliseconds*/
constexpr uint32_t kWaitForPayloadMaxTime = 60 * 1000;
/**> The time to wait for a communication transaction*/
constexpr uint32_t kDefaultCommunicationDelay = kWaitForPayloadMaxTime;


enum class PayloadDataStatus {
  kWaitingForPayload,
  kPayloadReady,
  kTrasferCompleted,
};

/**
 * @brief Binar X Emulator to implement the Emulator functions
 *
 */
class BinarXEmulator {
 public:
  /**
   * @brief Construct a new Binar X Emulator object with dependency injection
   *
   * @param spi_communication Pointer to the SPI implementation object
   * @param uart_communication Pointer to the UART implementation object
   * @param gpio_object Pointer to the GPIO implementation object
   */
  BinarXEmulator(binarx_serial_interface::SerialCommunicationInterface*
                     payload_communication,
                 binarx_serial_interface::SerialCommunicationInterface*
                     computer_communication,
                 binarx_gpio_interface::GpioInterface* gpio_object,
                 binarx_time_interface::TimeInterface* time_object)
      : payload_communication_(payload_communication),
        computer_communication_(computer_communication),
        gpio_controller_(gpio_object),
        time_controller_(time_object),
        button_pressed_(false),
        payload_status_(PayloadDataStatus::kWaitingForPayload){};

  /**
   * @brief Function that holds what happens when the Emulator is first started
   */
  void Init();

  /**
   * @brief Function to use when payload data is requested by the students
   *
   */
  void Run();

  void PayloadCommunicationCallback();
  void ButtonPressCallback();

 private:
  binarx_serial_interface::SerialCommunicationInterface* payload_communication_;
  binarx_serial_interface::SerialCommunicationInterface*
      computer_communication_;
  binarx_gpio_interface::GpioInterface* gpio_controller_;
  binarx_time_interface::TimeInterface* time_controller_;

  /**
   * @brief Function that provide information to the user when the emulator
   * starts
   *
   */
  void RunStartInfo();

  /**
   * @brief Function that provide information to the user when the emulator
   * stops
   *
   */
  void RunEndInfo();

  /**
   * @brief Function that communicates with the payload
   *
   */
  void PayloadCommunicationHandler();

  void ButtonPressHandler();

 protected:
  bool button_pressed_;
  PayloadDataStatus payload_status_;
};

}  // namespace binarx_emulator
