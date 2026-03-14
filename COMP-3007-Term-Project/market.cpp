#include "market.h"
#include <sstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "ui_market.h"
// TODO: Remove this
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

    // Navigate to the Market Schedule
    connect(ui->browse_market, &QPushButton::clicked, this, [=]{
        handle_market_schedule();
    });

    // Navigate to the dashboard
    connect(ui->back_to_dashboard, &QPushButton::clicked, this, [=]{
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

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            vector = &market_date_system->market_dates[index].artisan_booking.users;
        }
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
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
            market_date_system->make_booking(current_user, index);
        }

        handle_market_schedule();
    });

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
        // TODO: make sure the operator cant book one for themselves
        // operator book for vendors

        int64_t is_booked = market_date_system->is_user_booked(current_user, index);
        if (is_booked == -1) {
            msg = QString("You do not have a booking for this date (%1).")
                    .arg(date.c_str());
            QMessageBox::warning(
                        this,
                        "Cancellation Failed",
                        msg);
            return;
        }
        // TODO: remove this once implemented properly
        else if (is_booked == -2)
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
    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_ADMIN ||
        current_user->perms.user_type == (USER_TYPE) USER_TYPE_OPERATOR)
    {
        snprintf(buff, sizeof(buff),
                 "Account Type: %s",
                 user_type_strings[current_user->perms.user_type].c_str());
        ui->list_user_information->addItem("");
        ui->list_user_information->addItem(buff);
    }

    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_ARTISAN ||
        current_user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        snprintf(buff, sizeof(buff),"Business Name: %s", current_user->business_name.c_str());
        ui->list_user_information->addItem(buff);

        ui->list_user_information->addItem("");
        ui->list_user_information->addItem("Contact Information");

        snprintf(buff, sizeof(buff),"Email: %s", current_user->email.c_str());
        ui->list_user_information->addItem(buff);
        snprintf(buff, sizeof(buff),"Phone Number: %s", current_user->phone_number.c_str());
        ui->list_user_information->addItem(buff);
        snprintf(buff, sizeof(buff),"Mailing Address: %s", current_user->mail_address.c_str());
        ui->list_user_information->addItem(buff);

        ui->list_user_information->addItem("");
        snprintf(buff, sizeof(buff),"Vendor Category: %s", user_type_strings[current_user->perms.user_type].c_str());
        ui->list_user_information->addItem(buff);

        ui->list_user_information->addItem("");
        ui->list_user_information->addItem("Compliance Documentation");

        snprintf(buff, sizeof(buff),"Business Licence Number %s", current_user->compliance_docs.business_licence.number.c_str());
        ui->list_user_information->addItem(buff);
        snprintf(buff, sizeof(buff),"Business Licence Expiration Date %s", current_user->compliance_docs.business_licence.expiration_date.c_str());
        ui->list_user_information->addItem(buff);

        ui->list_user_information->addItem("");
        snprintf(buff, sizeof(buff),"Liability Insurance Policy Number %s", current_user->compliance_docs.liability_insurance.policy_number.c_str());
        ui->list_user_information->addItem(buff);
        snprintf(buff, sizeof(buff),"Liability Insurance Provider %s", current_user->compliance_docs.liability_insurance.provider.c_str());
        ui->list_user_information->addItem(buff);
        snprintf(buff, sizeof(buff),"Liability Insurance Expiration Date %s", current_user->compliance_docs.liability_insurance.expiration_date.c_str());
        ui->list_user_information->addItem(buff);
    }

    if (current_user->perms.user_type == (USER_TYPE) USER_TYPE_FOOD)
    {
        ui->list_user_information->addItem("");
        snprintf(buff, sizeof(buff),"Food Handler Certification Number %s", current_user->compliance_docs.food_handler.certification_number.c_str());
        ui->list_user_information->addItem(buff);
        snprintf(buff, sizeof(buff),"Food Handler Expiration Date %s", current_user->compliance_docs.food_handler.expiration_date.c_str());
        ui->list_user_information->addItem(buff);
    }

    // notifications
    ui->list_notifications->clear();
    std::vector<std::string> notifications = notification_system->get_notifications(current_user->id);
    for (uint64_t i = 0; i < notifications.size(); i++)
    {
        ui->list_notifications->addItem(QString(notifications[i].c_str()));
    }

    // active bookings
    ui->list_active_bookings->clear();
    for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
    {
        MarketDate &market_date = market_date_system->market_dates[i];

        for (uint64_t j = 0; j < std::min(market_date.artisan_booking.users.size(), market_date.artisan_booking.limit); j++)
        {
            if (market_date.artisan_booking.users[j].id == current_user->id.id)
            {
                ui->list_active_bookings->addItem(market_date.date.to_string().c_str());
            }
        }
        for (uint64_t j = 0; j < std::min(market_date.food_booking.users.size(), market_date.food_booking.limit); j++)
        {
            if (market_date.food_booking.users[j].id == current_user->id.id)
            {
                ui->list_active_bookings->addItem(market_date.date.to_string().c_str());
            }
        }
    }

    // active waitlists
    ui->list_active_waitlists->clear();
    for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
    {
        MarketDate &market_date = market_date_system->market_dates[i];

        for (uint64_t j = market_date.artisan_booking.limit; j < market_date.artisan_booking.users.size(); j++)
        {
            if (market_date.artisan_booking.users[j].id == current_user->id.id)
            {
                QString s = QString("%1 (queue position: %2)")
                        .arg(QString(market_date.date.to_string().c_str()))
                        .arg(j - market_date.artisan_booking.limit + 1);
                ui->list_active_waitlists->addItem(s);
            }
        }
        for (uint64_t j = market_date.food_booking.limit; j < market_date.food_booking.users.size(); j++)
        {
            if (market_date.food_booking.users[j].id == current_user->id.id)
            {
                QString s = QString("%1 (queue position: %2)")
                        .arg(QString(market_date.date.to_string().c_str()))
                        .arg(j - market_date.food_booking.limit + 1);
                ui->list_active_waitlists->addItem(s);
            }
        }
    }

}

void Market::handle_market_schedule()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->table_market_dates->clear();
    ui->table_market_dates->setColumnCount(3);
    ui->table_market_dates->setRowCount(5);
    ui->table_market_dates->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_market_dates->verticalHeader()->setVisible(false);
    ui->table_market_dates->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList headers = {"Date", "Availability", "Status"};
    ui->table_market_dates->setHorizontalHeaderLabels(headers);


    for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++) {
        uint64_t availability = 0;
        int64_t book_or_wait = -1;
        uint64_t waitlist_position;

        MarketDate *market_date = &market_date_system->market_dates[i];

        // Get availability
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            availability = market_date->artisan_booking.limit - market_date->artisan_booking.booked;
        }

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            availability = market_date->food_booking.limit - market_date->food_booking.booked;
        }

        // Check if booked or on waitlist
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            for (uint64_t j = 0; j < market_date->artisan_booking.users.size(); j++) {
                if (market_date->artisan_booking.users[j] == current_user->id) {
                    book_or_wait = 0;
                    if (j > market_date->artisan_booking.limit - 1) {
                        book_or_wait = 1;
                        waitlist_position = j - market_date->artisan_booking.limit + 1;
                    }
                }
            }
        }

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            for (uint64_t j = 0; j < market_date->food_booking.users.size(); j++) {
                if (market_date->food_booking.users[j] == current_user->id) {
                    book_or_wait = 0;
                    if (j > market_date->food_booking.limit - 1) {
                        book_or_wait = 1;
                        waitlist_position = j - market_date->food_booking.limit + 1;
                    }
                }
            }
        }

        QString availability_str = QString("Stalls available: %1").arg(std::max(0lu, availability));
        QString date_str = QString(market_date_system->market_dates[i].date.to_string().c_str());
        QString status_str;

        if (book_or_wait == 0)
        {
            status_str = QString("Booked");
        } else if (book_or_wait == 1){
            status_str = QString("Waitlisted (queue position: %1)").arg(waitlist_position);
        } else {
            status_str = QString("-");
        }

        ui->table_market_dates->setItem((int)i, 0, new QTableWidgetItem(date_str));
        ui->table_market_dates->setItem((int)i, 1, new QTableWidgetItem(availability_str));
        ui->table_market_dates->setItem((int)i, 2, new QTableWidgetItem(status_str));
    }
}
