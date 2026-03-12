#ifndef MARKETSCHEDULE_H
#define MARKETSCHEDULE_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QWidget>
#include "ui_marketschedule.h"
#pragma GCC diagnostic pop

namespace Ui {
class MarketSchedule;
}

class MarketSchedule : public QWidget
{
    Q_OBJECT

public:
    explicit MarketSchedule(QWidget *parent = nullptr);
    ~MarketSchedule();
    Ui::MarketSchedule *ui;
};

#endif // MARKETSCHEDULE_H
