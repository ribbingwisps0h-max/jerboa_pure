🛸 Jerboa Pure: Digital Ark
Jerboa Pure — это высокопроизводительный бэкенд на языке C для проекта «Цифровой Ковчег». Система предназначена для регистрации пользователей, анализа их «кармы» и визуализации глобального списка выживших.

🏗 Архитектура системы
Проект построен на базе микросервисной архитектуры в Docker:

Nginx: Reverse-proxy, SSL-терминация (HTTPS) и отдача статики.

C-Backend (Mongoose): Ядро системы. Обработка API, парсинг JSON, бизнес-логика.

PostgreSQL 18: Основное хранилище (пользователи, транзакции, добрые дела).

Redis 7: Кэширование «горячих» данных (Топ-100 выживших) и статусы оплат.

📁 Структура модулей (C-Code)
Проект строго разделен на модули для обеспечения масштабируемости:
Файл,Описание
main.c,"Инициализация серверов, маршрутизация HTTP запросов (Mongoose)."
db.c / .h,"Слой данных. Авто-создание таблиц, SQL-запросы (libpq)."
cache.c / .h,Слой кэширования. Работа со списками и счетчиками (hiredis).
payment.c / .h,"Логика обработки билетов (Silver, Gold, Platinum) и Webhooks."
mongoose.c / .h,Встраиваемый веб-сервер (Cesanta Mongoose).

🚀 Быстрый старт (Production)
Настройте окружение: Создайте файл .env на основе примера:

Code snippet
POSTGRES_DB=jerboa
POSTGRES_USER=aleksandr
POSTGRES_PASSWORD=your_secure_pass
DB_URL=host=db dbname=jerboa user=aleksandr password=your_secure_pass
Запустите контейнеры:

Bash
docker compose up -d --build
SSL (Let's Encrypt): При первом запуске выполните команду генерации сертификата (см. документацию в docker-compose.yml).

🛠 Разработка (Development)
Для работы в CLion используйте docker-compose.dev.yml.

Интерфейс: API слушает на 0.0.0.0:8000.

База данных: Доступна локально на порту 5432.

Redis: Доступен локально на порту 6379.

🛰 API Endpoints
POST /api/v1/register — Регистрация (Email, Имя, Фамилия, Страна, Тип билета).

GET /api/v1/profile?email=... — Получение данных профиля и Кармы.

GET /api/v1/ark/icons — Быстрый список (Топ-100) из Redis.

POST /api/v1/payment/webhook — Подтверждение оплаты и активация профиля.

🛡 Безопасность
SQL Injection: Все запросы используют PQexecParams (Prepared Statements).

Memory Safety: Динамическое выделение строк через mg_mprintf с обязательным free().

Isolation: Бэкенд скрыт внутри Docker-сети и доступен только через Nginx.