#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int mainwhile ();

void theFood(int *stud);
int theBorders(int *x, int *y, int *COLLISIONS);
void theRandomFood (int *x, int *y, bool *checkFood);

int main () {
    initscr();

    mainwhile();

    endwin();
    return 0;
}

int mainwhile () {
    srand (time(NULL));
    char field [10][31];
    int ch_move;
    keypad(stdscr, true);
    int x = 1, y = 1, sx = 5, sy = 5, collisions = 0, students = 0;
    bool checkFood;
    
    move (0, 0);
    printw ("Welcome to the UP-PO-P game!");

    while (ch_move != KEY_F(6)) {
        srand (time(NULL));
        move(1, 0);
        strcpy(field[0], "##############################");
        for (int i = 1; i < 9; i++) strcpy (field[i], "#                            #");
        strcpy(field[9], "##############################");

//здесь должна быть функция рандомайзера еды (точнее расположения)
        //theRandomFood(&sx, &sy, &checkFood);

        field[x][y] = 'R';
        field[sx][sy] = 's';

        for (int i = 0; i < 10; i++) {printw("%s\n", field[i]);
        refresh();}

        ch_move = getch();

        switch (ch_move){
            case KEY_DOWN:
                x++;
                break;
            case KEY_RIGHT:
                y++;
                break;
            case KEY_LEFT:
                y--;
                break;
            case KEY_UP:
                x--;
                break;
            case KEY_F(6):
                return 1;
                break;
            default:
                break;
        }

        if (field[x][y] == '#')
            if (theBorders(&x, &y, &collisions) == -1) return 1;

        if ((x == sx) && (y == sy)) {//if (field[x][y] == field[sx][sy])
            move(0, 0);
            clrtoeol();
            sx = rand()%(7-2+1)+2;
            sy = rand()%(27-2+1)+2;
            students++;
            printw ("Good prepod! u eat another one student...");
            refresh();
        }

        move (13, 0);
        printw ("collisions = %d\nstudent = %d\ns = [%d]:[%d]\nR = [%d][%d]", collisions, students, sx, sy, x, y);
        // move (1, 0);
        // for (int i = 0; i < 11; i++) clrtoeol();//отчистка поля
    }
    keypad(stdscr, false);
    return 1;
}

void theFood (int *stud) {
    move(0, 0);
    clrtoeol();
    *stud+=1;
    //*checkFood = true;
    printw ("Good prepod! u eat another one student...");
    refresh();
}

int theBorders (int *x, int *y, int *COLLISIONS) {
    move (0, 0);
    clrtoeol();
    *x = 1; *y = 1;
    *COLLISIONS+=1;
    printw ("Dont Move On #! +1 coll");
    refresh();
    if (*COLLISIONS == 3) return -1;
    else return 1;
}

void theRandomFood (int *x, int *y, bool *checkFood) {
    srand(time(NULL));
    *y = rand()%(7-2+1)+2;
    *x = rand()%(27-2+1)+2;
    *checkFood = false;
}