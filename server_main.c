#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "server_ad.h"


int main(void)
{
    int r;
    pthread_t server;
    r=pthread_create(&server,NULL,server_f,NULL);
    if (r!=0){
        perror("Создание потока");
    }
    pthread_join(server,NULL);
    printf("Конец работ\n");
    return 0;
}


