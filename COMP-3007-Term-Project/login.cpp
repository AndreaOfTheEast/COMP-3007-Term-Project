#pragma once
#include "hinton_market.h"
#include "logindialog.h"

#include <QPushButton>
#include <QLineEdit>
#include <QMainWindow> //
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QMessageBox>

// andwu: TODO: actually make this a ui thing

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Hinton Market Login");

    layout = new QVBoxLayout(this);
    loginButton = new QPushButton("Login", this);

    userTextField = new QLineEdit(this);
    userTextField->setPlaceholderText("Email");

    passTextField = new QLineEdit(this);
    passTextField->setPlaceholderText("Password");
    passTextField->setEchoMode(QLineEdit::Password);

    layout->addWidget(loginButton);
    layout->addWidget(userTextField);
    layout->addWidget(passTextField);
    
    /*
    QObject::connect(loginButton, &QPushButton::clicked, [=]() {
	std::cout << "Text entered: " << textField->text();
    });
    */

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::attemptLogin);
}

LoginDialog::~LoginDialog()
{
    QLayoutItem *item;
    QWidget *widget;
    while ((item = layout->takeAt(0)))
    {
	// dbc: Sublayouts are not handled
	if ((widget = item->widget()) != 0)
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

void LoginDialog::attemptLogin()
{
    QString username = userTextField->text();
    QString password = passTextField->text();

    if(username == "admin" && password == "123")
    {
	accept();
    }
    else
    {
	QMessageBox::warning(this, "Login Failed", "Invalid credentials");
	passTextField->clear();
    }
}
