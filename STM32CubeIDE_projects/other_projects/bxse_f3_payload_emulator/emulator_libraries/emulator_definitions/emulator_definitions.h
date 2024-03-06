/**
 * @file emulator_definitions.h
 * @author Tristan
 * @brief A file that stores required values for the emulator libraries
 * @version 0.1
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
namespace binarx::emulator_definitions {
/**> Max payload data size allowed */
constexpr uint16_t kMaxPayloadDataLength = 1000;
/**> Packet Length*/
constexpr uint16_t kPacketLength = 250;
/**> Synck byte value used to determine if The SPI transmision failed*/
constexpr uint8_t kSyncByte = 5;
/**< the number of bytes in the payload header*/
constexpr uint8_t kNumberOfBytesInHeader = 3;

}  // namespace binarx::emulator_definitions