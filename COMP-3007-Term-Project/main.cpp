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
    BookingSystem booking_system;

    uint64_t i;

    // HARD CODED USERS
    USER_TYPE user_types[] = {USER_TYPE_FOOD, USER_TYPE_FOOD, USER_TYPE_FOOD, USER_TYPE_FOOD, USER_TYPE_ARTISAN, USER_TYPE_ARTISAN, USER_TYPE_ARTISAN, USER_TYPE_ARTISAN};
    std::string usernames[] = {"burgers", "bubbletea", "jamsnhoney", "pancakes", "candles", "fiber", "computers", "artwork"};
    std::string buisness_names[] = {"burgers", "bubbletea", "jamsnhoney", "pancakes", "candles", "fiber", "computers", "artwork"};
    std::string phone_numbers[] = {"000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000", "000-000-0000"};
    std::string owner_names[] = {"John", "James", "Jo", "Anne", "Kirk", "David", "Marvin", "Abalone"};
    std::string emails[] = {"burgers@gmail.com", "bubbletea@gmail.com", "jamsnhoney@gmail.com", "pancakes@gmail.com", "candles@gmail.com", "fiber@gmail.com", "computers@gmail.com", "artwork@gmail.com"};
    std::string mail_addresses[] = {"ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123", "ABC 123"};


    for (i = 0; i < 8; i++) {
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
    for (i = 0; i < 4; i++) {
        Booking booking;
        booking.artisan_limit = 2;
        booking.food_limit = 2;
        booking.date.day = i;

        booking_system.add_booking(booking);
    }

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
