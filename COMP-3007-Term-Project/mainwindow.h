#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hinton_market.h"
#include "login.h"
#include "market.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;

    UserSystem *user_system;
    MarketDateSystem *market_date_system;

    User *current_user;
    Date current_date;

    MainWindow(UserSystem *user_system, MarketDateSystem *booking_system, QWidget *parent = nullptr);
    ~MainWindow();

    void handle_dashboard();
    void handle_market_schedule();

public slots:
public emit:
};
#endif // MAINWINDOW_H
