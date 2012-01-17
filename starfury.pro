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
    strob/strob.cpp \
    strob/strobgeometry.cpp \
    utils/cvhelpfun.cpp \
    boxes/frame.cpp \
    com/sharedmem.cpp \
    com/framerec.cpp \
    com/snudpsrv.cpp \
    boxes/artifact.cpp \
    gui/drawing.cpp \
    sky/skysegment.cpp \
    sky/starcatreader.cpp \
    boxes/star.cpp \
    utils/guidecodec.cpp \
    sky/starcatscreen.cpp \
    astrometry/angmeter.cpp \
    math/astrocalc.cpp \
    math/lincor.cpp \
    common/logfile.cpp \
    utils/timeutils.cpp \
    boxes/telescope.cpp \
    astrometry/freevec/freevec.cpp \
    astrometry/simtri/triangle.cpp \
    astrometry/simtri/combo.cpp \
    astrometry/astrometry.cpp \
    gui/controlwindow.cpp \
    gui/imagewindow.cpp \
    gui/strobwnd.cpp \
    detector/detector.cpp \
    detector/detection.cpp \
    detector/accumulator.cpp \
    strob/strob_wrapper.cpp \
    gui/detector_wnd.cpp \
    gui/photometry_wnd.cpp \
    sky/velocimeter.cpp

HEADERS  += \
    strob/strob.h \
    strob/strobgeometry.h \
    utils/cvhelpfun.h \
    boxes/frame.h \
    com/sharedmem.h \
    com/framerec.h \
    com/snudpsrv.h \
    boxes/artifact.h \
    gui/drawing.h \
    sky/skysegment.h \
    sky/starcatreader.h \
    boxes/star.h \
    utils/guidecodec.h \
    sky/starcatscreen.h \
    math/astrocalc.h \
    common/globalskeys.h \
    astrometry/angmeter.h \
    boxes/databox.h \
    math/lincor.h \
    common/logfile.h \
    utils/timeutils.h \
    boxes/telescope.h \
    astrometry/freevec/freevec.h \
    astrometry/simtri/triangle.h \
    astrometry/simtri/combo.h \
    astrometry/astrometry.h \
    gui/controlwindow.h \
    gui/imagewindow.h \
    gui/strobwnd.h \
    detector/detector.h \
    detector/detection.h \
    detector/accumulator.h \
    strob/strob_wrapper.h \
    gui/detector_wnd.h \
    gui/photometry_wnd.h \
    sky/velocimeter.h


INCLUDEPATH += "C:/opencv231/include"

LIBS += -L"C:/opencv231/lib" \
-l"opencv_calib3d231" \
-l"opencv_contrib231" -l"opencv_core231" -l"opencv_features2d231" -l"opencv_flann231" \
-l"opencv_gpu231" -l"opencv_highgui231" -l"opencv_imgproc231" -l"opencv_legacy231" \
-l"opencv_ml231" -l"opencv_objdetect231" -l"opencv_ts231" -l"opencv_video231"


INCLUDEPATH += "C:/Qwt-6.0.1/include"

LIBS += -L"C:/Qwt-6.0.1/lib" \
        "libqwt" \
        "libqwtd" \
        "libqwtmathml" \
        "libqwtmathmld"


FORMS += \
    gui/controlwindow.ui















































































































































































