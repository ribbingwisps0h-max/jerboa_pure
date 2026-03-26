//
// Created by Александр Красавцев on 26.03.2026.
//

#include "db.h"
#include <stdlib.h>
#include <string.h>

static PGconn *conn = NULL;

bool db_init(const char *conn_str) {
    conn = PQconnectdb(conn_str);
    if (PQstatus(conn) != CONNECTION_OK) return false;
    return db_create_tables();
}

bool db_create_tables() {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id SERIAL PRIMARY KEY, "
        "first_name VARCHAR(100), "
        "last_name VARCHAR(100), "
        "email VARCHAR(255) UNIQUE NOT NULL, "
        "country VARCHAR(100), "
        "ticket_type VARCHAR(20), "
        "karma INT DEFAULT 0, "
        "percentage FLOAT DEFAULT 0.0, "
        "good_deeds TEXT, "
        "status VARCHAR(20) DEFAULT 'pending', "
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP);";

    PGresult *res = PQexec(conn, sql);
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return success;
}

UserProfile* db_get_user(const char *email) {
    const char *params[1] = {email};
    PGresult *res = PQexecParams(conn,
        "SELECT id, first_name, last_name, email, country, ticket_type, karma, percentage, good_deeds, status "
        "FROM users WHERE email = $1", 1, NULL, params, NULL, NULL, 0);

    if (PQntuples(res) == 0) { PQclear(res); return NULL; }

    UserProfile *u = malloc(sizeof(UserProfile));
    u->id = atoi(PQgetvalue(res, 0, 0));
    strncpy(u->first_name, PQgetvalue(res, 0, 1), 100);
    strncpy(u->last_name, PQgetvalue(res, 0, 2), 100);
    strncpy(u->email, PQgetvalue(res, 0, 3), 255);
    strncpy(u->country, PQgetvalue(res, 0, 4), 100);
    strncpy(u->ticket_type, PQgetvalue(res, 0, 5), 20);
    u->karma = atoi(PQgetvalue(res, 0, 6));
    u->percentage = atof(PQgetvalue(res, 0, 7));
    strncpy(u->good_deeds, PQgetvalue(res, 0, 8), 2048);
    strncpy(u->status, PQgetvalue(res, 0, 9), 20);

    PQclear(res);
    return u;
}

void db_free_profile(UserProfile *ptr) { free(ptr); }