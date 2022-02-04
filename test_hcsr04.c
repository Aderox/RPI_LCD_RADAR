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

void pulseIn(){
    //printf("[INFO] Start pulseIn\n");
    printf("[INFO] readGpio: %d\n", readGpio(GPIO_ECHO));
}

void poke(){
    usleep(1);
    gpioWrite(GPIO_TRIG, 1);
    usleep(10);
    gpioWrite(GPIO_TRIG, 0);
    usleep(1);
}

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
        poke();
        gpioSetAlertFunc(GPIO_ECHO, pulseIn);
        
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