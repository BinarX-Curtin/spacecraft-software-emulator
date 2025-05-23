/**
 * @file fgd02f.c
 * @author Will Blackie
 * @brief This file contains the function implementations for interacting with
 * the fgd02f Dosimeter device.
 *
 */

 #include "fgd02f.h"

uint32_t FGD02FWrite(uint8_t *write_buf,
                         uint16_t len) {
  return _dosimeter->write_command(write_buf, len);
}
 
uint32_t FGD02FRead(uint8_t *read_buf,
                        uint16_t len) {
  return _dosimeter->read_command(read_buf, len);
}
 
void FGD02FInit(const fgd02f dosimeter){
  uint8_t pID = GetPid();
  if (pID != CHIPID_VAL) {
    // Handle error: incorrect device ID
    return;
  }  

  // assign the dosimeter to the global variable
  _dosimeter = dosimeter;

  uint8_t write_buf[1] = {CHIPID};
  uint8_t read_buf[1] = {0};

  // setup the chip


}

uint8_t GetPid() {
  uint8_t write_buf[1] = {CHIPID};
  uint8_t read_buf[1] = {0};

  FGD02FWrite(write_buf, sizeof(write_buf));
  FGD02FRead(read_buf, sizeof(read_buf));

  return *read_buf;
}
 
uint32_t SensorCounter() {
  uint8_t write_buf[2] = {INTERNAL_CONTROL_0, (INT_MEAS_DONE_EN | TM_M)};
  return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}

uint32_t ReferenceCounter() {
  uint8_t write_buf[2] = {INTERNAL_CONTROL_0, (INT_MEAS_DONE_EN | TM_M)};
  return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}
 
uint32_t TemperatureMeasurement() {
  uint8_t write_buf[2] = {INTERNAL_CONTROL_0, (INT_MEAS_DONE_EN | TM_T)};
  return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}
 
field_axes_t GetField(const mmc5983ma_t *magnetometer) {
  uint8_t write_buf[1] = {X_OUT_0};

  uint8_t read_buf[6] = {0};

  Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
  Mmc5983maRead(magnetometer, read_buf, sizeof(read_buf));

  uint16_t x_axis = (read_buf[0] << 8) | read_buf[1];
  uint16_t y_axis = (read_buf[2] << 8) | read_buf[3];
  uint16_t z_axis = (read_buf[4] << 8) | read_buf[5];

  field_axes_t axes_readings = {
      .x_axis = x_axis, .y_axis = y_axis, .z_axis = z_axis};

  return axes_readings;
}
 
uint8_t GetTemp(const mmc5983ma_t *magnetometer) {
  uint8_t write_buf[1] = {TEMP_OUT};
  uint8_t read_buf[1] = {0};

  Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
  Mmc5983maRead(magnetometer, read_buf, sizeof(read_buf));

  return *read_buf;
}
 
uint32_t ClearMagFieldInt(const mmc5983ma_t *magnetometer) {
   uint8_t write_buf[2] = {STATUS, MEAS_M_DONE};
   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}
 
uint32_t ClearTempInt(const mmc5983ma_t *magnetometer) {
   uint8_t write_buf[2] = {STATUS, MEAS_T_DONE};
   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}
 
uint32_t Set(const mmc5983ma_t *magnetometer) {
   uint8_t write_buf[2] = {INTERNAL_CONTROL_0, MAG_SET};
   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}
 
uint32_t Reset(const mmc5983ma_t *magnetometer) {
   uint8_t write_buf[2] = {INTERNAL_CONTROL_0, MAG_RESET};
   return Mmc5983maWrite(magnetometer, write_buf, sizeof(write_buf));
}