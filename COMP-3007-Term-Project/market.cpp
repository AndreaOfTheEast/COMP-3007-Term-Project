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
        std::vector<UserId> *vector = nullptr;
        QMessageBox msgBox;

        // if nothing is selected than just exit
        if (ui->table_market_dates->selectedItems().isEmpty())
        {
            return;
        }

        // Check if user is already inside the booking
        if (market_date_system->is_user_booked(current_user, index) >= 0)
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
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_OPERATOR)
        {
            std::string username = ui->user_list_market->currentItem()->text().toStdString();
            Credentials creds = { username };
            user = user_system->get_user(creds);
        }
        if (user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            vector = &market_date_system->market_dates[index].artisan_booking.users;
        }
        if (user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            vector = &market_date_system->market_dates[index].food_booking.users;
        }

        if (vector == nullptr)
        {
            msgBox.setText(QString("Your account type prohibits booking directly."));
            msgBox.exec();
            return;
        }

        QMessageBox::StandardButton question;
        question = QMessageBox::question(this, "Confirm Action", "Are you sure you want to book this date?", QMessageBox::Yes | QMessageBox::No);

        if (question == QMessageBox::Yes)
        {
            if (current_user->perms.user_type == USER_TYPE_OPERATOR)
            {
                std::string username = ui->user_list_market->currentItem()->text().toStdString();

                if (ui->user_list_market->currentItem() != nullptr)
                {
                    username = ui->user_list_market->currentItem()->text().toStdString();
                }
                else
                {
                    QMessageBox::warning(
                                this,
                                "Booking Failed",
                                "No selected user.",
                                QMessageBox::Ok);
                }

                market_date_system->make_booking(user, index);

                std::stringstream notification_msg;
                notification_msg << "[Action] Booked "
                                 << ui->table_market_dates->item((int32_t)index, 0)->text().toStdString()
                                 << " for " << user->creds.username << ".";
                notification_system->add_notification(current_user->id, notification_msg.str());
            }
            else
            {
                market_date_system->make_booking(user, index);
            }
        }

        display_market_information(ui->table_market_dates, user);
    });

    // Cancel booking
    connect(ui->cancel_booking, &QPushButton::clicked, this, [=]{
        uint64_t index = (uint64_t)ui->table_market_dates->currentRow();
        QMessageBox::StandardButton question;
        QString msg;
        std::string date;

        // if nothing is selected than just exit
        if (ui->table_market_dates->selectedItems().isEmpty())
        {
            return;
        }

        date = market_date_system->market_dates[index].date.to_string();

        // Check if user is in booking list
        // operator book for vendors

        int64_t is_booked = market_date_system->is_user_booked(current_user, index);

        if (is_booked == -2)
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
                    QMessageBox::Yes | QMessageBox::No );

        if (question != QMessageBox::Yes) { return; }

        // Cancel booking
        market_date_system->cancel_booking(current_user, index);

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

        Credentials creds = { username };
        User *user = in_user_system->get_user(creds);

        // Display account information
        display_account_information(ui->user_information_view, in_user_system->get_user(creds));

        // Active bookings
        for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
        {
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
            if(booking == 0) { break; }

            for (uint64_t j = 0; j < std::min(booking->users.size(), booking->limit); j++)
            {
                if (booking->users[j].id == user->id.id)
                {
                    ui->user_booking_list->addItem(market_date->date.to_string().c_str());
                }
            }
        }

        // Active waitlists
        for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
        {
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
            if(booking == 0) { break; }

            for (uint64_t j = booking->limit; j < booking->users.size(); j++)
            {
                if (booking->users[j].id == user->id.id)
                {
                    QString s = QString("%1 (queue position: %2)")
                            .arg(QString(market_date->date.to_string().c_str()))
                            .arg(j - booking->limit + 1);
                    ui->user_waitlist_list->addItem(s);
                }
            }
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

        // Get username
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
        Credentials creds = { username };
        User *user = in_user_system->get_user(creds);

        // Check if the user has selected a booking or a waitlist
        if (!ui->user_booking_list->selectedItems().isEmpty())
        {
            date = ui->user_booking_list->currentItem()->text();
        }
        else if (!ui->user_waitlist_list->selectedItems().isEmpty())
        {
            date = ui->user_waitlist_list->currentItem()->text().left(10);
            is_waitlist = 1;
        }
        else
        {
            return;
        }

        uint64_t index = 0;

        for (; index < market_date_system->market_dates.size(); index++)
        {
            if (market_date_system->market_dates[index].date.to_string() == date.toStdString())
            {
                break;
            }
        }

        // User's booking is not found
        if (index >= market_date_system->market_dates.size()) {
            QMessageBox::warning(
                        this,
                        "Cancellation Failed",
                        QString("This booking/waitlist (%1) does not exist.")
                            .arg(date),
                        QMessageBox::Ok);
            return;
        }

        // Cancel booking
        market_date_system->cancel_booking(user, index);

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
        display_market_information(ui->table_market_dates, user_system->get_user(creds));
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
        for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
        {
            MarketDate *market_date = &market_date_system->market_dates[i];

            Booking *booking = 0;
            if(current_user->perms.user_type == USER_TYPE_ARTISAN)
            {
                booking = &market_date->artisan_booking;
            }
            if(current_user->perms.user_type == USER_TYPE_FOOD)
            {
                booking = &market_date->food_booking;
            }
            if(booking == 0) { break; }

            for (uint64_t j = 0; j < std::min(booking->users.size(), booking->limit); j++)
            {
                if (booking->users[j].id == current_user->id.id)
                {
                    ui->list_active_bookings->addItem(market_date->date.to_string().c_str());
                }
            }
        }

        // active waitlists
        ui->list_active_waitlists->clear();
        for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
        {
            MarketDate *market_date = &market_date_system->market_dates[i];

            Booking *booking = 0;
            if(current_user->perms.user_type == USER_TYPE_ARTISAN)
            {
                booking = &market_date->artisan_booking;
            }
            if(current_user->perms.user_type == USER_TYPE_FOOD)
            {
                booking = &market_date->food_booking;
            }
            if(booking == 0) { break; }

            for (uint64_t j = booking->limit; j < booking->users.size(); j++)
            {
                if (booking->users[j].id == current_user->id.id)
                {
                    QString s = QString("%1 (queue position: %2)")
                            .arg(QString(market_date->date.to_string().c_str()))
                            .arg(j - booking->limit + 1);
                    ui->list_active_waitlists->addItem(s);
                }
            }
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
    std::vector<User> users = user_system->get_user_list();
    ui->user_list_market->clear();
    for (uint64_t i = 0; i < users.size(); i++)
    {
        if (users[i].perms.user_type == USER_TYPE_FOOD ||
            users[i].perms.user_type == USER_TYPE_ARTISAN)
        {
            ui->user_list_market->addItem(users[i].creds.username.c_str());
        }
    }

    // Display market dates
    if (ui->user_list_market->currentItem() != nullptr)
    {
        std::string username = ui->user_list_market->currentItem()->text().toStdString();
        Credentials creds = { username };
        display_market_information(ui->table_market_dates, user_system->get_user(creds));
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
        std::vector<User> users = user_system->get_user_list();

        // Display all vendors
        ui->user_list->clear();
        for (uint64_t i = 0; i < users.size(); i++)
        {
            if (users[i].perms.user_type == (USER_TYPE) USER_TYPE_FOOD ||
                users[i].perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN)
            {
                ui->user_list->addItem(QString::fromStdString(users[i].creds.username));
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
    QStringList headers = {"Date", "Availability", "Status"};
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
}
