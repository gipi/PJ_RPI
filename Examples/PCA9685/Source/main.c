#include "PJ_RPI.h"
#include "PCA9685.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

struct timespec tp;
long startTime, procesTime;

void exit_handler(int s)
{
	PCA9685_SetDutyCycle(0, 0);
	PCA9685_SetDutyCycle(1, 0);
	PCA9685_SetDutyCycle(2, 0);
	PCA9685_SetDutyCycle(3, 0);
	exit(1);
}

int main()
{
	// Gain access to peripheral memory structures
    if(map_peripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }
    if(map_peripheral(&bsc0) == -1) {
        printf("Failed to map the physical BSC0 (I2C) registers into the virtual memory space.\n");
        return -1;
    }
 
    // I2C initialization
    i2c_init();

	// Initialize Hardware
	PCA9685_Init();

	// Set PWM Frequency to 100Hz
	PCA9685_SetFrequency(100);
	PCA9685_SetDutyCycle(0, 10);
	sleep(2);

	// Catch ctrl+C so that the motors stop then (SAFETY)
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = exit_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);
 
	while(1)
	{
		// Get process start time
		clock_gettime(CLOCK_REALTIME, &tp);
		startTime = tp.tv_sec*1000000000 + tp.tv_nsec;

		// Update PWM ; Convert percentage from O -> 100 to 10 -> 20
		PCA9685_SetDutyCycle(0, 10+0.1*100);
		
		// Get process end time
		clock_gettime(CLOCK_REALTIME, &tp);
		procesTime = (tp.tv_sec*1000000000 + tp.tv_nsec) - startTime;
		// Wait until process has lasted 10 ms
		while(procesTime < 10000000)
		{
			clock_gettime(CLOCK_REALTIME, &tp);
			procesTime = (tp.tv_sec*1000000000 + tp.tv_nsec) - startTime;
		}
	}
}  
