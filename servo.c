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
#include <inttypes.h>

#define GPIO_TRIG 6 // fil bleu
#define GPIO_ECHO 5 // fil blanc

#define GPIO_SERVO 13 //fil gris


int main(int argc, char *argv[])
{
    printf("salut \n");
    // branchelent: gnd sur 39 et trig sur 37

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    gpioSetMode(GPIO_SERVO, PI_OUTPUT);

    if(gpioSetPWMfrequency(GPIO_SERVO, 50) < 0)
    {
        fprintf(stderr, "pigpio set pwm frequency failed\n");
        return 1;
    }

    printf("servo moteur script: \n");
    // gpioSetAlertFunc(GPIO_ECHO, pulseIn);
    int i = 0;
    while (1)
    {

        printf("cycle \n");
        gpioServo(GPIO_SERVO, i);
        i++;
        if(i>2500)
            i=0;
        time_sleep(0.1);
    }

    printf("ciao \n");
    gpioTerminate();
}