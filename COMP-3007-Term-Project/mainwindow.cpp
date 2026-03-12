#include "mainwindow.h"
#include "dashboard.h"

#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "ui_mainwindow.h"
#pragma GCC diagnostic pop

MainWindow::MainWindow(UserSystem *user_system, BookingSystem *booking_system, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , user_system(user_system)
    , booking_system(booking_system)
{
    ui->setupUi(this);

    connect(ui->browse_market, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentIndex(1);
        handle_market_schedule();
    });
    connect(ui->back_to_dashboard, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentIndex(0);
        handle_dashboard();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_dashboard()
{
    char buff[2048];

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
    char buff[2048];
    for (uint64_t i = 0; i < booking_system->bookings.size(); i++) {
        snprintf(buff, sizeof(buff),"%lu", booking_system->bookings[i].date.day);
        ui->list_booking_dates->addItem(buff);
    }
}
