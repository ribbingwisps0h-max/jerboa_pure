//
// Created by Александр Красавцев on 26.03.2026.
//

#ifndef JERBOA_PURE_CACHE_H
#define JERBOA_PURE_CACHE_H

#include <stdbool.h>
#include "db.h"

bool cache_init(const char *host, int port);
// Теперь принимает структуру целиком
void cache_add_to_ark_list(const UserProfile *u);
// Возвращает char*, который нужно будет free() после использования
char* cache_get_ark_list(void);

#endif //JERBOA_PURE_CACHE_H