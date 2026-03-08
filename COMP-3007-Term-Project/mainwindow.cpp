#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "hinton_market.h"
#include "login.cpp"
#include "dashboard.cpp"
#include "market.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    User user;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
