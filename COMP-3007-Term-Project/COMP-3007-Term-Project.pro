QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -Wpedantic -Wconversion -Wfloat-conversion -Wsign-conversion -Wdouble-promotion -Wshadow -Wcast-align -Wdeprecated-declarations -Wmissing-field-initializers -Wfloat-equal -Wpointer-arith -Wtautological-compare -Wno-missing-braces -Wno-unused-parameter -Wno-unused-function -Wno-unused-value -Wno-unused-variable -Wno-unused-local-typedef -Wno-unused-but-set-variable -Wno-initializer-overrides -Wno-gnu-zero-variadic-macro-arguments -fmax-errors=0

SOURCES += \
    login.cpp \
    main.cpp \
    market.cpp \
    market_model.cpp

HEADERS += \
    hinton_market.h \
    login.h \
    market.h \
    market_model.h

copydb.commands = test -f $$OUT_PWD/hintonMarket.db || cp $$PWD/hintonMarket.db $$OUT_PWD/
QMAKE_EXTRA_TARGETS += copydb
PRE_TARGETDEPS += copydb

FORMS += \
    market.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
