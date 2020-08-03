#include <mbed.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

#define BNO055_I2C_FREQUENCY 100000L
#define BNO055_SENSOR_ID 1
#define CALIBRATION_SLEEP_MS 1000

#define PRINT_GRYO_CALIBRATION_STATUS 1<<0
#define PRINT_ACC_CALIBRATION_STATUS  1<<1
#define PRINT_MAG_CALIBRATION_STATUS  1<<2
#define PRINT_ALL_CALIBRATION_STATUS  (1<<0 | 1<<1 | 1<<2) 

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

static int printCalibrationStatus(Adafruit_BNO055 & sensor, int mode)
{
    uint8_t system, gyro, accel, mag;
    sensor.getCalibration(&system, &gyro, &accel, &mag);

    const char * status_str_ptr = NULL;
    const char * sensor_str_ptr = NULL;

    switch(mode)
    {
        case PRINT_GRYO_CALIBRATION_STATUS:
            status_str_ptr = gyro == 0x3 ? "CALIBRATED" : "NOT CALIBRATED";
            sensor_str_ptr = "gyro";
            break;
        case PRINT_ACC_CALIBRATION_STATUS:
            status_str_ptr = accel == 0x3 ? "CALIBRATED" : "NOT CALIBRATED";
            sensor_str_ptr = "accel";
            break;
        case PRINT_MAG_CALIBRATION_STATUS:
            status_str_ptr = mag == 0x3 ? "CALIBRATED" : "NOT CALIBRATED";
            sensor_str_ptr = "mag";
            break;
        case PRINT_ALL_CALIBRATION_STATUS:
            status_str_ptr = system == 0x3 ? "CALIBRATED" : "NOT CALIBRATED";
            sensor_str_ptr = "system";
            break;
        default:
            break;
    }
    printf("%s calibration status: %s\r\n", sensor_str_ptr, status_str_ptr);
    
    int res = 0, position = 0;
    res |= gyro   == 0x3 ? 1 << position : 0; position++;
    res |= accel  == 0x3 ? 1 << position : 0; position++;
    res |= mag    == 0x3 ? 1 << position : 0; position++;
    res |= system == 0x3 ? 1 << position : 0; position++;
    return res;
}

int main()
{
    I2C * i2c_ptr = new I2C(I2C1_SDA, I2C1_SCL);
    Adafruit_BNO055 bno055(i2c_ptr,BNO055_I2C_FREQUENCY,BNO055_SENSOR_ID);

    printf("Program started!\r\n");

    if(!bno055.begin())
    {
        printf("There was problem with sensors initialization!\r\n");
        failure.attach(callback(signal_bno055_fail),0.5f);
        while(1);
    }
    
    // Gyroscope calibration
    printf("To calibrate gyroscope leave sensor motionless for a few seconds.\r\n");
    while(!(printCalibrationStatus(bno055, PRINT_GRYO_CALIBRATION_STATUS) & 1))
    {
        ThisThread::sleep_for(CALIBRATION_SLEEP_MS);
    }
    printf("Gyro calibartion successful!\r\n\n");

    // Accelerometer calibration
    printf("To calibrate accelerometer you will need to place the device in 6 stable positions, perpendicular to axis x,y and z.\n\r"
           "In each postion leave the device for a period of few seconds to allow the accelerometer to stabilize.\n\r"
           "Make sure there is slow movement between 2 stable positions.\n\r");
    while(!(printCalibrationStatus(bno055, PRINT_ACC_CALIBRATION_STATUS) & 2))
    {
        ThisThread::sleep_for(CALIBRATION_SLEEP_MS);
    }
    printf("Accel calibration successful!\r\n\n");

    // Magneteometer calibration
    printf("to calibrate magnetometer try to write \"8\" symbol in air with the device.\r\n");
    while(!(printCalibrationStatus(bno055, PRINT_MAG_CALIBRATION_STATUS) & 4))
    {
        ThisThread::sleep_for(CALIBRATION_SLEEP_MS);
    }
    printf("Magnetometer calibration successful!\r\n\n");
    
    printf("Your device's calibration was successful!\r\n");
}
