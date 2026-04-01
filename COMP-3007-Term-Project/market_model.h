#pragma once
#include "hinton_market.h"

class NotificationSystem
{
public:
    std::vector<std::string> get_notifications(UserId id);
    void add_notification(UserId id, std::string content);
};

class UserSystem
{
public:
    NotificationSystem *notification_system;

    bool get_user(Credentials creds, User *user);
    void add_user(User user);
};

class MarketDateSystem
{
public:
    NotificationSystem *notification_system;
    std::vector<MarketDate> market_dates;

    void add_market_date(Date date, uint64_t food_limit, uint64_t artisan_limit);
    int make_booking(UserId user, MarketDateId market_date_id);
    void cancel_booking(UserId user, MarketDateId market_date_id);
    bool is_user_booked(UserId user, MarketDateId market_date_id);
};
