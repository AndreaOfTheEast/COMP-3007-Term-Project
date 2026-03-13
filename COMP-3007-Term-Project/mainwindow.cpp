#include "mainwindow.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "ui_mainwindow.h"
#pragma GCC diagnostic pop

MainWindow::MainWindow(UserSystem *in_user_system, MarketDateSystem *in_market_date_system, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , user_system(in_user_system)
    , market_date_system(in_market_date_system)
{
    ui->setupUi(this);

    // set up date
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
        // if nothing is selected than just exit
        if (ui->table_market_dates->selectedItems().isEmpty())
        {
            return;
        }

        std::vector<UserId> *vector = nullptr;

        QMessageBox msgBox;

        // TODO: check if user is already inside the booking
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            vector = &market_date_system->market_dates[index].artisan_users;

        }
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            vector = &market_date_system->market_dates[index].food_users;
        }

        if (vector != nullptr)
        {
            auto it = std::find(vector->begin(), vector->end(), current_user->id);
            if (it != vector->end())
            {
                msgBox.setText(QString("You are already booked for that date."));
                msgBox.exec();
                return;
            }
        }
        else
        {
            msgBox.setText(QString("Your account type prohibits booking directly."));
            msgBox.exec();
            return;
        }

        // TODO: Need to show what date the user selected for confirmation.

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

        // if nothing is selected than just exit
        if (ui->table_market_dates->selectedItems().isEmpty())
        {
            return;
        }

        // Remove
        // TODO: add notifications
        MarketDate &market_date = market_date_system->market_dates[index];
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            for (uint32_t i = 0; i < market_date.artisan_users.size(); i++)
            {
                if (market_date.artisan_users[i] == current_user->id)
                {
                    market_date.artisan_users.erase(market_date.artisan_users.begin() + i);
                    handle_market_schedule();
                    return;
                }
            }
        }

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            for (uint32_t i = 0; i < market_date.food_users.size(); i++)
            {
                if (market_date.food_users[i] == current_user->id)
                {
                    market_date.food_users.erase(market_date.food_users.begin() + i);
                    handle_market_schedule();
                    return;
                }
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_dashboard()
{
    char buff[2048];
    ui->stackedWidget->setCurrentIndex(0);

    ui->dashboard_date->setText(QString(("Today's Date: " + current_date.to_string()).c_str()));

    // user information
    ui->list_user_information->clear();

    snprintf(buff, sizeof(buff),"User ID: %lu", current_user->id.id);
    ui->list_user_information->addItem(buff);
    snprintf(buff, sizeof(buff),"Username: %s", current_user->creds.username.c_str());
    ui->list_user_information->addItem(buff);
    snprintf(buff, sizeof(buff),"Buisness Name: %s", current_user->business_name.c_str());
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

    ui->list_user_information->addItem("");
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

    ui->list_user_information->addItem("");
    snprintf(buff, sizeof(buff),"Food Handler Certification Number %s", current_user->compliance_docs.food_handler.certification_number.c_str());
    ui->list_user_information->addItem(buff);
    snprintf(buff, sizeof(buff),"Food Handler Expiration Date %s", current_user->compliance_docs.food_handler.expiration_date.c_str());
    ui->list_user_information->addItem(buff);

    // TODO: notifications

    // active bookings
    ui->list_active_bookings->clear();
    for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++)
    {
        MarketDate &market_date = market_date_system->market_dates[i];

        for (uint64_t j = 0; j < std::min(market_date.artisan_users.size(), market_date.artisan_limit); j++)
        {
            if (market_date.artisan_users[j].id == current_user->id.id)
            {
                ui->list_active_bookings->addItem(market_date.date.to_string().c_str());
            }
        }
        for (uint64_t j = 0; j < std::min(market_date.food_users.size(), market_date.food_limit); j++)
        {
            if (market_date.food_users[j].id == current_user->id.id)
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

        for (uint64_t j = market_date.artisan_limit; j < market_date.artisan_users.size(); j++)
        {
            if (market_date.artisan_users[j].id == current_user->id.id)
            {
                QString s = QString("%1 (queue position: %2)")
                        .arg(QString(market_date.date.to_string().c_str()))
                        .arg(j - market_date.artisan_limit + 1);
                ui->list_active_waitlists->addItem(s);
            }
        }
        for (uint64_t j = market_date.food_limit; j < market_date.food_users.size(); j++)
        {
            if (market_date.food_users[j].id == current_user->id.id)
            {
                QString s = QString("%1 (queue position: %2)")
                        .arg(QString(market_date.date.to_string().c_str()))
                        .arg(j - market_date.food_limit + 1);
                ui->list_active_waitlists->addItem(s);
            }
        }
    }

}

void MainWindow::handle_market_schedule()
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

        // get availability
        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            availability = market_date->artisan_limit - market_date->artisan_users.size();
        }

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            availability = market_date->food_limit - market_date->food_users.size();
        }

        // check if booked or on waitlist

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
        {
            for (uint64_t j = 0; j < market_date->artisan_users.size(); j++) {
                if (market_date->artisan_users[j] == current_user->id) {
                    book_or_wait = 0;
                    if (j > market_date->artisan_limit - 1) {
                        book_or_wait = 1;
                        waitlist_position = j - market_date->artisan_limit + 1;
                    }
                }
            }
        }

        if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
        {
            for (uint64_t j = 0; j < market_date->food_users.size(); j++) {
                if (market_date->food_users[j] == current_user->id) {
                    book_or_wait = 0;
                    if (j > market_date->food_limit - 1) {
                        book_or_wait = 1;
                        waitlist_position = j - market_date->food_limit + 1;
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
