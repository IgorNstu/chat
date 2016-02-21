#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "user_ad.h"




int main (void)
{
    int r;
    int i;
    pthread_t client[CLIENT_NUM];

    for (i=0;i<CLIENT_NUM;i++) {
        r=pthread_create(&client[i],NULL,client_f,NULL);
        if (r!=0){
            perror("Создание потока");
        }
        sleep(SLEEP+rand() % 2);
    }
    
    for (i=0;i<CLIENT_NUM;i++) {
        pthread_join(client[i],NULL);
        }
    return 0;
}

