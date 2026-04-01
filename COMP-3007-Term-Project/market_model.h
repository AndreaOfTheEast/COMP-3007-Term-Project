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
    QSqlDatabase db;

    UserSystem(
            void)
    {
        db = QSqlDatabase::addDatabase("QSQLITE");

        QString dbPath =
            QCoreApplication::applicationDirPath()
            + "COMP-3007-Term-Project/hintonMarket.db";
            // andwu: TODO: is this the right path?
        db.setDatabaseName(dbPath);

        if(!db.open())
        {
            fprintf(stderr, "DB: %s", db.lastError().text().toStdString().c_str());
            return;
        }
    }

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
    int64_t is_user_booked(UserId user, MarketDateId market_date_id);
};
