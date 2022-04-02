#include <stdint.h>

#ifndef __HTS221_H__
#define __HTS221_H__

// device identification
#define HTS221_ADDR                     0x5f
#define HTS221_WHO_AM_I_ADDR            0x0f
#define HTS221_WHO_AM_I_VAL             0Xbc

// humidity and temperature resolution mode register
#define HTS221_AV_CONF_ADDR             0x10
#define HTS221_AV_CONF_VAL              0x1b

// control registers
#define HTS221_CTRL_REG1_ADDR           0x20
#define HTS221_CTRL_REG2_ADDR           0x21
#define HTS221_CTRL_REG3_ADDR           0x22
#define HTS221_CTRL_REG_DEFAULT_VAL     0x00

// status register
#define HTS221_STATUS_REG_ADDR          0x27
#define HTS221_STATUS_REG_DEFAULT_VAL   0x00       

// humidity registers
#define HTS221_HUMIDITY_OUT_L           0x28
#define HTS221_HUMIDITY_OUT_H           0x29

// temperature registers
#define HTS221_TEMP_OUT_L               0x2a
#define HTS221_TEMP_OUT_H               0x2b

// calibration registers
#define HTS221_CALIB_REG_30             0x30
#define HTS221_CALIB_REG_31             0x31
#define HTS221_CALIB_REG_32             0x32
#define HTS221_CALIB_REG_33             0x33
#define HTS221_CALIB_REG_34             0x34
#define HTS221_CALIB_REG_35             0x35
#define HTS221_CALIB_REG_36             0x36
#define HTS221_CALIB_REG_37             0x37
#define HTS221_CALIB_REG_38             0x38
#define HTS221_CALIB_REG_39             0x39
#define HTS221_CALIB_REG_3A             0x3a
#define HTS221_CALIB_REG_3B             0x3b
#define HTS221_CALIB_REG_3C             0x3c
#define HTS221_CALIB_REG_3D             0x3d
#define HTS221_CALIB_REG_3E             0x3e
#define HTS221_CALIB_REG_3F             0x3f

int i2c_write(unsigned char reg, unsigned char byte );

uint16_t i2c_read_2Byte(unsigned char reg);

uint8_t i2c_read_1Byte(unsigned char reg);

int enable_heater();

int disable_heater();

int i2c_init(const char*);

void i2c_close(void);

float hts221_GetTemp();

float hts221_GetHumidity();

#endif