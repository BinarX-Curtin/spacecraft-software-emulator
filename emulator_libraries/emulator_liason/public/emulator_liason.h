/**
 * @file emulator_liason.h
 * @author Tristan Ward
 * @brief Binar X Emulator header file
 *
 */

#pragma once
#include <stdint.h>

#include "abstraction_layer/gpio/public/gpi_interface.h"
#include "abstraction_layer/gpio/public/gpo_interface.h"
#include "abstraction_layer/serial_communication_interface.h"
#include "emulator_definitions/emulator_definitions.h"

namespace binarx::emulator_liason {

using namespace binarx::emulator_definitions;

/**
 * @brief Binar X Emulator to implement the Emulator functions
 *
 */
class EmulatorLiason {
 public:
  /**
   * @brief Construct a new Emulator Liason object with dependency injection
   *
   * @param emulator_communication Reference to the serial comunication
   * implementation
   * @param gpo_payload_ready Reference to the GPO implementation
   */
  EmulatorLiason(binarx_serial_interface::SerialCommunicationInterface&
                     emulator_communication,
                 bsf::hal::gpio::GpoInterface& gpo_payload_ready)
      : emulator_communication_(emulator_communication),
        gpo_payload_ready_(gpo_payload_ready),
        bytes_to_send(0),
        payload_status_(PayloadDataStatus::kCapturingData){};

  /**
   * @brief Transmit data to the Emulator
   *
   * @param buffer pointer to the C style array with the data to send
   * @param size of the data to send
   */
  void Transmit(uint8_t* buffer, uint16_t size);

  /**
   * @brief ChipSelectInterrupt The function to run when the Chip select
   * interrupt pin is treggered
   *
   */
  void ChipSelectInterrupt();

  /**
   * @brief TransmitCallBackInterrupt The function to call when the Serial
   * Transmision has completed
   *
   */
  void TransmitCallBackInterrupt();

 private:
  /**< Reference to the payload comunication implementation*/
  binarx_serial_interface::SerialCommunicationInterface&
      emulator_communication_;
  /**< Reference to the GPIO implementation*/
  bsf::hal::gpio::GpoInterface& gpo_payload_ready_;
  /**< Buffer to store the data to send*/
  std::array<uint8_t, kMaxPayloadDataLength + kNumberOfBytesInHeader>
      buffer_data;
  /**< Size of the data plus any bytes extra to fill all required packets that
   * must be sent to the emulator */
  uint16_t bytes_to_send;

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
    kCapturingData,
    kPayloadReadyToTransmit,
    kChipSelectTriggered,
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