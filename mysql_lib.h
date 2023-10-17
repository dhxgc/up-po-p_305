#include <mysql/mysql.h>
#include <mysql/my_global.h>
#include <mysql/mysqld_error.h>
#include <mysql/errmsg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

MYSQL* mysql;
MYSQL_RES* res;
MYSQL_ROW row;
MYSQL_FIELD* field;
int i;

void error_info () {
        printf("Error connect\n");
        printf("error N: %i %s\n", 
            mysql_errno(mysql), mysql_error(mysql));
}

void show_result () {
    res = mysql_store_result(mysql); // ???
    if (res == NULL)
        printf ("\n\tres == NULL\t Error get data!\n");
    else {
        if (mysql_num_rows(res) > 0) {
            printf ("\n\tResult: %lli row\n",
            mysql_num_rows(res));
            i = 0;
            while (i < mysql_num_fields(res)) {
                field = mysql_fetch_field_direct(res, i);
                printf ("\n\t%s\t", field->name);
                i++; // ВЫВОД ???????? ПО-МОЕМУ ЗДЕСЬ ВЫВОДЯТ ИЛИ
                //ПРИМЕРНЫЙ ЗАПРОС ИЛИ В КАКОЙ ДБ МЫ СЕЙЧАС РАБОТАЕМ
            }
            printf ("\n\t-----------\n");
            while ((row = mysql_fetch_row(res)) != NULL) {
/*Выбирает следующую строку в результате запроса. Возвратит пустой указатель, когда все строки были получены.*/
                for (i = 0; i < mysql_num_fields(res); i++) // Возвращает количество столбцов (полей) в результате запроса.
                {
                    printf ("\t%s\t", row[i]);
                }
                printf ("\n");
            }
            mysql_free_result(res);
        }
        else {
            printf ("\n\t No Data! code = -sr\n");
        }

        printf ("\n\tEND OF THE QUERY\n\n");
        sleep(5);
        system("clear");
    }
}

void init_mysql () {
    mysql = mysql_init(NULL);
    if (mysql == NULL) printf ("\n\tError init!\n"); 
    else printf ("\n\tinit OK!\n");
}


/// @brief подключение к нужной БД
void connect_mysql () {
    if (!mysql_real_connect(mysql,
                    "localhost",
                    "vscode",
                    "trfnbv12",
                    "userks53",
                    0,
                    NULL,
                    0
                    )) error_info();
    else printf ("\tConnection OK!\n");
}

void __mysql_query(const char query[]) {
    if (mysql_query(mysql, query) != 0) {
        error_info();
    } 
    else {
        show_result();
    }
}