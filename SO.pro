QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    algorithms.hpp \
    concurrent_queue.hpp \
    mainwidget.hpp \
    menuwidget.hpp \
    process.hpp \
    simulationwidget.hpp \
    titlewidget.hpp

SOURCES += \
    algorithms.cpp \
    main.cpp \
    mainwidget.cpp \
    menuwidget.cpp \
    process.cpp \
    simulationwidget.cpp \
    titlewidget.cpp
