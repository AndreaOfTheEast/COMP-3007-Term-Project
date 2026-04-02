#include "market.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // start the database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath =
        QCoreApplication::applicationDirPath()
        + "/hintonMarket.db";
        // andwu: TODO: is this the right path?
    db.setDatabaseName(dbPath);

    if(!db.open())
    {
        fprintf(stderr, "DB: %s \n", db.lastError().text().toStdString().c_str());
        Assert(0, "database failed to start");
    }

    NotificationSystem notification_system(db);
    UserSystem user_system(db);
    user_system.notification_system = &notification_system;
    MarketDateSystem market_date_system(db);
    market_date_system.notification_system = &notification_system;

//    Assert(0, "andwu: TODO: 'cp hintonMarket_original.db hintonMarket.db'");

    // HARD CODED MARKET DATES
    time_t now = std::time(NULL);
    struct tm *sunday = localtime(&now);

    int nearest_sunday = 7 - sunday->tm_wday;
    now += 86400 * nearest_sunday;

    for (int i = 0; i < 8; i++) {
        sunday = localtime(&now);
        Date date;
        date.day = (uint64_t)sunday->tm_mday;
        date.month = (uint64_t)sunday->tm_mon;
        date.year = (uint64_t)sunday->tm_year;

        uint64_t artisan_limit = 2;
        uint64_t food_limit = 2;

        market_date_system.add_market_date(date, artisan_limit, food_limit);
        now += 86400 * 7;
    }

    LoginDialog login(&user_system);
    Market w(&user_system, &market_date_system, &notification_system);

    while (login.exec() == QDialog::Accepted)
    {
        w.current_user = login.current_user;
        w.handle_dashboard();
        w.show();
        a.exec();
        login.user_text_field->clear();
    }
    return 0;
}
