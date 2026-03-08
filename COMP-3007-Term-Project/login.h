#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QMainWindow> //
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>

class LoginDialog: public QDialog
{
    Q_OBJECT

public:
    QVBoxLayout *layout;
    QPushButton *loginButton;
    QLineEdit *userTextField;
    QLineEdit *passTextField;

    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

public slots:
    void attemptLogin();
};

#endif
