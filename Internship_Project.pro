QT       += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Path to OpenCV installation
OPENCV_DIR = C:/opencv/build

INCLUDEPATH += /opt/GVision/include
LIBS += -L/opt/GVision/lib -lGVision
QMAKE_LFLAGS += -Wl,-rpath /opt/GVision/lib

INCLUDEPATH += /opt/genicam/include
LIBS += -L/opt/genicam/lib -lGCBase_gcc48_v3_4 -lGenApi_gcc48_v3_4

INCLUDEPATH += /usr/include/hpdf
LIBS += -lhpdf


INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_videoio


SOURCES += \
    header.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    homewindow.cpp \
    report.cpp

HEADERS += \
    header.h \
    loginwindow.h \
    homewindow.h \
    mainwindow.h \
    report.h

FORMS += \
    header.ui \
    loginwindow.ui \
    mainwindow.ui \
    homewindow.ui \
    report.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data.json
