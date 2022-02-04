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

void pulseIn(int gpio, int level, uint32_t tick)
{
    uint32_t start = gpioTick();
    uint32_t end = 0;

    //printf("[INFO] Start pulseIn\n");
    printf("[INFO] gpio %d\n", gpio);
    printf("[INFO] Et pouf le signal est en haut (%d), a t=%f\n", level, start);
    //unsigned long i = 0;
    while(gpioRead(GPIO_ECHO) == 1){                                        //TODO ADD TIMEOUT
       printf("[INFO] readGpio: %d\n", gpioRead(GPIO_ECHO));
       //on attend que le signal soit en bas
        end = gpioTick();
    }
    //end = gpioTick();
    printf("[INFO] fin du truc, a t=%f\n", end);
    printf("[INFO] Différence entre les deux: %f\n", end - start);
    printf("[INFO] Distance: %f\n", (end - start)/0.58);
    return;
}

void poke(){
    usleep(1);
    gpioWrite(GPIO_TRIG, 1);
    usleep(10);
    gpioWrite(GPIO_TRIG, 0);
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
    //gpioSetAlertFunc(GPIO_ECHO, pulseIn);
    while (1)
    {   
        printf("lessgo :");
        //supply power to vcc in order to start measurement and sleep 10 us
        poke();

        while(gpioRead(GPIO_ECHO) == 0){
            //printf("[INFO] readGpio: %d\n", gpioRead(GPIO_ECHO));
            //on attend que le signal soit en haut
        }    
        uint32_t start = gpioTick();
        while(gpioRead(GPIO_ECHO) == 1){
            //printf("[INFO] readGpio: %d\n", gpioRead(GPIO_ECHO));
            //on attend que le signal soit en bas
        }
        uint32_t end = gpioTick();
        printf("[INFO] Différence entre les deux: %zuus\n", (end - start);

        time_sleep(3);
       /* uint32_t start = gpioTick();
        time_sleep(3);
        uint32_t end = gpioTick();
        printf("[INFO] Différence entre les deux: %zu\n", (end - start)/1000000);
       */ 
        
        
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