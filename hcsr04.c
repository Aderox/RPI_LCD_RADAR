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

void poke()
{
    gpioWrite(GPIO_TRIG, 1);
    usleep(10);
    gpioWrite(GPIO_TRIG, 0);
}

long meusureDistance()
{
    uint32_t startTick;
    uint32_t endTick;
    float diffTick;

    double timeOut = time_time() + (1.5);
    int wasHigh = 0;

    printf("\nPOKE\n");
    poke();
    while (time_time() < timeOut)
    {
        if (gpioRead(GPIO_ECHO) == 1)
        {
            if (!wasHigh)
            {
                startTick = gpioTick();
                wasHigh = 1;
            }
            wasHigh = 1;
        }
        if (gpioRead(GPIO_ECHO) == 0 && wasHigh == 1)
        {
            endTick = gpioTick();
            diffTick = endTick - startTick;
            return diffTick/58.0;
        }
    }
    return -1;
}



int main(int argc, char *argv[])
{
    printf("salut \n");
    // branchelent: gnd sur 39 et trig sur 37

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }

    gpioSetMode(GPIO_TRIG, PI_OUTPUT);
    gpioSetMode(GPIO_ECHO, PI_INPUT);

    printf("on bip:\n");

    double distance = 0;

    printf("time: %f \n", time_time());
    // gpioSetAlertFunc(GPIO_ECHO, pulseIn);
    while (1)
    {
        // supply power to vcc in order to start measurement and sleep 10 us

        // measure distance
        distance = meusureDistance();
        if(distance > 0)
        {
            printf("distance: %fcm\n", distance);
        }
        else
        {
            printf("[ERROR] timout: %fcm\n", distance);
        }

        time_sleep(2);
    }

    printf("ciao \n");
    gpioTerminate();
}
