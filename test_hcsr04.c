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
    // branchelent: gnd sur 39 et trig sur 37

    if(gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    gpioSetMode(GPIO_PIN, PI_OUTPUT);

    time_sleep(1);
    printf("on bip: ");
    while (1)
    {
        gpioWrite(GPIO_PIN, 1); /* on */
        time_sleep(200);
        gpioWrite(GPIO_PIN, 0);
    }

    gpioTerminate();
}