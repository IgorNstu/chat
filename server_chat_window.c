#include <stdio.h>
#include <ncurses.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


WINDOW* chat;
WINDOW* user_list;
WINDOW* user_list_box;


/*Текущие строки окон*/
int y_ptr_msg=2,y_ptr_list=1;

/*Создание интерфейса чата*/
void new_chat_window (void) {
    initscr();
    curs_set(0);
    keypad(stdscr,0);
    noecho();
    start_color();
    init_pair(1,COLOR_BLUE,COLOR_WHITE);

    user_list_box = newwin(getmaxy (stdscr),getmaxx(stdscr)/2,0,getmaxx(stdscr)/2);
    chat = newwin(getmaxy(stdscr),(getmaxx(stdscr)/2)-1,0,0);
    user_list= newwin((getmaxy (stdscr)-2),(getmaxx(stdscr)/2-2),1,getmaxx(stdscr)/2+1);

    wbkgd(chat,COLOR_PAIR(1));
    wbkgd(user_list,COLOR_PAIR(1));
    wbkgd(user_list_box,COLOR_PAIR(1));
    wattron(chat,COLOR_PAIR(1)|A_BOLD);
    wattron(user_list,COLOR_PAIR(1)|A_BOLD);
    wattron(user_list_box,COLOR_PAIR(1)|A_BOLD);
    box(chat,0,0);
    box(user_list_box,0,0);
    wmove(chat,1,1);
    wprintw(chat,"Hi,I'm server,WELCOM! Ctrl+C to exit");
    wmove(user_list,0,0);
    wprintw(user_list,"On-line User");
    wrefresh(chat);
    wrefresh(user_list_box);
    wrefresh(user_list);



}

/*Функция вывода на экран сообщения. На вход Пид отправителя и текст сообщения*/
void print_msg(char* text, pid_t name) {
    /*Если окно с сообщениями заполненно, то очищаем его и устанавливаем курсор на начало*/
    if (y_ptr_msg>(getmaxy(stdscr)-2)){
        y_ptr_msg=clear_msg_window();
    }
    wmove(chat,y_ptr_msg,1);
    wprintw(chat,"%d:%s",name,text);
    y_ptr_msg ++;
    wrefresh(chat);
    return;

}
/*Функция добавления нового пользователя в чат. На вход ПИД пользователя.
  На выходе его номер в списке текущих пользователй чата*/
int new_user (pid_t name) {
    int r;
    /*Если в чате нету свободных мест, возвращаем -1*/
    if (y_ptr_list>(getmaxy(stdscr)-3)){
        return -1;
    }
    wmove(user_list,y_ptr_list,0);
    wprintw(user_list,"%d",name);
    r=y_ptr_list;
    y_ptr_list++;
    wrefresh(user_list);
    return r;
}

/*Функция очистки окна сообщений*/
int clear_msg_window (void) {
    wclear(chat);
    box(chat,0,0);
    wmove(chat,1,1);
    wprintw(chat,"Hi,I'm server,WELCOM! Ctrl+C to exit");
    wrefresh(chat);
    return 2;

}
/*Функция удаления пользователя. На входе ПИД пользователя*/

void delete_user (pid_t pid_name) {
    int name,i,c,int_name;
    char r[getmaxx(user_list)-2];
    int_name=(int)pid_name; // приводим ПИД к Int (???)
    /*Считываем поочерёдно номера текущих пользователей в чате в строку*/
    for (i=0;i<=(getmaxy(user_list));i++){
        wmove(user_list,i,0);
        for (c=0;c<=getmaxx(user_list);c++){
            r[c]=mvwinch(user_list,i,c);

        }
        name=atoi(r); // приводим строку к INT
        /*если номер совпадает, то удаляем пользователя и подтягиваем список вверх*/
        if (name==int_name) {
            wdeleteln(user_list);
            wrefresh(user_list);
            y_ptr_list--;
            return;

        }
    }

    return;


}

/*Очищаем окно терминала и восстанавливаем настройки*/
void clear_term (void) {
    move(0,0);
    wclear(chat);
    wclear(user_list);
    wclear(user_list_box);
    clear();
    wrefresh(chat);
    wrefresh(user_list);
    wrefresh(user_list_box);
    delwin(chat);
    delwin(user_list);
    delwin(user_list_box);
    echo();
    curs_set(1);
    keypad(stdscr,1);
    refresh();
    touchwin(stdscr);
    delwin(stdscr);
    endwin();



}
