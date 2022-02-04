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

#define GPIO_TRIG 26 //fil rouge
#define GPIO_ECHO 13 //fil marron

int main(int argc, char *argv[])
{   
    printf("salut \n");
    // branchelent: gnd sur 39 et trig sur 37

    if(gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    gpioSetMode(GPIO_TRIG, PI_OUTPUT);
    gpioSetMode(GPIO_ECHO, PI_INPUT);

    printf("on bip:\n");
    while (1)
    {   
        //supply power to vcc in order to start measurement and time_sleep 10 us
        gpioWrite(GPIO_TRIG, 1);
        time_sleep(0.00001);
        gpioWrite(GPIO_TRIG, 0);

        //read value from ECHO pin
        int value = gpioRead(GPIO_ECHO);
        printf("value: %d\n", value);
        time_sleep(0.1);
    }

    gpioTerminate();
}