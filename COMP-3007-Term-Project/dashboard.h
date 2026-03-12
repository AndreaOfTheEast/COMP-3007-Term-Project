#ifndef DASHBOARD_H
#define DASHBOARD_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QWidget>
#pragma GCC diagnostic pop

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private:
    Ui::Dashboard *ui;
};

#endif // DASHBOARD_H
