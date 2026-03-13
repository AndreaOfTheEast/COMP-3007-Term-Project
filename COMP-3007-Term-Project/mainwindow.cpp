#include "mainwindow.h"
#include "dashboard.h"

#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "ui_mainwindow.h"
#pragma GCC diagnostic pop

void
MainWindow::handle_market_date_information()
{
    int64_t availability = 0;
    uint64_t index = (uint64_t)ui->list_market_dates->currentRow();
    MarketDate *market_date = &market_date_system->market_dates[index];

    if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_ARTISAN)
    {
        availability = (int64_t)(market_date->artisan_limit - market_date->artisan_users.size());
    }

    if (current_user->perms.user_type == (USER_TYPE)USER_TYPE_FOOD)
    {
        availability = (int64_t)(market_date->food_limit - market_date->food_users.size());
    }

    // TODO: display date
    QString availability_str = QString("Stalls available: %1").arg(availability);

    ui->list_booking_information->clear();
    ui->list_booking_information->addItem(availability_str);
}

MainWindow::MainWindow(UserSystem *user_system, MarketDateSystem *market_date_system, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , user_system(user_system)
    , market_date_system(market_date_system)
{
    ui->setupUi(this);

    // set up date
    current_date = Date::get_now();

    // Navigate to the Market Schedule
    connect(ui->browse_market, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentIndex(1);
        handle_market_schedule();
    });

    // Navigate to the dashboard
    connect(ui->back_to_dashboard, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentIndex(0);
        handle_dashboard();
    });

    // Show Market Date information (availability)
    connect(ui->list_market_dates, &QListWidget::itemClicked, this, [=] {
        handle_market_date_information();
    });

    // Make a booking
    connect(ui->make_booking, &QPushButton::clicked, this, [=]{
        uint64_t index = (uint64_t)ui->list_market_dates->currentRow();
        // if nothing is selected than just exit
        if (ui->list_market_dates->selectedItems().isEmpty())
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
            handle_market_date_information();
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
}

void MainWindow::handle_market_schedule()
{
    ui->list_market_dates->clear();
    ui->list_booking_information->clear();

    for (uint64_t i = 0; i < market_date_system->market_dates.size(); i++) {
        ui->list_market_dates->addItem(QString((market_date_system->market_dates[i].date.to_string()).c_str()));
    }
}
