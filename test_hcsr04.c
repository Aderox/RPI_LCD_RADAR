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

    double distance = 0;
    int value = 0;

    printf("time: %lf \n", time_time());
    while (1)
    {   
        //supply power to vcc in order to start measurement and sleep 10 us
        gpioWrite(GPIO_TRIG, 1);
        usleep(10);
        gpioWrite(GPIO_TRIG, 0);


        //read value from ECHO pin

        double startTime = 0;
        double endTime = 0;
        double timeOut = time_time() + 0.5;
        //pulse in
        value = gpioRead(GPIO_ECHO);
        while(gpioRead(GPIO_ECHO) == 0 && time_time() < timeOut)
        {
            //printf("[DEBUG] low value: %d\n", gpioRead(GPIO_ECHO));
            startTime = time_time();
        }
        while(gpioRead(GPIO_ECHO) == 1 && time_time() < timeOut)
        {
            //printf("[DEBUG] hight value: %d\n", gpioRead(GPIO_ECHO));
            endTime = time_time();
        }

        if(time_time() < timeOut)
        {
            double diffTime = endTime - startTime;
            printf("[DEBUG] diffTime: %lf\n", diffTime);

            distance = ((endTime - startTime)*340*100)/2;; 
            printf("Distance: %lfcm\n", distance);
        }
        else
        {
            printf("/!\\ timeout\n");
        }

        time_sleep(1);
        //int timeOut = time_time()+5;

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
    }

    gpioTerminate();
}