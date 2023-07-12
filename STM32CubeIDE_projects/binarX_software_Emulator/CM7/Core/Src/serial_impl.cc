/**
 * @file spi_impl.cc
 * @author Tristan Ward
 * @brief Implementation for SPI communication c
 *
 */

#include "serial_impl.h"

namespace binarx_serial_impl {

binarx_serial_interface::SerialStatus SpiImpl::Transmit(uint8_t *buffer,
                                                        uint16_t size,
                                                        uint32_t timeout) {
  HAL_StatusTypeDef hal_status =
      HAL_SPI_Transmit(&hspi1, buffer, size, timeout);
  return SerialErrorHandler(hal_status);
};
binarx_serial_interface::SerialStatus SpiImpl::Receive(uint8_t *receive_buffer,
                                                       uint16_t size,
                                                       uint32_t timeout) {
  HAL_StatusTypeDef hal_status =
      HAL_SPI_Receive(&hspi1, receive_buffer, size, timeout);
  return SerialErrorHandler(hal_status);
};

binarx_serial_interface::SerialStatus UartImpl::Transmit(uint8_t *buffer,
                                                         uint16_t size,
                                                         uint32_t timeout) {
  HAL_StatusTypeDef hal_status =
      HAL_UART_Transmit(&huart3, buffer, size, timeout);
  return SerialErrorHandler(hal_status);
};
binarx_serial_interface::SerialStatus UartImpl::Receive(uint8_t *receive_buffer,
                                                        uint16_t size,
                                                        uint32_t timeout) {
  HAL_StatusTypeDef hal_status =
      HAL_UART_Receive(&huart3, receive_buffer, size, timeout);
  return SerialErrorHandler(hal_status);
};

binarx_serial_interface::SerialStatus SerialErrorHandler(
    HAL_StatusTypeDef hal_status) {
  switch (hal_status) {
    case HAL_OK:
      return binarx_serial_interface::SerialStatus::Success;
    case HAL_ERROR:
      return binarx_serial_interface::SerialStatus::Error;
    case HAL_BUSY:
      return binarx_serial_interface::SerialStatus::Busy;
    case HAL_TIMEOUT:
      return binarx_serial_interface::SerialStatus::Timeout;
    default:
      return binarx_serial_interface::SerialStatus::Error;
  }
};

}  // namespace binarx_serial_impl
