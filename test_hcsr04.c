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

#define GPIO_TRIG 26 // fil rouge
#define GPIO_ECHO 13 // fil marron

volatile uint32_t before;  // used for sonar distance

int last_range = 0;	     // last sonar reading

void delay(int ms) {  // delay in miliseconds
    gpioDelay(1000*ms); 
}

void ping(void)
{ // send out an ultrasonic 'ping'

    before = 0xffffffff; // set for guard variable

    gpioSetMode(GPIO_TRIG, PI_OUTPUT);

    // trigger a sonar pulse

    gpioWrite(GPIO_TRIG, PI_OFF);
    gpioDelay(5);
    gpioWrite(GPIO_TRIG, PI_ON);
    gpioDelay(10);
    gpioWrite(GPIO_TRIG, PI_OFF);
    gpioDelay(5);

    gpioSetMode(GPIO_ECHO, PI_INPUT);

    before = gpioTick(); // get tick right after sending pulse
}

void range(int gpio, int level, uint32_t tick)
{

    static uint32_t startTick, endTick;

    uint32_t diffTick;

    if (tick > before)
    { // make sure we don't measure trigger pulse

        if (level == PI_ON)
        { // start counting on rising edge
            startTick = tick;
        }
        else

            if (level == PI_OFF)
        { // stop counting on falling edge

            endTick = tick;
            diffTick = (endTick - startTick) / 58;

            last_range = diffTick;

            if (diffTick < 600)
                printf("%u\n", diffTick);
            else
            {
                printf("OUT OF RANGE"); // for seeedstudio sensor
                last_range = 0;
            }
        }
    }
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
    int value = 0;

    printf("time: %lf \n", time_time());
    while (1)
    {
        // supply power to vcc in order to start measurement and sleep 10 us
        int i, n;
        char buff[128], ch;

        if (gpioInitialise() < 0)
            return 1;

        gpioSetMode(GPIO_ECHO, PI_INPUT);

        // register callback on change of sonar pin
        gpioSetAlertFunc(GPIO_ECHO, range);

        sleep(2);

        while (1)
        {

            ping(); // prime the last_range variable
            sleep(1);

            printf("distance: %d \n", last_range);
        }

        sleep(1);

        puts("Bye now!");

        gpioTerminate();

        return 0;
        /*

        //read value from ECHO pin

        double startTime = 0;
        double endTime = 0;
        double timeOut = time_time() + 1;
        //pulse in
        value = gpioRead(GPIO_ECHO);
        while(gpioRead(GPIO_ECHO) == 0 && time_time() < timeOut)
        {
            printf("[DEBUG] %d\n", gpioRead(GPIO_ECHO));
            startTime = time_time();
        }
        while(gpioRead(GPIO_ECHO) == 1 && time_time() < timeOut)
        {
            printf("[DEBUG] %d\n", gpioRead(GPIO_ECHO));
            endTime = time_time();
        }

        if(time_time() < timeOut)
        {
            double diffTime = endTime - startTime;
            printf("[DEBUG] diffTime: %lf\n", diffTime);

            //distance = ((endTime - startTime)*340*100)/2;;
            distance = diffTime / 0.000058;
            printf("Distance: %lfcm\n", distance);
        }
        else
        {
            printf("/!\\ timeout\n");
        }

        time_sleep(1);
        */
    }

    gpioTerminate();
}