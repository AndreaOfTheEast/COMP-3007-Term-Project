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
    dashboard = new Dashboard(this);

    this->setCentralWidget(dashboard);
}

MainWindow::~MainWindow()
{
    delete ui;
}
