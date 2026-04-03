#include "market.h"
#include <sstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "ui_market.h"
#include <QDebug>
#pragma GCC diagnostic pop



Market::Market(UserSystem *in_user_system, MarketDateSystem *in_market_date_system, NotificationSystem *in_notification_system, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Market)
    , user_system(in_user_system)
    , market_date_system(in_market_date_system)
    , notification_system(in_notification_system)
{
    ui->setupUi(this);

    // Set up date
    current_date = Date::get_now();
    connect(ui->edit_user_information, &QPushButton::clicked, this, [=]{
        handle_edit_information();
    });

    connect(ui->save_user_info, &QPushButton::clicked, this, [=]{
        save_user_information();
    });

    connect(ui->return_user_info, &QPushButton::clicked, this, [=]{
        handle_dashboard();
        ui->edit_email->clear();
        ui->edit_mailing->clear();
        ui->edit_phone->clear();
        ui->edit_owner->clear();
        ui->edit_business->clear();

        ui->business_exp->clear();

        ui->business_number->clear();

        ui->liability_number->clear();

        ui->liability_exp->clear();

        ui->liability_provider->clear();

        ui->business_number->clear();

        ui->food_number->clear();

        ui->food_exp->clear();
    });

    // Navigate to the Market Schedule
    connect(ui->browse_market, &QPushButton::clicked, this, [=]{
        handle_market_schedule();
    });

    // Navigate to the dashboard
    connect(ui->back_to_dashboard, &QPushButton::clicked, this, [=]{
        handle_dashboard();
    });
    connect(ui->operator_back_to_dashboard, &QPushButton::clicked, this, [=] {
        handle_dashboard();
    });
    connect(ui->operator_back_to_dashboard_2, &QPushButton::clicked, this, [=] {
        handle_dashboard();
    });

    // Make a booking
    connect(ui->make_booking, &QPushButton::clicked, this, [=]{
        uint64_t index = (uint64_t)ui->table_market_dates->currentRow();
        MarketDateId market_date_id;
        market_date_id.id = (uint64_t)ui->table_market_dates->item((int)index, 1);
        std::string mdate = ui->table_market_dates->item((int)index, 0)->text().toStdString();
        std::string temp = "";
        temp += mdate[0];
        temp += mdate[1];
        int month = std::stoi(temp);
        temp = "";
        temp += mdate[3];
        temp += mdate[4];
        int day = std::stoi(temp);
        temp = "";
        temp += mdate[8];
        temp += mdate[9];
        int year = std::stoi(temp);

        int id = -1;
        std::string query_string;
        QSqlQuery query;
        query_string = std::string("SELECT id FROM market_dates where year = :year and month = :month and day = :day;");
        query.prepare(QString(query_string.c_str()));

        query.bindValue(":year", QString::fromStdString(std::to_string(year+100)));
        query.bindValue(":month", QString::fromStdString(std::to_string(month)));
        query.bindValue(":day", QString::fromStdString(std::to_string(day-1)));
        Assert(query.exec(), "Query for id fail");
        while(query.next()){
            id = query.value("id").toInt();
        }
        if(id == -1){
            return;
        }
        market_date_id.id = (uint64_t)id;
        QMessageBox msgBox;

        if (ui->table_market_dates->selectedItems().isEmpty())
        {
            return;
        }

        if(market_date_system->is_user_booked(current_user->id, market_date_id))
        {
            QMessageBox::warning(
                        this,
                        "Booking Failed",
                        "You already booked for this date.");
            return;
        }

        User *user = current_user;
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ADMIN)
        {
            QMessageBox::warning(
                        this,
                        "Booking Failed",
                        "Your account prohibits booking directly.",
                        QMessageBox::Ok);
        }
        else if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_OPERATOR)
        {
            std::string username = ui->user_list_market->currentItem()->text().toStdString();
            Credentials creds = { username };
            User temp_user;
            bool ok = user_system->get_user(creds, &temp_user);
            if(!ok) { user = 0; }
            else {
                user = &temp_user;
                query_string = std::string("SELECT user_id FROM users where username = :username;");
                query.prepare(QString(query_string.c_str()));

                query.bindValue(":username", QString::fromStdString(username));
                Assert(query.exec(), "Query for User id fail");
                while(query.next()){
                    temp_user.id.id = (uint64_t)query.value("user_id").toInt();
                }
            }
            if(market_date_system->is_user_booked(temp_user.id, market_date_id))
            {
                QMessageBox::warning(
                            this,
                            "Booking Failed",
                            "You already booked for this date.");
                return;
            }
        }

#if 0
        if (user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            Assert(0, "TODO: ANDERWERWERWEWER database retrieve, artisan_booking for a market date");
            // vector = &market_date_system->market_dates[index].artisan_booking.users;
        }
        else if (user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            Assert(0, "TODO: ANDREWEWENEWWNENWE database retrieve, food_booking for a market date");
            // TODO: database retrieve andrew
            // vector = &market_date_system->market_dates[index].food_booking.users;
        }

        Assert(0, "TODO:ANDREWEWENEWWNENWE figure out if userslist are null");
        if (/*vector == nullptr*/0)
        {
            msgBox.setText(QString("Your account type prohibits booking directly."));
            msgBox.exec();
            return;
        }
#endif

        QMessageBox::StandardButton question;
        question = QMessageBox::question(
                    this,
                    "Confirm Action",
                    "Are you sure want to book this date?",
                    QMessageBox::Yes | QMessageBox::No);
        if (question == QMessageBox::Yes)
        {
            std::string username;
            if (ui->user_list_market->currentItem() != nullptr)
            {
                username = ui->user_list_market->currentItem()->text().toStdString();
            }
            else if(current_user->perms.user_type == (USER_TYPE)USER_TYPE_ADMIN || current_user->perms.user_type == (USER_TYPE)USER_TYPE_OPERATOR)
            {
                QMessageBox::warning(
                            this,
                            "Booking Failed",
                            "No selected user.",
                            QMessageBox::Ok);
            }

            market_date_system->make_booking(user->id, market_date_id);


            // Add notification for the operator if
            // they made the booking
            if(current_user->perms.user_type == USER_TYPE_OPERATOR)
            {
                std::stringstream notification_msg;
                notification_msg << "[Action] Booked "
                                 << ui->table_market_dates->item((int32_t)index, 0)->text().toStdString()
                                 << " for " << user->creds.username << ".";
                notification_system->add_notification(current_user->id, notification_msg.str());
            }
        }
        else
        {
            market_date_system->make_booking(user->id, market_date_id);
        }
        display_market_information(ui->table_market_dates, user);
    });

    // Cancel booking
    connect(ui->cancel_booking, &QPushButton::clicked, this, [=]{
        QMessageBox::StandardButton question;
        QString msg;

        int64_t index = (int64_t)ui->table_market_dates->currentRow();

        if (ui->table_market_dates->currentRow() < 0)
        {
            return;
        }

        MarketDateId market_date_id;
        QTableWidgetItem *market_id_widget = ui->table_market_dates->item((int)index, 1);
        market_date_id.id = (uint64_t)market_id_widget->text().toInt(); // COLUMN 1 = IDs

        if (ui->table_market_dates->selectedItems().isEmpty())
        {
            return;
        }

        std::string date = market_date_system->date_str_from_id(market_date_id);
        // TODO: if(date == "") { }

        bool is_booked = market_date_system->is_user_booked(current_user->id, market_date_id);

        if(current_user->perms.user_type != USER_TYPE_FOOD &&
           current_user->perms.user_type != USER_TYPE_ARTISAN &&
           current_user->perms.user_type != USER_TYPE_OPERATOR)
        {
            QMessageBox::warning(
                        this,
                        "Cancellation Failed",
                        "Your account type prohibits cancellation.");
            return;
        }

        // Prompt question
        msg = QString("Are you sure you want to cancel this booking for this date (%1)?")
                .arg(date.c_str());
        question = QMessageBox::question(
                    this,
                    "Confirm Action",
                    msg,
                    QMessageBox::Yes | QMessageBox::No);

        if (question != QMessageBox::Yes) { return; }

        market_date_system->cancel_booking(current_user->id, market_date_id);

        msg = QString("Successfully cancelled booking for %1.")
                .arg(date.c_str());
        QMessageBox::information(
                    this,
                    "Cancellation Success",
                    msg);

        handle_market_schedule();
    });

    // Add selection exclusion
    connect(ui->user_booking_list, &QListWidget::itemClicked, this, [=] {
        ui->user_waitlist_list->clearSelection();
        ui->user_waitlist_list->setCurrentItem(nullptr);
        ui->cancel_booking_waitlist->setText("Cancel Booking");
    });
    connect(ui->user_waitlist_list, &QListWidget::itemClicked, this, [=] {
        ui->user_booking_list->clearSelection();
        ui->user_booking_list->setCurrentItem(nullptr);
        ui->cancel_booking_waitlist->setText("Cancel Waitlist");
    });


    // OPERATOR - See selected user information
    connect(ui->user_list, &QListWidget::itemClicked, this, [=] {

        // Clear Information
        ui->user_information_view->clear();
        ui->user_booking_list->clear();
        ui->user_waitlist_list->clear();


        std::string username;

        if (QListWidgetItem *item = ui->user_list->currentItem())
        {
            username = item->text().toStdString();
        }
        else
        {
            return;
        }

        User *user = 0;
        Credentials creds = { username };
        User temp_user;
        bool ok = user_system->get_user(creds, &temp_user);
        if(ok) { user = &temp_user; }

        // Display account information
        display_account_information(ui->user_information_view, user);

        // Active bookings
//        Assert(0, "TODO: query all our bookings from the db");

        std::vector<std::string> selected_user_bookings; // dates
        std::vector<std::string> selected_user_waitlists; // dates

        ui->user_booking_list->clear();
        QSqlQuery query;
        std::vector<int> years;
        std::vector<int> months;
        std::vector<int> days;
        std::string query_string = "SELECT for_day,for_month,for_year FROM bookings WHERE user_id = :id AND is_waitlist = 0;";
        query.prepare(QString(query_string.c_str()));
        query.bindValue(":id", QString::fromStdString(std::to_string(temp_user.id.id)));
        Assert(query.exec(), "Failed query");

        while(query.next()){
            years.push_back(query.value("for_year").toInt()+1900);
            months.push_back(query.value("for_month").toInt());
            days.push_back(query.value("for_day").toInt()+1);
        }

        for(size_t i = 0; i < (size_t)years.size(); i++){
            std::string toAdd = "";
            toAdd += std::to_string(temp_user.id.id);
            toAdd += ": ";
            toAdd += std::to_string(months[i]);
            toAdd += "/";
            toAdd += std::to_string(days[i]);
            toAdd += "/";
            toAdd += std::to_string(years[i]);
            ui->user_booking_list->addItem(toAdd.c_str());
        }
    });

    // OPERATOR - Cancel a booking or waitlist position for a vendor
    connect(ui->cancel_booking_waitlist, &QPushButton::clicked, this, [=] {
        std::string username;
        uint8_t is_waitlist = 0;
        std::vector<std::string> str = { "booking", "waitlist" };

        if (current_user->perms.user_type != USER_TYPE_OPERATOR)
        {
            QMessageBox::warning(
                        this,
                        "Cancellation Failed",
                        "Your account prohibits booking cancellation.",
                        QMessageBox::Ok);
            return;
        }

        if (QListWidgetItem *item = ui->user_list->currentItem())
        {
            username = item->text().toStdString();
        }
        else
        {
            QMessageBox::warning(
                        this,
                        "Cancellation Failed",
                        "This user does not exist.",
                        QMessageBox::Ok);
            return;
        }

        QString date;
        MarketDateId market_date_id;

        User *user = 0;
        Credentials creds = { username };
        User temp_user;
        bool ok = user_system->get_user(creds, &temp_user);
        if(ok) { user = &temp_user; }

        if (!ui->user_booking_list->selectedItems().isEmpty())
        {
            QString s = ui->user_booking_list->currentItem()->text();
            int i = s.indexOf(':');
            Assert(i != -1, "dates should have ids");
            market_date_id = MarketDateId{ (uint64_t)s.left(i).toInt() };

            i += 2;
            date = s.mid(i, s.size() - i);
        }
        else if (!ui->user_waitlist_list->selectedItems().isEmpty())
        {
            QString s = ui->user_waitlist_list->currentItem()->text();
            int i = s.indexOf(':');
            Assert(i != -1, "dates should have ids");
            market_date_id = MarketDateId{ (uint64_t)s.left(i).toInt() };

            i += 2;
            int j = s.indexOf('(');
            if(j < 0) { j = s.size(); }
            j -= i;
            date = s.mid(i, j);

            is_waitlist = 1;
        }
        else
        {
            return;
        }

        bool date_real = market_date_system->is_date_real(market_date_id);

        // User booking/waitlist is not found
        if (date_real)
        {
            QMessageBox::warning(
                        this,
                        "Cancellation Failed",
                        QString("This booking/waitlist (%1) does not exist.")
                            .arg(date),
                        QMessageBox::Ok);
            return;
        }

        // Cancel booking
        market_date_system->cancel_booking(user->id, market_date_id);

        QMessageBox::information(
                    this,
                    "Cancellation Sucess",
                    QString("Cancelled %1 on %2 for %3.")
                        .arg(str[is_waitlist].c_str())
                        .arg(date)
                        .arg(username.c_str()),
                    QMessageBox::Ok);

        // Remove the item from the list.
        QListWidgetItem *item;
        if (is_waitlist == 1)
        {
            item = ui->user_waitlist_list->currentItem();
            delete ui->user_waitlist_list->takeItem(ui->user_waitlist_list->row(item));
        }
        else
        {
            item = ui->user_booking_list->currentItem();
            delete ui->user_booking_list->takeItem(ui->user_booking_list->row(item));
        }

        // Notification for the operator
        std::stringstream notification_msg;
        notification_msg << "[Action] Cancelled " << str[is_waitlist] << " on "
                         << date.toStdString() << " for " << username << ".";
        notification_system->add_notification(current_user->id, notification_msg.str());
    });

    // OPERATOR - Display user's market schedule view
    connect(ui->user_list_market, &QListWidget::itemClicked, this, [=]{
        std::string username = ui->user_list_market->currentItem()->text().toStdString();
        Credentials creds = { username };
        User temp_user;
        bool ok = user_system->get_user(creds, &temp_user);
        if(ok)
        {
            display_market_information(ui->table_market_dates, &temp_user);
        }
        else
        {
            QMessageBox::warning(
                        this,
                        "Failed to find user to display market",
                        QString("The user did not seem to exist in the database"),
                        QMessageBox::Ok);
        }
    });
}

Market::~Market()
{
    delete ui;
}

void Market::handle_dashboard()
{
    char buff[2048];

    ui->stackedWidget->setCurrentIndex(0);

    ui->dashboard_date->setText(QString(("Today's Date: " + current_date.to_string()).c_str()));
    ui->list_user_information->clear();

    // USER INFORMATION
    snprintf(buff, sizeof(buff),"User ID: %lu", current_user->id.id);
    ui->list_user_information->addItem(buff);
    snprintf(buff, sizeof(buff),"Username: %s", current_user->creds.username.c_str());
    ui->list_user_information->addItem(buff);

    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN ||
        current_user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        ui->edit_user_information->setText("Edit Information");

        display_account_information(ui->list_user_information, current_user);
    }

    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_ADMIN ||
        current_user->perms.user_type == (USER_TYPE) USER_TYPE_OPERATOR)
    {
        snprintf(buff, sizeof(buff), "Account Type: %s", user_type_strings[current_user->perms.user_type].c_str());
        ui->list_user_information->addItem(buff);

        ui->edit_user_information->setText("View Users");
    }

    // Notifications
    ui->list_notifications->clear();
    std::vector<std::string> notifications = notification_system->get_notifications(current_user->id);
    for (uint64_t i = 0; i < notifications.size(); i++)
    {
        ui->list_notifications->addItem(QString(notifications[i].c_str()));
    }

    // Only show active bookings and waitlists for vendors
    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN ||
        current_user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        ui->label_4->show();
        ui->label_5->show();
        ui->list_active_bookings->show();
        ui->list_active_waitlists->show();

        // active bookings
        ui->list_active_bookings->clear();
        QSqlQuery query;
        std::vector<int> years;
        std::vector<int> months;
        std::vector<int> days;
        std::string query_string = "SELECT for_day,for_month,for_year FROM bookings WHERE user_id = :id AND is_waitlist = 0;";
        query.prepare(QString(query_string.c_str()));
        query.bindValue(":id", QString::fromStdString(std::to_string(current_user->id.id)));
        Assert(query.exec(), "Failed query");

        while(query.next()){
            years.push_back(query.value("for_year").toInt()+1900);
            months.push_back(query.value("for_month").toInt());
            days.push_back(query.value("for_day").toInt()+1);
        }

        for(size_t i = 0; i < (size_t)years.size(); i++){
            std::string toAdd = "";

            toAdd += std::to_string(months[i]);
            toAdd += "/";
            toAdd += std::to_string(days[i]);
            toAdd += "/";
            toAdd += std::to_string(years[i]);
            ui->list_active_bookings->addItem(toAdd.c_str());
        }

        //active waitlists
        ui->list_active_waitlists->clear();
        std::vector<int> wyears;
        std::vector<int> wmonths;
        std::vector<int> wdays;
        query_string = "SELECT for_day,for_month,for_year FROM bookings WHERE user_id = :id AND is_waitlist = 1;";
        query.prepare(QString(query_string.c_str()));
        query.bindValue(":id", QString::fromStdString(std::to_string(current_user->id.id)));
        Assert(query.exec(), "Failed query");

        while(query.next()){
            wyears.push_back(query.value("for_year").toInt()+1900);
            wmonths.push_back(query.value("for_month").toInt());
            wdays.push_back(query.value("for_day").toInt()+1);
        }

        for(size_t i = 0; i < (size_t)wyears.size(); i++){
            std::string toAdd = "";

            toAdd += std::to_string(wmonths[i]);
            toAdd += "/";
            toAdd += std::to_string(wdays[i]);
            toAdd += "/";
            toAdd += std::to_string(wyears[i]);
//            QString s = QString("%1 (queue position: %2)")
//                    .arg(QString(toAdd))
//                    .arg(j - booking->limit + 1);
//            ui->list_active_waitlists->addItem(s);
            ui->list_active_waitlists->addItem(toAdd.c_str());
        }
    }
    else
    {
        // Do not show active bookings and waitlists for
        // operators or system administrators
        ui->label_4->hide();
        ui->label_5->hide();
        ui->list_active_bookings->hide();
        ui->list_active_waitlists->hide();
    }
}

void Market::handle_market_schedule()
{
    if (current_user->perms.user_type == USER_TYPE_FOOD ||
        current_user->perms.user_type == USER_TYPE_ARTISAN)
    {
        ui->user_list_market->hide();
        ui->label_24->hide();
        ui->cancel_booking->show();
    }
    else
    {
        ui->user_list_market->show();
        ui->label_24->show();
        ui->cancel_booking->hide();
    }

    ui->stackedWidget->setCurrentIndex(1);

    // User list
//    Assert(0, "TODO: i have no idea what this means.. "
//            "but we can query from the db all the users if we want to");
    std::string query_string;
    QSqlQuery query;

    query_string = std::string("SELECT username, user_type FROM users");
    query.prepare(QString(query_string.c_str()));
    Assert(query.exec(), "Query for users failed");

    std::vector<std::string> usernames;
    std::vector<USER_TYPE> userperms;
    while(query.next()){
            usernames.push_back(query.value("username").toString().toStdString());
            userperms.push_back((USER_TYPE)query.value("user_type").toInt());
    }

    ui->user_list_market->clear();
    for (uint64_t i = 0; i < usernames.size(); i++)
    {
        if (userperms[i] == USER_TYPE_FOOD ||
            userperms[i] == USER_TYPE_ARTISAN)
        {
            ui->user_list_market->addItem(QString::fromStdString(usernames[i]));
        }
    }

    // Display market dates
    if (ui->user_list_market->currentItem() != nullptr)
    {
        std::string username = ui->user_list_market->currentItem()->text().toStdString();
        Credentials creds = { username };
        User temp_user;
        bool ok = user_system->get_user(creds, &temp_user);
        if(ok)
        {
            display_market_information(ui->table_market_dates, &temp_user);
        }
        else
        {
            QMessageBox::warning(
                        this,
                        "Failed to find user to display market",
                        QString("The user did not seem to exist in the database"),
                        QMessageBox::Ok);
        }
    }
    else
    {
        display_market_information(ui->table_market_dates, current_user);
    }
}

void Market::handle_edit_information(){

    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD ||
        current_user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN)
    {
        // EDIT INFORMATION VIEW
        ui->stackedWidget->setCurrentIndex(4);

        ui->edit_business->setText(QString::fromStdString(current_user->business_name));
        ui->edit_owner->setText(QString::fromStdString(current_user->owner_name));
        ui->edit_phone->setText(QString::fromStdString(current_user->phone_number));
        ui->edit_email->setText(QString::fromStdString(current_user->email));
        ui->edit_mailing->setText(QString::fromStdString(current_user->mail_address));

        // Documentation Edit
        ui->business_exp->setText(QString::fromStdString(current_user->compliance_docs.business_licence.expiration_date));
        ui->business_number->setText(QString::fromStdString(current_user->compliance_docs.business_licence.number));

        ui->liability_exp->setText(QString::fromStdString(current_user->compliance_docs.liability_insurance.expiration_date));
        ui->liability_number->setText(QString::fromStdString(current_user->compliance_docs.liability_insurance.policy_number));
        ui->liability_provider->setText(QString::fromStdString(current_user->compliance_docs.liability_insurance.provider));

        ui->food_exp->setText(QString::fromStdString(current_user->compliance_docs.food_handler.expiration_date));
        ui->food_number->setText(QString::fromStdString(current_user->compliance_docs.food_handler.certification_number));
    }
    else
    {
        // USER LIST VIEW
        ui->stackedWidget->setCurrentIndex(2);
        std::string query_string;
        QSqlQuery query;

        query_string = std::string("SELECT username, user_type FROM users");
        query.prepare(QString(query_string.c_str()));
        Assert(query.exec(), "Query for users failed");

        std::vector<std::string> usernames;
        std::vector<USER_TYPE> userperms;
        while(query.next()){
                usernames.push_back(query.value("username").toString().toStdString());
                userperms.push_back((USER_TYPE)query.value("user_type").toInt());
        }

        // Display all vendors
        ui->user_list->clear();
        for (uint64_t i = 0; i < usernames.size(); i++)
        {
            if (userperms[i] == (USER_TYPE) USER_TYPE_FOOD ||
                userperms[i] == (USER_TYPE) USER_TYPE_ARTISAN)
            {
                ui->user_list->addItem(QString::fromStdString(usernames[i]));
            }
        }

        // Active bookings of user
        ui->user_booking_list->clear();
        ui->user_waitlist_list->clear();
        ui->user_information_view->clear();
    }
}

void Market::save_user_information(){
    if(ui->edit_business->text() != ""){
        current_user->business_name = ui->edit_business->text().toStdString();
        ui->edit_business->clear();
    }
    if(ui->edit_owner->text() != ""){
        current_user->owner_name = ui->edit_owner->text().toStdString();
        ui->edit_owner->clear();
    }
    if(ui->edit_phone->text() != ""){
        current_user->phone_number = ui->edit_phone->text().toStdString();
        ui->edit_phone->clear();
    }
    if(ui->edit_email->text() != ""){
        current_user->email = ui->edit_email->text().toStdString();
        ui->edit_email->clear();
    }
    if(ui->edit_mailing->text() != ""){
        current_user->mail_address = ui->edit_mailing->text().toStdString();
        ui->edit_mailing->clear();
    }

    // Documentation Edit
    if(ui->business_exp->text() != ""){
        current_user->compliance_docs.business_licence.expiration_date = ui->business_exp->text().toStdString();
        ui->business_exp->clear();
    }
    if(ui->business_number->text() != ""){
        current_user->compliance_docs.business_licence.number = ui->business_number->text().toStdString();
        ui->business_number->clear();
    }
    if(ui->liability_number->text() != ""){
        current_user->compliance_docs.liability_insurance.policy_number = ui->liability_number->text().toStdString();
        ui->liability_number->clear();
    }
    if(ui->liability_exp->text() != ""){
        current_user->compliance_docs.liability_insurance.expiration_date = ui->liability_exp->text().toStdString();
        ui->liability_exp->clear();
    }
    if(ui->liability_provider->text() != ""){
        current_user->compliance_docs.liability_insurance.provider = ui->liability_provider->text().toStdString();
        ui->liability_provider->clear();
    }

    if(ui->food_number->text() != ""){
        current_user->compliance_docs.food_handler.certification_number = ui->food_number->text().toStdString();
        ui->food_number->clear();
    }
    if(ui->food_exp->text() != ""){
        current_user->compliance_docs.food_handler.expiration_date = ui->food_exp->text().toStdString();
        ui->food_exp->clear();
    }

    user_system->update_user(*current_user);

    handle_dashboard();
}

void Market::display_account_information(QListWidget *list, User *user)
{
    char buff[2048];

    snprintf(buff, sizeof(buff),"Business Name: %s", user->business_name.c_str());
    list->addItem(buff);

    list->addItem("");
    list->addItem("Contact Information");

    snprintf(buff, sizeof(buff),"Email: %s", user->email.c_str());
    list->addItem(buff);
    snprintf(buff, sizeof(buff),"Phone Number: %s", user->phone_number.c_str());
    list->addItem(buff);
    snprintf(buff, sizeof(buff),"Mailing Address: %s", user->mail_address.c_str());
    list->addItem(buff);

    list->addItem("");
    snprintf(buff, sizeof(buff),"Vendor Category: %s", user_type_strings[user->perms.user_type].c_str());
    list->addItem(buff);

    list->addItem("");
    list->addItem("Compliance Documentation");

    snprintf(buff, sizeof(buff),"Business Licence Number %s", user->compliance_docs.business_licence.number.c_str());
    list->addItem(buff);
    snprintf(buff, sizeof(buff),"Business Licence Expiration Date %s", user->compliance_docs.business_licence.expiration_date.c_str());
    list->addItem(buff);

    list->addItem("");
    snprintf(buff, sizeof(buff),"Liability Insurance Policy Number %s", user->compliance_docs.liability_insurance.policy_number.c_str());
    list->addItem(buff);
    snprintf(buff, sizeof(buff),"Liability Insurance Provider %s", user->compliance_docs.liability_insurance.provider.c_str());
    list->addItem(buff);
    snprintf(buff, sizeof(buff),"Liability Insurance Expiration Date %s", user->compliance_docs.liability_insurance.expiration_date.c_str());
    list->addItem(buff);

    if (user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        list->addItem("");
        snprintf(buff, sizeof(buff),"Food Handler Certification Number %s", user->compliance_docs.food_handler.certification_number.c_str());
        list->addItem(buff);
        snprintf(buff, sizeof(buff),"Food Handler Expiration Date %s", user->compliance_docs.food_handler.expiration_date.c_str());
        list->addItem(buff);
    }
}

void Market::display_market_information(QTableWidget *table, User *user)
{
    table->clear();
    table->setColumnCount(3);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList headers = {"Date", "ID", "Availability", "Status"};
    table->setHorizontalHeaderLabels(headers);

    if (user->perms.user_type == USER_TYPE_OPERATOR ||
        user->perms.user_type == USER_TYPE_ADMIN)
    {
        table->setRowCount(8);
    }
    else
    {
        table->setRowCount(4);
    }

//    Assert(0, "TODO: query all the market dates again");
    std::vector<MarketDate> market_dates;

    std::string query_string =
            "SELECT * FROM market_dates";
    QSqlQuery query;
    query.prepare(QString(query_string.c_str()));
    query.exec();
    while(query.next()){
        MarketDate marketDate;
        Date date;
        int year = query.value("year").toInt();
        int month = query.value("day").toInt();
        int day = query.value("month").toInt();
        int id = query.value("id").toInt();
        date.day = (uint64_t)day;
        date.month = (uint64_t)month;
        date.year = (uint64_t)year;
        marketDate.date = date;
        marketDate.id.id = (uint64_t)id;
        market_dates.push_back(marketDate);
    }

    for (uint64_t i = 0; i < market_dates.size(); i++)
    {
        int64_t availability = 0;
        int64_t book_or_wait = -1;
        uint64_t waitlist_position;

//        Assert(0, "TODO: ANDWUUU get booking information of "
//                  "current market date based on user type ARTISAN/FOOD");
        Booking booking;
        //GET LIMIT
        int year = -1;
        int month = -1;
        int day = -1;
        if(user->perms.user_type == USER_TYPE_ARTISAN){
            query_string = std::string("SELECT artisan_limit, year, day, month FROM market_dates where id = :id;");
            query.prepare(QString(query_string.c_str()));

            query.bindValue(":id", QString::fromStdString(std::to_string(market_dates[i].id.id)));
            Assert(query.exec(), "Query for Limit fail");

            while(query.next()){
                 booking.limit = (uint64_t)query.value("artisan_limit").toInt();
            }
        }
        else if(user->perms.user_type == USER_TYPE_FOOD){
            query_string = std::string("SELECT food_limit, year, day, month FROM market_dates where id = :id;");
            query.prepare(QString(query_string.c_str()));

            query.bindValue(":id", QString::fromStdString(std::to_string(market_dates[i].id.id)));
            Assert(query.exec(), "Query for Limit fail");

            while(query.next()){
                booking.limit = (uint64_t)query.value("food_limit").toInt();
                month = query.value("month").toInt();
                day = query.value("day").toInt();
                year = query.value("year").toInt();
            }
        }

        //GET NUM BOOKINGS
        query_string = std::string("SELECT DISTINCT booking_id FROM bookings INNER JOIN users WHERE user_type = :type AND for_year = :year AND for_month = :month AND for_day = :day;");
        query.prepare(QString(query_string.c_str()));

        query.bindValue(":type", QString::fromStdString(std::to_string(user->perms.user_type)));
        query.bindValue(":year", QString::fromStdString(std::to_string(year)));
        query.bindValue(":month", QString::fromStdString(std::to_string(month)));
        query.bindValue(":day", QString::fromStdString(std::to_string(day)));
        Assert(query.exec(), "Query for Limit fail");
        int numBookings = 0;
        while(query.next()){
            numBookings++;
        }
        availability = (int64_t)booking.limit - numBookings;

        // OPERATOR - Show overall availability
        if (user->perms.user_type == USER_TYPE_OPERATOR ||
            user->perms.user_type == USER_TYPE_ADMIN)
        {
            booking.limit = 4;
//            Assert(0, "TODO: ANDWUWUWUWUWUW get booking information of the other USER_TYPE ARTISAN OR FOOD"
//                   ", this displays total availability of market date for the operator");
            availability += (int64_t)booking.limit - (int64_t)booking.users.size();
        }

        // Check if booked or on waitlist

        for (uint64_t j = 0; j < booking.users.size(); j++)
        {
            if(booking.users[j] == user->id)
            {
                book_or_wait = 0;
                if(j > booking.limit - 1)
                {
                    book_or_wait = 1;
                    waitlist_position = j - booking.limit + 1;
                }
            }
        }

        QString availability_str = QString("Stalls available: %1").arg(std::max(0l, availability));
        QString market_date_id = QString("%1").arg(market_dates[i].id.id);
        QString date_str = QString(market_dates[i].date.to_string().c_str());
        QString status_str;

        if (current_user->perms.user_type == USER_TYPE_OPERATOR ||
            current_user->perms.user_type == USER_TYPE_ADMIN)
        {
            if (availability == 0)
            {
                status_str = QString("Unavailable");
            }
            else
            {
                status_str = QString("-");
            }
        }
        else
        {
            if (book_or_wait == 0)
            {
                status_str = QString("Booked");
            } else if (book_or_wait == 1){
                status_str = QString("Waitlisted (queue position: %1)").arg(waitlist_position);
            } else {
                status_str = QString("-");
            }
        }

        table->setItem((int)i, 0, new QTableWidgetItem(date_str));
        table->setItem((int)i, 1, new QTableWidgetItem(market_date_id));
        table->setItem((int)i, 2, new QTableWidgetItem(availability_str));
        table->setItem((int)i, 3, new QTableWidgetItem(status_str));
    }
#if 0
    for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++) {
        int64_t availability = 0;
        int64_t book_or_wait = -1;
        uint64_t waitlist_position;

        MarketDate *market_date = &market_date_system->market_dates[i];

        Booking *booking = 0;
        if(user->perms.user_type == USER_TYPE_ARTISAN)
        {
            booking = &market_date->artisan_booking;
        }
        if(user->perms.user_type == USER_TYPE_FOOD)
        {
            booking = &market_date->food_booking;
        }
        if(booking == 0)
        {
            booking = &market_date->food_booking;
        }

        // Get availability
        availability = (int64_t)booking->limit - (int64_t)booking->users.size();

        // OPERATOR - Show overall availability
        if (user->perms.user_type == USER_TYPE_OPERATOR ||
            user->perms.user_type == USER_TYPE_ADMIN)
        {
            booking = &market_date->artisan_booking;
            availability += (int64_t)booking->limit - (int64_t)booking->users.size();
        }

        // Check if booked or on waitlist
        for (uint64_t j = 0; j < booking->users.size(); j++) {
            if (booking->users[j] == user->id) {
                book_or_wait = 0;
                if (j > booking->limit - 1) {
                    book_or_wait = 1;
                    waitlist_position = j - booking->limit + 1;
                }
            }
        }

        QString availability_str = QString("Stalls available: %1").arg(std::max(0l, availability));
        QString date_str = QString(market_date_system->market_dates[i].date.to_string().c_str());
        QString status_str;

        if (current_user->perms.user_type == USER_TYPE_OPERATOR ||
            current_user->perms.user_type == USER_TYPE_ADMIN)
        {
            if (availability == 0)
            {
                status_str = QString("Unavailable");
            }
            else
            {
                status_str = QString("-");
            }
        }
        else
        {
            if (book_or_wait == 0)
            {
                status_str = QString("Booked");
            } else if (book_or_wait == 1){
                status_str = QString("Waitlisted (queue position: %1)").arg(waitlist_position);
            } else {
                status_str = QString("-");
            }
        }


        table->setItem((int)i, 0, new QTableWidgetItem(date_str));
        table->setItem((int)i, 1, new QTableWidgetItem(availability_str));
        table->setItem((int)i, 2, new QTableWidgetItem(status_str));
    }
#endif
}
