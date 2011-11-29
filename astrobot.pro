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
    strobgeometry.cpp \
    cvhelpfun.cpp \
    frame.cpp \
    sharedmem.cpp \
    framerec.cpp \
    snudpsrv.cpp \
    artifact.cpp \
    drawhelpfun.cpp \
    skysegment.cpp \
    starcatreader.cpp \
    star.cpp \
    guidecodec.cpp \
    starcatscreen.cpp \
    stardetector.cpp \
    equator.cpp \
    adapters.cpp \
    artifactbox.cpp \
    arttriangle.cpp

HEADERS  += mainwindow.h \
    strob.h \
    strobgeometry.h \
    cvhelpfun.h \
    frame.h \
    sharedmem.h \
    framerec.h \
    snudpsrv.h \
    artifact.h \
    drawhelpfun.h \
    skysegment.h \
    starcatreader.h \
    star.h \
    guidecodec.h \
    starcatscreen.h \
    stardetector.h \
    astrocalc.h \
    equator.h \
    adapters.h \
    artifactbox.h \
    globalskeys.h \
    arttriangle.h

INCLUDEPATH += "C:/opencv231/include"
INCLUDEPATH += "../astrobot/astrocalc"


LIBS += -L"C:/opencv231/lib" \
-l"opencv_calib3d231" \
-l"opencv_contrib231" -l"opencv_core231" -l"opencv_features2d231" -l"opencv_flann231" \
-l"opencv_gpu231" -l"opencv_highgui231" -l"opencv_imgproc231" -l"opencv_legacy231" \
-l"opencv_ml231" -l"opencv_objdetect231" -l"opencv_ts231" -l"opencv_video231"

LIBS += -L"../astrobot/astrocalc/debug" \
        -l"astrocalc"

FORMS += mainwindow.ui


















































































































