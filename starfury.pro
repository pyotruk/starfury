#-------------------------------------------------
#
# Project created by QtCreator 2011-10-18T18:14:41
#
#-------------------------------------------------

QT       += core gui \
         network
TARGET = starfury
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    strob/strob.cpp \
    strob/strobgeometry.cpp \
    utils/cvhelpfun.cpp \
    boxes/frame.cpp \
    com/sharedmem.cpp \
    com/framerec.cpp \
    com/snudpsrv.cpp \
    boxes/artifact.cpp \
    utils/drawhelpfun.cpp \
    sky/skysegment.cpp \
    sky/starcatreader.cpp \
    boxes/star.cpp \
    utils/guidecodec.cpp \
    sky/starcatscreen.cpp \
    detector/stardetector.cpp \
    astrometry/angmeter.cpp \
    math/astrocalc.cpp \
    math/lincor.cpp \
    common/logfile.cpp \
    utils/timeutils.cpp \
    boxes/telescope.cpp \
    astrometry/freevec/freevec.cpp \
    astrometry/simtri/triangle.cpp \
    astrometry/simtri/combo.cpp

HEADERS  += mainwindow.h \
    strob/strob.h \
    strob/strobgeometry.h \
    utils/cvhelpfun.h \
    boxes/frame.h \
    com/sharedmem.h \
    com/framerec.h \
    com/snudpsrv.h \
    boxes/artifact.h \
    utils/drawhelpfun.h \
    sky/skysegment.h \
    sky/starcatreader.h \
    boxes/star.h \
    utils/guidecodec.h \
    sky/starcatscreen.h \
    detector/stardetector.h \
    math/astrocalc.h \
    common/globalskeys.h \
    astrometry/angmeter.h \
    boxes/databox.h \
    math/lincor.h \
    boxes/combo.h \
    common/logfile.h \
    utils/timeutils.h \
    boxes/telescope.h \
    astrometry/freevec/freevec.h \
    astrometry/simtri/triangle.h \
    astrometry/simtri/combo.h


INCLUDEPATH += "C:/opencv231/include"


LIBS += -L"C:/opencv231/lib" \
-l"opencv_calib3d231" \
-l"opencv_contrib231" -l"opencv_core231" -l"opencv_features2d231" -l"opencv_flann231" \
-l"opencv_gpu231" -l"opencv_highgui231" -l"opencv_imgproc231" -l"opencv_legacy231" \
-l"opencv_ml231" -l"opencv_objdetect231" -l"opencv_ts231" -l"opencv_video231"


FORMS += mainwindow.ui















































































































































