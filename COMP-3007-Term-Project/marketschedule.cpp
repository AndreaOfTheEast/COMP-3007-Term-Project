#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "marketschedule.h"
#include "ui_marketschedule.h"
#pragma GCC diagnostic pop

MarketSchedule::MarketSchedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketSchedule)
{
    ui->setupUi(this);
//    connect(ui->make_booking, &QPushButton::clicked, this, [=]{add_booking();});
}

void MarketSchedule::add_booking(){
    std::cout<<"hi"<<std::endl;
}

MarketSchedule::~MarketSchedule()
{
    delete ui;
}
