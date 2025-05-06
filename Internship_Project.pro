QT       += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Path to OpenCV installation
OPENCV_DIR = C:/opencv/build


INCLUDEPATH += /usr/include/hpdf
LIBS += -lhpdf


INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_dnn \
        -lopencv_videoio


SOURCES += \
    header.cpp \
    jsonhandler.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    homewindow.cpp \
    pdfreportgenerator.cpp \
    report.cpp

HEADERS += \
    header.h \
    jsonhandler.h \
    loginwindow.h \
    homewindow.h \
    mainwindow.h \
    pdfreportgenerator.h \
    report.h \
    json.hpp

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
    reportData.json \
    userData.json
