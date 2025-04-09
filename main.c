#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
// #include "tof.h"
#include <stdbool.h>

// Default and new I2C addresses - valid addresses (0x08 - 0x77)
#define DEFAULT_I2C_ADDRESS 0x29
#define NEW_I2C_ADDRESS_FRONT 0x30
#define NEW_I2C_ADDRESS_REAR 0x31
#define VL53L0X_REG_RESULT_RANGE_STATUS 0x14

// VL53L0X Register Addresses
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS 0x8A

int change_i2c_address(int old_addr, int new_addr) {
    if (wiringPiI2CWriteReg8(old_addr, VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS, new_addr) < 0) {
        printf("Error changing I2C address\n");
        return -1;
    }
    printf("I2C address changed from 0x%X to 0x%X\n", DEFAULT_I2C_ADDRESS, new_addr);

    return wiringPiI2CSetup(new_addr);
}

void read_distance(int addr){
	usleep(50000);  // Small delay for measurement

	int data_high = wiringPiI2CReadReg16(addr, VL53L0X_REG_RESULT_RANGE_STATUS);
	int data_low = wiringPiI2CReadReg16(addr, VL53L0X_REG_RESULT_RANGE_STATUS + 1);
	if (data_high < 0 || data_low < 0) {
		printf("Error reading distance\n");
		return;
	}

	int distance = (data_high << 8) | data_low;
	printf("Distance: %d mm\n", distance);
};

int main() {

	int i, j;
	int front_Distance, rear_Distance;

	pthread_t front_thread;
	pthread_t rear_thread;

	int front_tof = wiringPiI2CSetup(DEFAULT_I2C_ADDRESS);
    if (front_tof < 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }

	int rear_tof = wiringPiI2CSetup(DEFAULT_I2C_ADDRESS);
    if (rear_tof < 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }


	wiringPiSetup();
	pinMode(0, OUTPUT); //wiringpi 0 is bcm 17
	pinMode(2, OUTPUT); //wiringpi 2 is bcm 27
	digitalWrite(0, LOW);
	digitalWrite(2, LOW);
	delay(10);
	digitalWrite(0, HIGH);
	digitalWrite(2, HIGH);
	delay(10);
	digitalWrite(2, LOW);
	
    
	delay(10);
    front_tof = change_i2c_address(front_tof, NEW_I2C_ADDRESS_FRONT);
    if (front_tof < 0) {
        return 1;
    }

	digitalWrite(2, HIGH);
	delay(10);
	rear_tof = change_i2c_address(rear_tof, NEW_I2C_ADDRESS_REAR);
    if (rear_tof < 0) {
        return 1;
    }

	

	// i = tofInit(1, NEW_I2C_ADDRESS_FRONT, 1); // set long range mode (up to 2m)
	// if (i != 1)
	// {
	// 	return -1; // problem - quit
	// }
	// for (i=0; i<1200; i++) // read values 20 times a second for 1 minute
	// {
	// 	// pthread_create(&front_thread, NULL, tofReadDistance, NULL);
	// 	// pthread_join(front_thread, NULL);

	// 	front_Distance = tofReadDistance();
	// 	if (front_Distance < 4096) // valid range?
	// 		printf("Distance = %dmm\n", front_Distance);
	// 	usleep(50000); // 50ms
	// 	// pthread_join(&front_thread, NULL);
	// }
	


	// j = tofInit(1, NEW_I2C_ADDRESS_REAR, 1); // set long range mode (up to 2m)
	// if (j != 1)
	// {
	// 	return -1; // problem - quit
	// }

	// for (j=0; j<1200; j++) // read values 20 times a second for 1 minute
	// {
	// 	rear_Distance = tofReadDistance();
	// 	if (rear_Distance < 4096) // valid range?
	// 		printf("Distance = %dmm\n", rear_Distance);
	// 	usleep(50000); // 50ms
	// }

	while (true){
		read_distance(NEW_I2C_ADDRESS_FRONT);
	}


	return 0;
}

