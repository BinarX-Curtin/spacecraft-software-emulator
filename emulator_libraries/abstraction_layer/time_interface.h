/**
 * @file time_interface.h
 * @author Tristan Ward
 * @brief timer relatad interface for time specific selectable dependencies
 *
 */
#pragma once
#include <stdint.h>

namespace binarx_time_interface {

class TimeInterface {
 public:
  /**
   * @brief Delay implements the Syncronous Delay
   *
   * @param millisiconds The requested delay in milliseconds
   */
  virtual void Delay(uint32_t milliseconds) = 0;

  /**
   * @brief Get the Ticks object to get the number of miliseconds the system has
   * been on for
   *
   * @return uint32_t The number of milliseconds the system has been on for
   */
  virtual uint32_t GetTicks() = 0;
};

}  // namespace binarx_time_interface