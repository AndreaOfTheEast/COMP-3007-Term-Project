#include "market.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NotificationSystem notification_system;
    UserSystem user_system;
    user_system.notification_system = &notification_system;
    MarketDateSystem market_date_system;
    market_date_system.notification_system = &notification_system;

    uint64_t i;

    // HARD CODED USERS
    USER_TYPE user_types[] = {USER_TYPE_FOOD, USER_TYPE_FOOD, USER_TYPE_FOOD, USER_TYPE_FOOD, USER_TYPE_ARTISAN, USER_TYPE_ARTISAN, USER_TYPE_ARTISAN, USER_TYPE_ARTISAN, USER_TYPE_OPERATOR, USER_TYPE_ADMIN};
    std::string usernames[] = {"burgers", "bubbletea", "jamsnhoney", "pancakes", "candles", "fiber", "computers", "artwork", "operator", "admin"};
    std::string buisness_names[] = {"burgers", "bubbletea", "jamsnhoney", "pancakes", "candles", "fiber", "computers", "artwork", "", ""};
    std::string phone_numbers[] = {"000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000"};
    std::string owner_names[] = {"John", "James", "Jo", "Anne", "Kirk", "David", "Marvin", "Abalone", "Bonnie", "Clyde"};
    std::string emails[] = {"burgers@gmail.com", "bubbletea@gmail.com", "jamsnhoney@gmail.com", "pancakes@gmail.com", "candles@gmail.com", "fiber@gmail.com", "computers@gmail.com", "artwork@gmail.com", "bonnie.op@gmail.com", "clyde.ad@gmail.com"};
    std::string mail_addresses[] = {"ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123"};


    for (i = 0; i < 10; i++) {
        User user;
        user.id = {i};
        user.perms.user_type = user_types[i];
        user.creds = {usernames[i]};
        user.business_name = buisness_names[i];
        user.phone_number = phone_numbers[i];
        user.owner_name = owner_names[i];
        user.email = emails[i];
        user.mail_address = mail_addresses[i];
        user.compliance_docs = {{"a", "a"}, {"a", "a", "a"}, {"a", "a"}};
        user_system.add_user(user);
    }

    // HARD CODED MARKET DATES
    time_t now = std::time(NULL);
    struct tm *sunday = localtime(&now);

    int nearest_sunday = 7 - sunday->tm_wday;
    now += 86400 * nearest_sunday;

    for (i = 0; i < 8; i++) {
        sunday = localtime(&now);
        MarketDate market_date;
        market_date.date.day = (uint64_t)sunday->tm_mday;
        market_date.date.month = (uint64_t)sunday->tm_mon;
        market_date.date.year = (uint64_t)sunday->tm_year;

        market_date.artisan_booking.limit = 2;
        market_date.food_booking.limit = 2;

        market_date_system.add_market_date(market_date);
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
