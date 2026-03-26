#include "mongoose.h"
#include "db.h"
#include "cache.h"

void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        // Эндпоинт получения профиля
        if (struct mg_http_header(hm->uri, "/api/v1/profile")) {
            struct mg_str *email_query = mg_http_get_header_var(&hm->query, "email");
            if (!email_query) {
                mg_http_reply(c, 400, "", "{\"error\":\"email_required\"}");
                return;
            }

            char email[255];
            snprintf(email, sizeof(email), "%.*s", (int)email_query->len, email_query->ptr);

            UserProfile *u = db_get_user(email);
            if (!u) {
                mg_http_reply(c, 404, "", "{\"error\":\"not_found\"}");
            } else {
                mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{\"first_name\":\"%s\",\"last_name\":\"%s\",\"karma\":%d,\"percentage\":%.2f,\"ticket\":\"%s\"}",
                    u->first_name, u->last_name, u->karma, u->percentage, u->ticket_type);
                db_free_profile(u);
            }
        }
    }
}

int main() {
    // Инициализация из .env через Docker окружение
    if (!db_init(getenv("DB_URL"))) return 1;
    if (!cache_init("redis", 6379)) return 1;

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, NULL);

    printf("Jerboa Pure: Digital Ark is online.\n");
    for (;;) mg_mgr_poll(&mgr, 1000);
    return 0;
}