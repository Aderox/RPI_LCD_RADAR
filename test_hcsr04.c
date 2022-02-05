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

void pulseIn(int gpio, int level, uint32_t tick)
{
    uint32_t start = gpioTick();
    uint32_t end = 0;

    // printf("[INFO] Start pulseIn\n");
    printf("[INFO] gpio %d\n", gpio);
    printf("[INFO] Et pouf le signal est en haut (%d), a t=%f\n", level, start);
    // unsigned long i = 0;
    while (gpioRead(GPIO_ECHO) == 1)
    { // TODO ADD TIMEOUT
        printf("[INFO] readGpio: %d\n", gpioRead(GPIO_ECHO));
        // on attend que le signal soit en bas
        end = gpioTick();
    }
    // end = gpioTick();
    printf("[INFO] fin du truc, a t=%f\n", end);
    printf("[INFO] Différence entre les deux: %f\n", end - start);
    printf("[INFO] Distance: %f\n", (end - start) / 0.58);
    return;
}

long getMicrotime()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

void poke()
{
    gpioWrite(GPIO_TRIG, 1);
    usleep(10);
    gpioWrite(GPIO_TRIG, 0);
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

    printf("time: %f \n", time_time());
    // gpioSetAlertFunc(GPIO_ECHO, pulseIn);
    while (1)
    {
        // supply power to vcc in order to start measurement and sleep 10 us

           
        double startTick;
        double endTick;
        float diffTick;
        
        double timeOut = time_time() + (1.5);
        int i = 0;
        int wasHigh = 0;

        printf("\nPOKE\n");
        poke();
        while (time_time() < timeOut)
        {
            if(gpioRead(GPIO_ECHO) == 1)
            {
                startTick = time_time();
                //printf("[DEBUG] startTick: %f\n", start);
                wasHigh = 1;
            }
            if(gpioRead(GPIO_ECHO) == 0 && wasHigh == 1)
            {
                endTick = time_time();
                printf("[DEBUG] End tick meusure:%d\n", endTick);
                break;
            }
        }
        diffTick = endTick - startTick;
        printf("[INFO] Time: %f\n", diffTick);

        /*
        * DEBUG TO TEST TICK
        */
       printf("==on teste==\n");
        startTick = time_time();
        //print start tick
        printf("[DEBUG] Start tick:%d\n", startTick);
        time_sleep(0.5);
        endTick  = time_time();
        //print end tick
        printf("[DEBUG] End tick:%d\n", endTick);
        diffTick = endTick - startTick;
        printf("time nul: %f\n\n", diffTick);
       printf("fin du test\n");

        /*while(gpioRead(GPIO_ECHO) == 0 && time_time() < timeOut){
            printf("%d\n", gpioRead(GPIO_ECHO));
            //printf("[INFO] readGpio: %d\n", gpioRead(GPIO_ECHO));
            //on attend que le signal soit en haut
        }
        start = time_time();
        while(gpioRead(GPIO_ECHO) == 1 && time_time() < timeOut){
            printf("%d\n", gpioRead(GPIO_ECHO));
            //printf("[INFO] readGpio: %d\n", gpioRead(GPIO_ECHO));
            //on attend que le signal soit en bas
        }
        end = time_time();
        if(time_time() < timeOut){
            printf("fin !\n");
            time_sleep(0.5);
            distance = (end - start)/0.58;
            printf("[INFO] Distance: %f\n", distance);
            printf("end: %f \n", end);
            printf("[INFO] Différence entre les deux: %f   us\n", (end - start));
        }
        else{
            printf("[ERROR] Timeout\n");
        }
       */

        time_sleep(2);
    }

    printf("ciao \n");
    gpioTerminate();
}