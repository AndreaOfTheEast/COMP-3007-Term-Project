#pragma once

#include "hinton_market.h"
#include "market.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    QVBoxLayout *layout;
    QPushButton *login_button;
    QLineEdit *user_text_field;

    UserSystem *user_system;
    User *current_user;

    LoginDialog(UserSystem *user_system, QWidget *parent = nullptr);
    ~LoginDialog();

public slots:
    void attempt_login(void);
};
