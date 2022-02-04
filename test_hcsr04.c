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

int main(int argc, char *argv[])
{
    // branchelent: gnd sur 39 et trig sur 37

    if(gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    gpioSetMode(37, PI_OUTPUT);
    gpioServo(37, 1000000);

    time_sleep(1);
    while (1)
    {
        gpioWrite(18, 1); /* on */
        time_sleep(0.5);
        gpioWrite(18, 0);
    }
}