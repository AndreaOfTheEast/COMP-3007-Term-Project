#include "mainwindow.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QApplication>
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    UserSystem user_system;
    MarketDateSystem booking_system;

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
    time_t now =  std::time(NULL);
    struct tm *tdy = localtime(&now);
    std::cout<<tdy->tm_mday<<std::endl;

    int daysToAdd = 7 - tdy->tm_wday;
    now+=86400*daysToAdd;
    tdy = localtime(&now);
    std::cout<<tdy->tm_mday<<std::endl;
    MarketDate market_date;
    market_date.artisan_limit = 2;
    market_date.food_limit = 2;
    market_date.date.day = tdy->tm_mday;
    market_date.date.month = tdy->tm_mon;

    booking_system.add_market_date(market_date);

    for(int i = 0; i < 3; i++){
        now+=86400*7;
        tdy = localtime(&now);
        std::cout<<tdy->tm_mday<<std::endl;
        MarketDate market_date;
        market_date.artisan_limit = 2;
        market_date.food_limit = 2;
        market_date.date.day = tdy->tm_mday;
        market_date.date.month = tdy->tm_mon;

        booking_system.add_market_date(market_date);
    }

    now =  std::time(NULL);
    localtime(&now);

    LoginDialog login(&user_system);
    MainWindow w(&user_system, &booking_system);

    while (login.exec() == QDialog::Accepted)
    {
        w.current_user = login.current_user;
        w.handle_dashboard();
        w.show();
        a.exec();
    }
    return 0;
}
