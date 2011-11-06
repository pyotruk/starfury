#-------------------------------------------------
#
# Project created by QtCreator 2011-10-18T18:14:41
#
#-------------------------------------------------

QT       += core gui \
         network
TARGET = astrobot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    strob.cpp \
    rapidthread.cpp \
    strobgeometry.cpp \
    cvhelpfun.cpp \
    frame.cpp \
    angmeas.cpp \
    starcat.cpp \
    sharedmem.cpp \
    framerec.cpp \
    snudpsrv.cpp

HEADERS  += mainwindow.h \
    strob.h \
    rapidthread.h \
    strobgeometry.h \
    cvhelpfun.h \
    frame.h \
    angmeas.h \
    starcat.h \
    sharedmem.h \
    framerec.h \
    snudpsrv.h

INCLUDEPATH += "C:\opencv231\include"

LIBS += -L"C:\opencv231\lib" \
-l"opencv_calib3d231" \
-l"opencv_contrib231" -l"opencv_core231" -l"opencv_features2d231" -l"opencv_flann231" \
-l"opencv_gpu231" -l"opencv_highgui231" -l"opencv_imgproc231" -l"opencv_legacy231" \
-l"opencv_ml231" -l"opencv_objdetect231" -l"opencv_ts231" -l"opencv_video231"

FORMS    += mainwindow.ui



















































