//
// Created by Александр Красавцев on 26.03.2026.
//

#ifndef JERBOA_PURE_DB_H
#define JERBOA_PURE_DB_H

#include <libpq-fe.h>
#include <stdbool.h>

typedef struct {
    int id;
    char first_name[100];
    char last_name[100];
    char email[255];
    char country[100];
    char ticket_type[20]; // silver, gold, platinum
    int karma;
    double percentage;     // Процент вклада (для AI анализа)
    char good_deeds[2048];
    char status[20];       // pending, paid
} UserProfile;

bool db_init(const char *conn_str);
bool db_create_tables(); // Авто-создание таблиц
UserProfile* db_get_user(const char *email);
bool db_register_user(UserProfile *u);
void db_free_profile(UserProfile *ptr);

#endif //JERBOA_PURE_DB_H