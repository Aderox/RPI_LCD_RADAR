#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <pigpio.h>
#include <math.h>

#define GPIO_PIN 26

int main(int argc, char *argv[])
{   
    printf("salut \n");
    // branchelent: gnd sur 39 et trig sur 37

    if(gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    gpioSetMode(GPIO_PIN, PI_OUTPUT);

    time_sleep(1);
    printf("on bip:\n");
    time_sleep()
    while (1)
    {
        gpioWrite(GPIO_PIN, 0); /* on */
        time_sleep(0.05);
        gpioWrite(GPIO_PIN, 0);
        time_sleep(0.5);
    }

    gpioTerminate();
}