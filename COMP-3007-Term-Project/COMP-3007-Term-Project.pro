QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -Wpedantic -Wconversion -Wfloat-conversion -Wsign-conversion -Wdouble-promotion -Wshadow -Wcast-align -Wdeprecated-declarations -Wmissing-field-initializers -Wfloat-equal -Wpointer-arith -Wtautological-compare -Wno-missing-braces -Wno-unused-function -Wno-unused-value -Wno-unused-variable -Wno-unused-local-typedef -Wno-unused-but-set-variable -Wno-initializer-overrides

SOURCES += \
    dashboard.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    market.cpp

HEADERS += \
    hinton_market.h \
    mainwindow.h \
    logindialog.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
