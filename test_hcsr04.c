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

    int startTime = 0;
    int endTime = 0;
    int distance = 0;
    int value = 0;
    int previousValue = 0;
    while (1)
    {   
        //supply power to vcc in order to start measurement and sleep 10 us
        gpioWrite(GPIO_TRIG, 1);
        usleep(10);
        gpioWrite(GPIO_TRIG, 0);

        //read value from ECHO pin
        while(1){
            value = gpioRead(GPIO_ECHO);
            if(value != 0){
                printf("value: %d\n", value);
            }else{
                //0
            }
        }
        
        int timeOut = time_time()+5;

        /*while(gpioRead(GPIO_ECHO) == 0)// && (timeOut) > time_time())
        {
            //printf("%d\n", gpioRead(GPIO_ECHO));
            startTime = time_time();
        }
        while(gpioRead(GPIO_ECHO) == 1)// && (timeOut) > time_time())
        {
            //printf("%d\n", gpioRead(GPIO_ECHO));
            printf("read=1\n");
            endTime = time_time();
        }*/
        
        //printf("time out. difftemps: %d\n", startTime - endTime);


        /*while(value == 0)
        {
            previousValue = value;
            value = gpioRead(GPIO_ECHO);
            if(value == 1)
            {
                endTime = time_time();
            }
            //printf("value: %d\n", value);
        }

        printf("la valeur n'est pas nul !\n");
        endTime = time_time();
        distance = ((endTime - startTime) * 34300) / 2;
        printf("=== distance: %d cm ===\n", distance);*/


        time_sleep(0.1);
    }

    gpioTerminate();
}