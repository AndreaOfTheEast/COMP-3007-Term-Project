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

    uint64_t n = 0;

    FoodVendor food_vendors[4];
    food_vendors[n].id = {user_system.users.size()};
    food_vendors[n].creds = {"foodytownfoods"};
    food_vendors[n].business_name = "Foody Town Foods";
    food_vendors[n].owner_name = "David Davidson";
    food_vendors[n].email = "foodytownfoods@food.net";
    food_vendors[n].mail_address = "A1C 2B3";
    food_vendors[n].compliance_docs.information = "COMPLIANCE_DOC0.PDF";
    user_system.add_user(food_vendors[n]);
    n++;

    food_vendors[n].id = {user_system.users.size()};
    food_vendors[n].creds = {"burgershop"};
    food_vendors[n].business_name = "Burger Shop";
    food_vendors[n].owner_name = "Joe Chang";
    food_vendors[n].email = "burgers4you@mail.net";
    food_vendors[n].mail_address = "B3F 9D2";
    food_vendors[n].compliance_docs.information = "COMPLIANCE_DOC1.PDF";
    user_system.add_user(food_vendors[n]);
    n++;

    food_vendors[n].id = {user_system.users.size()};
    food_vendors[n].creds = {"bubbleteas"};
    food_vendors[n].business_name = "Bubble Teas";
    food_vendors[n].owner_name = "Gordon Ramsay";
    food_vendors[n].email = "Bbteas@gmail.com";
    food_vendors[n].mail_address = "3F3 4C2";
    food_vendors[n].compliance_docs.information = "COMPLIANCE_DOC2.PDF";
    user_system.add_user(food_vendors[n]);
    n++;

    food_vendors[n].id = {user_system.users.size()};
    food_vendors[n].creds = {"smaakencrackstation"};
    food_vendors[n].business_name = "Smaakencrack Station";
    food_vendors[n].owner_name = "Erikson Smaaldan";
    food_vendors[n].email = "smaakenstation@gmail.com";
    food_vendors[n].mail_address = "A1C 2B3";
    food_vendors[n].compliance_docs.information = "COMPLIANCE_DOC3.PDF";
    user_system.add_user(food_vendors[n]);
    n++;

    ArtisanVendor artisan_vendors[4];

    n = 0;
    artisan_vendors[n].id = {user_system.users.size()};
    artisan_vendors[n].creds = {"candlesforyourlove"};
    artisan_vendors[n].business_name = "Candles for Your Love";
    artisan_vendors[n].owner_name = "Lucy Madison";
    artisan_vendors[n].email = "candlesxxx@mail.web";
    artisan_vendors[n].mail_address = "D3D D3D";
    artisan_vendors[n].compliance_docs.information = "COMPLIANCE_DOC4.PDF";
    user_system.add_user(artisan_vendors[n]);
    n++;

    artisan_vendors[n].id = {user_system.users.size()};
    artisan_vendors[n].creds = {"computerpartsrepair"};
    artisan_vendors[n].business_name = "Computer Parts Repair";
    artisan_vendors[n].owner_name = "Joe Blake";
    artisan_vendors[n].email = "comppart@gmail.com";
    artisan_vendors[n].mail_address = "X3L 3L2";
    artisan_vendors[n].compliance_docs.information = "COMPLIANCE_DOC5.PDF";
    user_system.add_user(artisan_vendors[n]);
    n++;

    artisan_vendors[n].id = {user_system.users.size()};
    artisan_vendors[n].creds = {"jamsnhoney"};
    artisan_vendors[n].business_name = "Jams 'n Honey";
    artisan_vendors[n].owner_name = "Marc McLaughlin";
    artisan_vendors[n].email = "jamsandhoney@gmail.com";
    artisan_vendors[n].mail_address = "V3X P3J";
    artisan_vendors[n].compliance_docs.information = "COMPLIANCE_DOC6.PDF";
    user_system.add_user(artisan_vendors[n]);
    n++;

    artisan_vendors[n].id = {user_system.users.size()};
    artisan_vendors[n].creds = {"fibersoftheweed"};
    artisan_vendors[n].business_name = "Fibers of the Weed";
    artisan_vendors[n].owner_name = "San Chi";
    artisan_vendors[n].email = "clothesandfibers@gmail.com";
    artisan_vendors[n].mail_address = "L3P CV2";
    artisan_vendors[n].compliance_docs.information = "COMPLIANCE_DOC7.PDF";
    user_system.add_user(artisan_vendors[n]);


    Operator operators[1];

    n = 0;
    operators[n].id = {user_system.users.size()};
    operators[n].creds = {"bigbossman"};
    operators[n].business_name = "";
    operators[n].owner_name = "BigBoss Man";
    operators[n].email = "BigBoss@gmail.com";
    operators[n].mail_address = "K1R D93";
    operators[n].compliance_docs.information = "";
    user_system.add_user(operators[n]);

    Admin admins[1];
    admins[n].id = {user_system.users.size()};
    admins[n].creds = {"biggerbossman"};
    admins[n].business_name = "";
    admins[n].owner_name = "BiggerBoss Man";
    admins[n].email = "BiggerBoss@gmail.com";
    admins[n].mail_address = "K1R D93";
    admins[n].compliance_docs.information = "";
    user_system.add_user(admins[n]);

    LoginDialog login(&user_system);

    if(1)
    {
        login.exec();
        MainWindow w(&user_system);
        w.show();
        return a.exec();
    }

    return 0;
}
