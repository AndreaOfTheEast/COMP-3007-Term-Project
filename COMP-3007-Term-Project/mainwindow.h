#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QMainWindow>
#pragma GCC diagnostic pop

#include "hinton_market.h"
#include "login.cpp"
#include "dashboard.cpp"
#include "market.cpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    UserSystem user_system;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
public emit:
};
#endif // MAINWINDOW_H
