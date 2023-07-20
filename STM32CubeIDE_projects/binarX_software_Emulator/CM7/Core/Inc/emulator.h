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
  BinarXEmulator(binarx_serial_interface::SerialCommunicationInterface&
                     payload_communication,
                 binarx_serial_interface::SerialCommunicationInterface&
                     computer_communication,
                 binarx_gpio_interface::GpioInterface& gpio_object,
                 binarx_time_interface::TimeInterface& time_object)
      : payload_communication_(payload_communication),
        computer_communication_(computer_communication),
        gpio_controller_(gpio_object),
        time_controller_(time_object),
        button_pressed_(false),
        payload_status_(PayloadDataStatus::kWaitingForPayload){};

  /**
   * @brief Function that initialises the Emulator.
   *
   * @note Call this function once
   */
  void Init();

  /**
   * @brief Function to use to run the emulator code.
   *
   * @note call this fuction whithin the microcontroller while loop
   *
   */
  void Run();

  /**
   * @brief The function the holds what happens when the payload triggers the
   * ready interrupt
   *
   */
  void PayloadReadyInterruptCallback();

  /**
   * @brief The function to call when the User button is pressed
   *
   */
  void ButtonPressCallback();

 private:
  /**< Reference to the payload comunication implementation*/
  binarx_serial_interface::SerialCommunicationInterface& payload_communication_;
  /**< Reference to the computer comunication implementation*/
  binarx_serial_interface::SerialCommunicationInterface&
      computer_communication_;
  /**< Reference to the GPIO implementation*/
  binarx_gpio_interface::GpioInterface& gpio_controller_;
  /**< Reference to the time related implementation*/
  binarx_time_interface::TimeInterface& time_controller_;

  /**
   * @brief Function that provide information to the user when the emulator
   * starts running
   *
   */
  void RunStartInfo();

  /**
   * @brief Function that provide information to the user when the emulator
   * stops running
   *
   */
  void RunEndInfo();

  /**
   * @brief Emulator and Payload communication handler
   *
   */
  void PayloadCommunicationHandler();

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
    kWaitingForPayload,
    kPayloadReady,
    kTrasferCompleted,
  };

  /**
   * @brief The variable that holds the state if the button has been pressed or
   * not.
   *
   * The variable that holds the state if the button has been pressed or
   * not. This value ensures that a button press interrupt has no effet after
   * the first button pressed until the emulator has run succesfully.
   *
   *
   */
  bool button_pressed_;

  /**
   * @brief Holds the status of the payload and is used to determine the state
   * of the emulator
   *
   */
  PayloadDataStatus payload_status_;
};

}  // namespace binarx_emulator
