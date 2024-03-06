/**
 * @file emulator_test.cc
 * @author Tristan Ward
 * @brief The Unit tests for the emulator library
 *
 */

#include "emulator/public/emulator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sys/time.h>

#include "abstraction_layer/gpio/public/gpo_interface.h"
#include "abstraction_layer/serial_communication_interface.h"
#include "abstraction_layer/time_interface.h"
#include "emulator_definitions/emulator_definitions.h"

using namespace ::testing;

using namespace binarx::emulator_definitions;

class SerialCommunicationMock
    : public binarx_serial_interface::SerialCommunicationInterface {
 public:
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Transmit,
              (uint8_t * buffer, uint16_t size, uint32_t timeout), (override));
  MOCK_METHOD(binarx_serial_interface::SerialStatus, Receive,
              (uint8_t * receive_buffer, uint16_t size, uint32_t timeout),
              (override));
  MOCK_METHOD(binarx_serial_interface::SerialStatus, TransmitIt,
              (uint8_t * buffer, uint16_t size), (override));
};

class GpoMock : public bsf::hal::gpio::GpoInterface {
 public:
  MOCK_METHOD(void, SetHigh, (), (override));
  MOCK_METHOD(void, SetLow, (), (override));
  MOCK_METHOD(void, Toggle, (), (override));
};
class TimeMock : public binarx_time_interface::TimeInterface {
 public:
  MOCK_METHOD(void, Delay, (uint32_t milliseconds), (override));
  MOCK_METHOD(uint32_t, GetTicks, (), (override));
};

MATCHER_P2(ArraysAreEqual, array, size,
           "elements are equal [%(array)s, %(size)s]") {
  for (uint16_t i = 0; i < size; i++) {
    if (arg[i] != array[i]) {
      return false;
    }
  }
  return true;
}

class EmulatorMockTesting : public binarx::emulator::BinarXEmulator {
 public:
  EmulatorMockTesting(binarx_serial_interface::SerialCommunicationInterface&
                          payload_communication,
                      binarx_serial_interface::SerialCommunicationInterface&
                          computer_communication,
                      bsf::hal::gpio::GpoInterface& gpo_red_led,
                      bsf::hal::gpio::GpoInterface& gpo_yellow_led,
                      bsf::hal::gpio::GpoInterface& gpo_green_led,
                      bsf::hal::gpio::GpoInterface& gpo_payload_switch,
                      bsf::hal::gpio::GpoInterface& gpo_payload_chip_select,
                      binarx_time_interface::TimeInterface& time_object)
      : binarx::emulator::BinarXEmulator(
            payload_communication, computer_communication, gpo_red_led,
            gpo_yellow_led, gpo_green_led, gpo_payload_switch,
            gpo_payload_chip_select, time_object){};

  void SetPayloadStatus_TestOnly(PayloadDataStatus value) {
    payload_status_ = value;
  }
  void SetButtonPressed_TestOnly(bool value) { button_pressed_ = value; }

  using binarx::emulator::BinarXEmulator::PayloadDataStatus;
};

class EmulatorTest : public testing::Test {
 protected:
  // void SetUp() override{};
  NiceMock<SerialCommunicationMock> payload_com_mock;
  NiceMock<SerialCommunicationMock> computer_com_mock;
  NiceMock<GpoMock> gpo_red_led_mock;
  NiceMock<GpoMock> gpo_yellow_led_mock;
  NiceMock<GpoMock> gpo_green_led_mock;
  NiceMock<GpoMock> gpo_payload_switch;
  NiceMock<GpoMock> gpo_payload_chip_select;
  NiceMock<TimeMock> time_mock;

  EmulatorMockTesting emulator = EmulatorMockTesting(
      payload_com_mock, computer_com_mock, gpo_red_led_mock,
      gpo_yellow_led_mock, gpo_green_led_mock, gpo_payload_switch,
      gpo_payload_chip_select, time_mock);
};

uint16_t CalculateNumberOfPackets(uint16_t data_size) {
  // Calculate number of packets
  uint16_t num_packets = data_size / kPacketLength;
  // Add an extra packets if the data above has a reminder
  if (data_size % kPacketLength != 0) {
    num_packets++;
  }
  return num_packets;
}

TEST_F(EmulatorTest, Run_DataTransferSuccess) {
  // Given a buffer with data that is sent succesfully from the payload
  uint8_t kNumOfPaquets = 1;
  uint8_t kDataSize = kNumOfPaquets * kPacketLength;
  uint8_t data_buffer[kDataSize];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = static_cast<uint8_t>(i);
  };

  // The emulator should receive the number of bytes that the payload has to
  // send
  uint8_t metadata_packet[kNumberOfBytesInHeader] = {
      kSyncByte, static_cast<uint8_t>(kDataSize >> 8),
      static_cast<uint8_t>(kDataSize)};

  EXPECT_CALL(payload_com_mock, Receive(_, kNumberOfBytesInHeader, _))
      .WillOnce(
          DoAll(SetArrayArgument<0>(metadata_packet,
                                    metadata_packet + sizeof(metadata_packet)),
                Return(binarx_serial_interface::SerialStatus::Success)));

  // Then the data received by the emulator from the payload should be equal to
  // the data sent to by the emulator to the Users Computer
  EXPECT_CALL(payload_com_mock, Receive(_, kPacketLength, _))
      .WillOnce(
          DoAll(SetArrayArgument<0>(data_buffer, data_buffer + kPacketLength),
                Return(binarx_serial_interface::SerialStatus::Success)));

  // There can be any number of transmisions to the computer.
  EXPECT_CALL(computer_com_mock, Transmit(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  // But at least one must match the data receive from the payload
  EXPECT_CALL(computer_com_mock,
              Transmit(ArraysAreEqual(data_buffer, sizeof(data_buffer)),
                       sizeof(data_buffer), _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  // The CS line should be toggled to allow for transmisions
  EXPECT_CALL(gpo_payload_chip_select, SetHigh()).Times(1);
  EXPECT_CALL(gpo_payload_chip_select, SetLow()).Times(1);

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      EmulatorMockTesting::PayloadDataStatus::kPayloadReady);
  // When Payload Comunication is called
  emulator.Run();
}

TEST_F(EmulatorTest, Run_IsPayloadPoweredOn) {
  // Expect a call to check if the the payload is turned on.
  EXPECT_CALL(gpo_payload_switch, SetHigh()).Times(2);
  // And is it turned off
  EXPECT_CALL(gpo_payload_switch, SetLow()).Times(1);

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      EmulatorMockTesting::PayloadDataStatus::kTrasferCompleted);
  // When
  emulator.Run();
}

TEST_F(EmulatorTest, PayloadReturnsError) {
  // Given a buffer with data
  uint8_t data_buffer[kMaxPayloadDataLength];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = static_cast<char>(i);
  };

  // when data is copied to the received data buffer from the payload, but
  // the function returns an Error. then the buffer will not be transmited,
  // instead an error message gets sent
  EXPECT_CALL(payload_com_mock, Receive(_, _, _))
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Error));

  // There can be any number of transmisions to the computer and no transmission
  // should be equal to the data buffer received from the payload as it returned
  // timeout
  EXPECT_CALL(computer_com_mock, Transmit(_, Not(kPacketLength), _))
      .Times(3)
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      EmulatorMockTesting::PayloadDataStatus::kPayloadReady);
  // When
  emulator.Run();
}

TEST_F(EmulatorTest, EmulatorTimeout) {
  uint32_t now = 0;
  uint32_t emulator_before_timeout =
      binarx::emulator::kWaitForPayloadMaxTime - 1;
  uint32_t emulator_timeout = binarx::emulator::kWaitForPayloadMaxTime + 1;

  EXPECT_CALL(time_mock, GetTicks())
      .Times(3)
      .WillOnce(Return(now))
      .WillOnce(Return(emulator_before_timeout))
      .WillOnce(Return(emulator_timeout));

  emulator.SetButtonPressed_TestOnly(true);
  // When
  emulator.Run();
}

// ---- Parametised Tests ----

class EmulatorParameterizedTestFixture1
    : public EmulatorTest,
      public ::testing::WithParamInterface<uint16_t> {};

TEST_P(EmulatorParameterizedTestFixture1, DataSizeError) {
  // Given an erroneus data size number
  uint16_t data_size = GetParam();
  uint16_t kNumOfPaquets = CalculateNumberOfPackets(data_size);
  // The emulator should receive the number of packets to the payload has to
  // send
  uint8_t metadata_packet[kNumberOfBytesInHeader] = {
      kSyncByte, static_cast<uint8_t>(data_size >> 8),
      static_cast<uint8_t>(data_size)};

  EXPECT_CALL(payload_com_mock, Receive(_, kNumberOfBytesInHeader, _))
      .WillOnce(
          DoAll(SetArrayArgument<0>(metadata_packet,
                                    metadata_packet + sizeof(metadata_packet)),
                Return(binarx_serial_interface::SerialStatus::Success)));

  // There can be three computer communications but non should be of packet
  // length.
  EXPECT_CALL(computer_com_mock, Transmit(_, Not(kPacketLength), _))
      .Times(3)
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      EmulatorMockTesting::PayloadDataStatus::kPayloadReady);
  // When
  emulator.Run();
}

INSTANTIATE_TEST_CASE_P(ErroneousDataSizeTest,
                        EmulatorParameterizedTestFixture1,
                        ::testing::Values(0, kMaxPayloadDataLength + 1));

class EmulatorParameterizedTestFixture2
    : public EmulatorTest,
      public ::testing::WithParamInterface<uint16_t> {};

TEST_P(EmulatorParameterizedTestFixture2, Run_DataTransferSuccess) {
  // Given a buffer with data that is sent succesfully from the payload
  uint16_t data_size = GetParam();
  uint16_t kNumOfPaquets = CalculateNumberOfPackets(data_size);
  uint8_t packet_buffer[kPacketLength];
  for (uint16_t i = 0; i < sizeof(packet_buffer); i++) {
    packet_buffer[i] = static_cast<uint8_t>(i);
  };

  uint8_t data_buffer[kNumOfPaquets * kPacketLength];
  for (uint16_t i = 0; i < sizeof(data_buffer); i++) {
    data_buffer[i] = packet_buffer[i % kPacketLength];
  }

  // The emulator should receive the number of packets to the payload has to
  // send
  uint8_t metadata_packet[kNumberOfBytesInHeader] = {
      kSyncByte, static_cast<uint8_t>(data_size >> 8),
      static_cast<uint8_t>(data_size)};

  EXPECT_CALL(payload_com_mock, Receive(_, kNumberOfBytesInHeader, _))
      .WillOnce(
          DoAll(SetArrayArgument<0>(metadata_packet,
                                    metadata_packet + sizeof(metadata_packet)),
                Return(binarx_serial_interface::SerialStatus::Success)));

  // Then the data received by the emulator from the payload should be equal to
  // the data sent to by the emulator to the Users Computer
  EXPECT_CALL(payload_com_mock, Receive(_, kPacketLength, _))
      .WillRepeatedly(DoAll(
          SetArrayArgument<0>(packet_buffer, packet_buffer + kPacketLength),
          Return(binarx_serial_interface::SerialStatus::Success)));

  // There can be any number of transmisions to the computer.
  EXPECT_CALL(computer_com_mock, Transmit(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly(Return(binarx_serial_interface::SerialStatus::Success));

  // But at least one must match the data receive from the payload
  EXPECT_CALL(computer_com_mock,
              Transmit(ArraysAreEqual(data_buffer, data_size), data_size, _))
      .WillOnce(Return(binarx_serial_interface::SerialStatus::Success));

  emulator.SetButtonPressed_TestOnly(true);
  emulator.SetPayloadStatus_TestOnly(
      EmulatorMockTesting::PayloadDataStatus::kPayloadReady);
  // When Payload Comunication is called
  emulator.Run();
}

INSTANTIATE_TEST_CASE_P(CorrectPacketNumberTest,
                        EmulatorParameterizedTestFixture2,
                        ::testing::Values(1, 50, 250, 500, 600,
                                          kMaxPayloadDataLength));