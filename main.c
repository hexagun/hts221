#include <stdio.h> 
#include <string.h>
#include "hts221.h"

int main(){
    float temperature;
    float humidity;

    const char* bus = "/dev/apalis-i2c1";
    
    unsigned char reg;
    
    i2c_init(bus);       

    temperature = hts221_GetTemp();
    humidity = hts221_GetHumidity();

    printf("Relative humidity : %.2f \n", humidity);
    printf("Temperature in Celsius : %.2f C \n", temperature);
    printf("Temperature in Fahrenheit : %.2f F \n", (temperature*1.8) + 32);

    i2c_close();

    return 0;
}
