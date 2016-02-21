#include "user_ad.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>



#define _GNU_SOURCE

#define MSG 1L //информационное сообщение
#define CONNECT 2L //запрос на подключение
#define DISCONNECT 3L //запрос на отключение


struct my_msg
{
    long mtype;
    char msg[50];
    pid_t name;
    int connect_res;

};

 void * client_f (){
     key_t key;
     int i,r;
     struct my_msg msg_buf;
     pid_t t;
     t=syscall(SYS_gettid);
     key = ftok("/root/",'W');
     int qid;
     qid=msgget(key,0);
     /*Если очередь не существует, выдаём информацию о ошибке*/
     if (qid==-1){
         printf("Я,клиент с PID:%d,не смог подключиться к очереди\n",t);
         perror("Подключение к очереди");
         return NULL;

     }
     /*Заполняем массив тексом сообщений*/
     char msgtxt[4][50];
     strcpy( msgtxt[0],"HI!!");
     strcpy( msgtxt[1],"How are you?");
     strcpy( msgtxt[2],"I'm fine!");
     strcpy( msgtxt[3],"Goodbye!");
     /*Отправляем запрос на подключение, ожидаем сообщение с типом равному пиду процесса, приведённому к long*/
     msg_buf.mtype=CONNECT;
     msg_buf.name=t;
     msgsnd(qid,&msg_buf,sizeof(msg_buf),0);
     msgrcv(qid,&msg_buf,sizeof(msg_buf),(long)t,0);
     /*Если вернулось значение отличное от -1, подключение к чату прошло успешно*/
     if (msg_buf.connect_res!= -1) {
         printf("Я,PID:%d, успешно подключился\n",t);
     }
     else {
         printf("Мне,PID:%d, не удалось подключиться к  чату.Нет свободных мест\n",t);
         return NULL;
     }
     /*В цикле отправляем в очередь информационные сообщения с ралзичным текстом
        после каждого сообщения ожидаем случайную задержку.задержка регулируется через константу SLEEP*/
     for (i=0;i<=3;i++){
         msg_buf.mtype=MSG;
         strcpy(msg_buf.msg,msgtxt[i]);
         r=msgsnd(qid,&msg_buf,sizeof(msg_buf),0);
         if (r!=0){
             perror("Отправка сообщения в очередь");
             return 0;
         }
         sleep(SLEEP+rand()%3);
     }
     /*После отравки всех сообщений в очередь, через случайное значение времени, посылаем запрос на отлкючение от чата
и завершаем поток*/
     sleep(SLEEP+rand() % 4);
     msg_buf.mtype=DISCONNECT;
     msg_buf.name=t;
     msgsnd(qid,&msg_buf,sizeof(msg_buf),0);
     return NULL;
 }
