#pragma once

#include "hinton_market.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    QVBoxLayout *layout;
    QPushButton *login_button;
    QLineEdit *user_text_field;

    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

public slots:
    void attempt_login(void);
};
