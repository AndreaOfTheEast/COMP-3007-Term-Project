#include "mainwindow.h"
#include "dashboard.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "ui_mainwindow.h"
#pragma GCC diagnostic pop

MainWindow::MainWindow(UserSystem *user_system, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , user_system(user_system)
{
    ui->setupUi(this);

    connect(ui->browse_market, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->back_to_dashboard, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
