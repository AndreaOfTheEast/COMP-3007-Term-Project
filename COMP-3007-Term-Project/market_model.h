#pragma once
#include "hinton_market.h"

class NotificationSystem
{
public:
    QSqlDatabase& db;

    NotificationSystem(QSqlDatabase& _db): db(_db){}

    std::vector<std::string> get_notifications(UserId id);
    void add_notification(UserId id, std::string content);
};

class UserSystem
{
public:
    QSqlDatabase& db;

    NotificationSystem *notification_system;

    UserSystem(QSqlDatabase& _db): db(_db){}

    bool get_user(Credentials creds, User *user);
    void add_user(User user);
    void update_user(User user);
};

class MarketDateSystem
{
public:
    QSqlDatabase& db;
    NotificationSystem *notification_system;
    std::vector<MarketDate> market_dates;

    MarketDateSystem(QSqlDatabase& _db): db(_db){}

    void add_market_date(Date date, uint64_t food_limit, uint64_t artisan_limit);
    int make_booking(UserId user, MarketDateId market_date_id);
    void cancel_booking(UserId user, MarketDateId market_date_id);
    bool is_user_booked(UserId user, MarketDateId market_date_id);
    bool has_any_market_dates(void);
};
