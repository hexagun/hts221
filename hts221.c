#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "hts221.h"

int fd_i2c = -1; 

__uint8_t H0, H1;
__uint16_t T0, T1, raw;
__int16_t H2, H3, T2, T3;

__uint16_t i2c_read_2Byte(unsigned char reg){
        unsigned char outbuf[1];
        unsigned char inbuf[2];

        outbuf[0]= reg;
        inbuf[0] = 0;
        inbuf[1] = 0;


        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = HTS221_ADDR;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = outbuf;

        msgs[1].addr = HTS221_ADDR;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 2;
        msgs[1].buf = inbuf;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if(ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
                return errno;
        }

        __uint16_t reg_value = inbuf[1]*256 + inbuf[0] ;
        return reg_value;
}

__uint8_t i2c_read_1Byte(unsigned char reg){
        unsigned char outbuf[1];
        unsigned char inbuf[1];

        outbuf[0]= reg;
        inbuf[0] = 0;


        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = HTS221_ADDR;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = outbuf;

        msgs[1].addr = HTS221_ADDR;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 2;
        msgs[1].buf = inbuf;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if(ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
                return errno;
        }

        __uint8_t reg_value = inbuf[0] ;
        return reg_value;
}

int i2c_init(const char* busName){
    if (!(fd_i2c = open(busName, O_RDWR))){
        return errno;
    }

    // write configuration register AV_CONF data 00011011
    i2c_write(HTS221_AV_CONF_ADDR, 0x1B );
    
    // write configuration register CTRL_REG1 data 10000101
    i2c_write(HTS221_CTRL_REG1_ADDR, 0x85 );
    
    // Read of data from CALIB_REG
    H0 = i2c_read_1Byte(HTS221_CALIB_REG_30)/2;
    H1 = i2c_read_1Byte(HTS221_CALIB_REG_31)/2;
    
    // Read 2 byte of data from address(CALIB_REG_36 and CALIB_REG_37)
    // REG | 0x80 auto increment reg value - datasheet sect : 5.1.1
    H2 =  i2c_read_2Byte(HTS221_CALIB_REG_36 | 0x80);

    // Read 2 byte of data from address(CALIB_REG_3A and CALIB_REG_3B)
    H3 =  i2c_read_2Byte(HTS221_CALIB_REG_3A | 0x80);

    // Read data from CALIB_REG
    T0 = i2c_read_1Byte(HTS221_CALIB_REG_32);
    T1 = i2c_read_1Byte(HTS221_CALIB_REG_33);

    raw = i2c_read_1Byte(HTS221_CALIB_REG_35);

    // Convert to 10 bits
    T0 = ((raw & 0x03) * 256) + T0;
    T1 = ((raw & 0x0C) * 64) + T1;

    //Read 2 byte of data from address(CALIB_REG_3C and CALIB_REG_3D)
    T2 =  i2c_read_2Byte(HTS221_CALIB_REG_3C | 0x80);
    
    //Read 2 byte of data from address(CALIB_REG_3E and CALIB_REG_3F)
    T3 =  i2c_read_2Byte(HTS221_CALIB_REG_3E | 0x80);
}

void i2c_close(void){
    close(fd_i2c);
}

int i2c_write(unsigned char reg, unsigned char byte ){
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    unsigned char outbuf[2];

    outbuf[0] = reg;
    outbuf[1] = byte;

    msgs[0].addr = HTS221_ADDR;
    msgs[0].flags = 0;// 0 for write 
    msgs[0].len = 2;
    msgs[0].buf = outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
        return errno;
    }

    return 0;
}

int enable_heater(){
    // Read 1 byte of data from address(0x21)
    __uint8_t reg_value = i2c_read_1Byte(HTS221_CTRL_REG2_ADDR);
    reg_value |= 0x02 ;
    i2c_write(HTS221_CTRL_REG2_ADDR, reg_value );
    return 0;
}

int disable_heater(){
    // Read 1 byte of data from address(0x21)
    __uint8_t reg_value = i2c_read_1Byte(HTS221_CTRL_REG2_ADDR);
    reg_value &= ~(0x02);
    i2c_write(HTS221_CTRL_REG2_ADDR, reg_value );
    return 0;
}

float hts221_GetTemp(){
    // Read 2 byte of data from address(0x2A and 0x2B)
    __int16_t temp =  i2c_read_2Byte(HTS221_TEMP_OUT_L | 0x80);
      
    if(temp > 32767){
        temp -= 65536;
    }

    float cTemp = ((T1-T0)/8.0) * (temp-T2)/(T3-T2) + (T0/8.0);

    return cTemp;
}

float hts221_GetHumidity(){
    // Read 2 byte of data from address(0x28 and 0x29)
    __int16_t hum =  i2c_read_2Byte(HTS221_HUMIDITY_OUT_L | 0x80);

    float humidity = (1.0*H1 - 1.0*H0) * (1.0*hum - 1.0*H2) / (1.0*H3 - 1.0*H2) + (1.0*H0);
    
    if( humidity > 100.0){
        enable_heater();
        sleep(5);
        disable_heater();
        sleep(1);
    }

    return humidity;
}
