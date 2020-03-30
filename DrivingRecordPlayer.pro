#-------------------------------------------------
#
# Project created by QtCreator 2020-03-30T08:30:34
#
#-------------------------------------------------

QT       += core gui xml testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrivingRecordPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        module/VideoSlider.cpp \
        settingList/pagemovetableview.cpp \
    module/video_player.cpp

HEADERS += \
        mainwindow.h \
        module/VideoSlider.h \
        settingList/pagemovetableview.h \
    module/video_player.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=  $$PWD/src/include
                $$PWD/src/include/opencv
                $$PWD/src/include/opencv2

#INCLUDEPATH +=  C:/IDE/opencv/buildOpencv/install/include
#                C:/IDE/opencv/buildOpencv/install/include/opencv
#                C:/IDE/opencv/buildOpencv/install/include/opencv2

LIBS += $$PWD/src/lib/libopencv_core343.dll.a\
        $$PWD/src/lib/libopencv_highgui343.dll.a\
        $$PWD/src/lib/libopencv_ml343.dll.a\
        $$PWD/src/lib/libopencv_imgproc343.dll.a\
        $$PWD/src/lib/libopencv_imgcodecs343.dll.a\
        $$PWD/src/lib/libopencv_stitching343.dll.a\
        $$PWD/src/lib/libopencv_calib3d343.dll.a\
        $$PWD/src/lib/libopencv_features2d343.dll.a\
        $$PWD/src/lib/libopencv_flann343.dll.a\
        $$PWD/src/lib/libopencv_video343.dll.a\
        $$PWD/src/lib/libopencv_objdetect343.dll.a\
        $$PWD/src/lib/libopencv_videoio343.dll.a\
        $$PWD/src/lib/libopencv_photo343.dll.a\
        $$PWD/src/lib/libopencv_shape343.dll.a\
        $$PWD/src/lib/libopencv_superres343.dll.a\
        $$PWD/src/lib/libopencv_videostab343.dll.a

RESOURCES += \
    res.qrc
