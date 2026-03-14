#pragma once
#include "hinton_market.h"

class NotificationSystem
{
public:
    std::vector<Notification> notifications;

    std::vector<std::string> get_notifications(UserId id);
    void add_notification(UserId id, std::string content);
};

class UserSystem
{
public:
    NotificationSystem *notification_system;
    std::vector<User> users;

    User *get_user(Credentials creds);
    void add_user(User user);
};

class MarketDateSystem
{
public:
    NotificationSystem *notification_system;
    std::vector<MarketDate> market_dates;

    void add_market_date(MarketDate market_date);
    int make_booking(User *user, uint64_t market_date_index);
    void cancel_booking(User *user, uint64_t market_date_index);
    int64_t is_user_booked(User *user, uint64_t market_date_index);
};
