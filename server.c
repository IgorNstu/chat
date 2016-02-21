#include "server_ad.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define MSG 1L //информационное сообщение
#define CONNECT 2L //запрос на подключение
#define DISCONNECT 3L // запрос на отключение


/*Структура сообщений в очереди*/
struct my_msg
{
    long mtype;
    char msg[50];
    pid_t name;
    int connect_res;

};

/*Обработчик сигнала прерывания*/
void my_sig1 (int param){
    key_t key;
    int qid;
    clear_term();
    key = ftok("/root/",'W');
    qid=msgget(key,0);
    msgctl(qid,IPC_RMID,NULL); //удаляем очередь
    printf("Программа завершена по сигналу с номером:%d\n",param);
    exit(0);


}

/*Поток сервера. Подключает и отключает пользователей.Выводит сообщения на экран*/
void * server_f (void *arg) {
    key_t key;
    int r;
    struct my_msg msg_buf;
    key = ftok("/root/",'W');
    int qid;
    /*Если очередь уже существовала , удаляем её и создаём заново*/
    qid=msgget(key,IPC_CREAT|0666|IPC_EXCL);
       if(qid==-1)
       {

           qid=msgget(key,IPC_CREAT|0666);
            if(qid== -1){
                perror("создание очереди");
                return NULL;
            }
       }
    /*Отрисовываем интерфейс чата*/
    new_chat_window();
    signal(SIGINT,my_sig1);
    while(1){
        /*Получаем сообщение из очереди*/
        msgrcv(qid,&msg_buf,sizeof(msg_buf),0,0);
        /*Если сообщение информационное, передаём поле с текстом функции,выводящей текст на экран*/
        if (msg_buf.mtype==MSG) {
            print_msg(msg_buf.msg,msg_buf.name);
        }

        else if (msg_buf.mtype==CONNECT){
            r=new_user(msg_buf.name);
            if (r!= -1){
                /*Если пользователь добавлен успешно,заполняем поле в сообщении его номером в чате
                  а тип сообщения приравниваем к его PID(приведя его к Long)*/
                msg_buf.connect_res=r;
                msg_buf.mtype=(long)msg_buf.name;
            }
            else {
                /*Если подключить не удалось, то отправляем в поле значение -1*/
                msg_buf.connect_res=-1;
                msg_buf.mtype=(long)msg_buf.name;;
            }
            msgsnd(qid,&msg_buf,sizeof(msg_buf),0);
        }
        else if (msg_buf.mtype==DISCONNECT){
            delete_user(msg_buf.name);
        }

    }
}

