#include "mainwindow.h"
#include "logindialog.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <QApplication>
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog login;

    if (login.exec() == QDialog::Accepted)
    {
	    MainWindow w;
	    w.show();
	    return a.exec();
    }

    return 0;
}
