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

int fd_i2c = -1; // i2c bus file descriptor
const char *i2c_bus = "/dev/apalis-i2c1";
unsigned char slave_address = 0x5f;


int i2c_init(void)
{
    if ((fd_i2c = open(i2c_bus, O_RDWR)) < 0)
    {
        printf("Failed to open apalis-i2c1.");
        return -1;
    }
    return fd_i2c;
}

void i2c_close(void) 
{
    close(fd_i2c);
}

// Write to an I2C slave device's register:
int i2c_write(unsigned char slave_addr, unsigned char reg, unsigned char byte )
{
    struct i2c_msg msgs[1];
    struct i2c_rdwr_ioctl_data msgset[1];

    unsigned char outbuf[2];

    outbuf[0] = reg;
    outbuf[1] = byte;

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;// 0 for write 
    msgs[0].len = 2;
    msgs[0].buf = outbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 1;

    if (ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
        perror("ioctl(I2C_RDWR) in i2c_write");
        return -1;
    }

    return 0;
}


// Read the given I2C slave device's register and return the read value in `*result`:
__uint16_t i2c_read_2Byte(unsigned char slave_addr, unsigned char reg) 
{
        unsigned char outbuf[1];
        unsigned char inbuf[2];

        outbuf[0]= reg;
        inbuf[0] = 0;
        inbuf[1] = 0;


        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slave_addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = outbuf;

        msgs[1].addr = slave_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 2;
        msgs[1].buf = inbuf;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if(ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
                perror("ioctl(I2C_RDWR) in i2c_read");
                return -1;
        }

        __uint16_t reg_value = inbuf[1]*256 + inbuf[0] ;
        return reg_value;
}

__uint8_t i2c_read_1Byte(unsigned char slave_addr, unsigned char reg) 
{
        unsigned char outbuf[1];
        unsigned char inbuf[1];

        outbuf[0]= reg;
        inbuf[0] = 0;


        struct i2c_msg msgs[2];
        struct i2c_rdwr_ioctl_data msgset[1];

        msgs[0].addr = slave_addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = outbuf;

        msgs[1].addr = slave_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = 2;
        msgs[1].buf = inbuf;

        msgset[0].msgs = msgs;
        msgset[0].nmsgs = 2;

        if(ioctl(fd_i2c, I2C_RDWR, &msgset) < 0) {
                perror("ioctl(I2C_RDWR) in i2c_read");
                return -1;
        }

        __uint8_t reg_value = inbuf[0] ;
        return reg_value;
}


int main()
{
        unsigned char reg;
        i2c_init();

        //write configuration register 0x10
        reg = 0x10;
        i2c_write(slave_address, reg, 0x1B );

        //write configuration register 0x20
        reg = 0x20;
        i2c_write(slave_address, reg, 0x85 );

        //Read 1 byte of data from address(0x30)
        reg = 0x30;
        __uint8_t H0 = i2c_read_1Byte(slave_address, reg);
        H0 = H0 /2;

        //Read 1 byte of data from address(0x31)
        reg = 0x31;
        __uint8_t H1 = i2c_read_1Byte(slave_address, reg);
        H1 = H1 /2;

        
        //Read 2 byte of data from address(0x36 and 0x37)
        reg = 0x36 | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t H2 =  i2c_read_2Byte(slave_address, reg);

        //Read 2 byte of data from address(0x3a and 0x3b)
        reg = 0x3a | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t H3 =  i2c_read_2Byte(slave_address, reg);

        //Read 1 byte of data from address(0x32)
        reg = 0x32;
        __uint16_t T0 = i2c_read_1Byte(slave_address, reg);

        //Read 1 byte of data from address(0x33)
        reg = 0x33;
        __uint16_t T1 = i2c_read_1Byte(slave_address, reg);

        //Read 1 byte of data from address(0x35)
        reg = 0x35;
        __uint16_t raw = i2c_read_1Byte(slave_address, reg);

        // Convert the temperature Calibration values to 10-bits
	T0 = ((raw & 0x03) * 256) + T0;
	T1 = ((raw & 0x0C) * 64) + T1;

        //Read 2 byte of data from address(0x3C and 0x3D)
        reg = 0x3C | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t T2 =  i2c_read_2Byte(slave_address, reg);

        //Read 2 byte of data from address(0x3E and 0x3F)
        reg = 0x3E | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t T3 =  i2c_read_2Byte(slave_address, reg);

        //Read 2 byte of data from address(0x28 and 0x29)
        reg = 0x28 | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t hum =  i2c_read_2Byte(slave_address, reg);

        //Read 2 byte of data from address(0x2A and 0x2B)
        reg = 0x2A | 0x80 ;// | 0x80 auto increment reg value - datasheet sect : 5.5.1
        __int16_t temp =  i2c_read_2Byte(slave_address, reg);
        
        if(temp > 32767)
        {
                temp -= 65536;
        }

        float humidity = ((1.0 * H1) - (1.0 * H0)) * (1.0 * hum - 1.0 * H2) / (1.0 * H3 - 1.0 * H2) + (1.0 * H0);
        if( humidity > 100.0)
	{
		humidity = 100.00;
	}
        float cTemp = ((T1 - T0) / 8.0) * (temp - T2) / (T3 - T2) + (T0 / 8.0);
        float fTemp = (cTemp * 1.8 ) + 32;
    
        printf("Relative humidity : %.2f % \n", humidity);
        printf("Temperature in Celsius : %.2f C \n", cTemp);
        printf("Temperature in Fahrenheit : %.2f F \n", fTemp);


        i2c_close();
        return 0;
}

