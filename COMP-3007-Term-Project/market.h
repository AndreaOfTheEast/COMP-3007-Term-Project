#pragma once

#include "hinton_market.h"
#include "login.h"
#include "market_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Market; }
QT_END_NAMESPACE

class Market : public QMainWindow
{
    Q_OBJECT

public:
    Ui::Market *ui;

    UserSystem *user_system;
    MarketDateSystem *market_date_system;
    NotificationSystem *notification_system;

    User *current_user;
    Date current_date;

    Market(UserSystem *user_system, MarketDateSystem *booking_system, NotificationSystem *notification_system, QWidget *parent = nullptr);
    ~Market();

    void handle_dashboard();
    void handle_market_schedule();
    void handle_edit_information();
    void save_user_information();
    void display_account_information(QListWidget * list, User * user);
    void display_market_information(QTableWidget * table, User * user);

public slots:
public emit:
};
