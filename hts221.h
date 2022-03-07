#ifndef HTS221_H
#define HTS221_H

#include <stdio.h> // printf()
#include <sys/types.h> // open()
#include <sys/stat.h> // open()
#include <fcntl.h> // open()
#include <sys/ioctl.h> // ioctl()
#include <errno.h> // errno
#include <string.h> // strerror()
#include <unistd.h> // close()
#include <linux/i2c-dev.h> // struct i2c_msg
#include <linux/i2c.h> // struct i2c_rdwr_ioctl_data

#define slave_addr 0x5f

int i2c_write(unsigned char slave_address, unsigned char reg, unsigned char byte );
__uint16_t i2c_read_2Byte(unsigned char slave_address, unsigned char reg);
__uint8_t i2c_read_1Byte(unsigned char slave_address, unsigned char reg);
int EnableHeater();
int DisableHeater();
int i2c_init(void);
void i2c_close(void);

#endif