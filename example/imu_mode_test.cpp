#include <mbed.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

#define BNO055_I2C_FREQUENCY 100000L
#define BNO055_SENSOR_ID 1

Ticker failure;
DigitalOut led1(LED1,0), led2(LED2,0);

static void signal_bno055_fail()
{
    static bool first = true;
    if(first){
        led1 = 1;
        first = false;
        return;
    }
    led1 = !led1;
    led2 = !led2;
}

int main()
{
    I2C * i2c_ptr = new I2C(I2C1_SDA, I2C1_SCL);
    Adafruit_BNO055 bno055(i2c_ptr,BNO055_I2C_FREQUENCY,BNO055_SENSOR_ID);

    printf("Program started!\r\n");

    if(!bno055.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS))
    {
        printf("There was problem with sensors initialization!\r\n");
        failure.attach(callback(signal_bno055_fail),0.5f);
        return 1;
    }

    bno055.setExtCrystalUse(true);
    sensors_event_t event;
    
    while(1)
    {
        bno055.getEvent(&event);
        printf("%10d: X: %3.2fY: %3.2f Z: %3.2f\r\n", 
        event.timestamp, event.orientation.x ,event.orientation.y ,event.orientation.z);
        ThisThread::sleep_for(100);
    }
}