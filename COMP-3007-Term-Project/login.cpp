#include "hinton_market.h"
#include "login.h"

LoginDialog::LoginDialog(UserSystem *user_system, QWidget *parent)
    : QDialog(parent)
    , user_system(user_system)
{
    setWindowTitle("Hinton Market Login");
    resize(400, 100);

    layout = new QVBoxLayout(this);
    login_button = new QPushButton("Login", this);

    user_text_field = new QLineEdit(this);
    user_text_field->setPlaceholderText("Username");

    layout->addWidget(user_text_field);
    layout->addWidget(login_button);

    connect(login_button, &QPushButton::clicked, this, &LoginDialog::attempt_login);
}

LoginDialog::LoginDialog::~LoginDialog()
{
    QLayoutItem *item;
    QWidget *widget;
    for(;;)
    {
        item = layout->takeAt(0);
        if(!item) { break; }

        // dbc: Sublayouts are not handled
        widget = item->widget();
        if(widget != 0)
        {
            widget->hide();
            delete widget;
        }
        else
        {
            delete item;
        }
    }

    delete layout;
}

void
LoginDialog::attempt_login(void)
{
    QString username = user_text_field->text();
    Credentials credentials;
    credentials.username = username.toStdString();

    User *user = user_system->get_user(credentials);

    if(user != nullptr)
    {
        QString qs = QString("Logged in as %1")
                .arg(username);

        QMessageBox::information(this, "Login Success", qs);
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials");
    }
};
