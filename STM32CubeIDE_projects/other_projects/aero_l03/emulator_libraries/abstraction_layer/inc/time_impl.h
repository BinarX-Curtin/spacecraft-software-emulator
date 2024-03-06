/**
 * @file time_impl.h
 * @author Tristan Ward
 * @brief time implementation of the time interface within the stm32CubeIDE
 * project
 *
 */
#pragma once
#include "abstraction_layer/time_interface.h"

namespace binarx_time_impl {

class TimeImpl : public binarx_time_interface::TimeInterface {
 public:
  TimeImpl(){};
  /**
   * @brief Delay implements the Syncronous Delay
   *
   * @param millisiconds The requested delay in milliseconds
   */
  void Delay(uint32_t milliseconds);

  /**
   * @brief Get the Ticks object to get the number of miliseconds the system has
   * been on for
   *
   * @return uint32_t The number of milliseconds the system has been on for
   */
  uint32_t GetTicks();

 private:
};
}  // namespace binarx_time_impl
