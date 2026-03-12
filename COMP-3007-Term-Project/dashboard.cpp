#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "dashboard.h"
#include "ui_dashboard.h"
#pragma GCC diagnostic pop

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
}

Dashboard::~Dashboard()
{
    delete ui;
}
