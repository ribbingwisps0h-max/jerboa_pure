//
// Created by Александр Красавцев on 26.03.2026.
//
#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mongoose.h"
#include "db.h"
#include <read.h>

static redisContext *ctx = NULL;

bool cache_init(const char *host, int port) {
    ctx = redisConnect(host, port);
    return (ctx && !ctx->err);
}

void cache_add_to_ark_list(const UserProfile *u) {
    if (!ctx || !u) return;
    // Используем временный буфер для JSON одного пользователя
    char *json = mg_mprintf("{\"n\":\"%s %s\",\"c\":\"%s\",\"k\":%d,\"p\":%.1f}",
                            u->first_name, u->last_name, u->country, u->karma, u->percentage);

    if (json) {
        redisCommand(ctx, "LPUSH ark_survivors %s", json);
        redisCommand(ctx, "LTRIM ark_survivors 0 99");
        free(json); // Освобождаем память после отправки в Redis
    }
}

char* cache_get_ark_list(void) {
    if (!ctx) return strdup("[]");

    redisReply *reply = redisCommand(ctx, "LRANGE ark_survivors 0 -1");
    if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        if (reply) freeReplyObject(reply);
        return strdup("[]");
    }

    char *res = mg_mprintf("[");
    for (size_t i = 0; i < reply->elements; i++) {
        char *old_res = res;
        // Склеиваем: старая строка + текущий элемент + запятая (если не последний)
        res = mg_mprintf("%s%s%s", old_res, reply->element[i]->str,
                         (i < reply->elements - 1) ? "," : "");
        free(old_res);
    }

    char *final_res = mg_mprintf("%s]", res);
    free(res);
    freeReplyObject(reply);

    return final_res; // Эту строку вызывающий код должен будет free()
}