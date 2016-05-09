#-------------------------------------------------
#
# Project created by QtCreator 2016-04-30T14:24:43
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += multimediawidgets
QT       += concurrent

CONFIG   += thread

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TSRDApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    classifier.cpp \
    processor.cpp \
    opennetdialog.cpp \
    util.cpp \
    frameprobevsurface.cpp \
    caffenetbuilder.cpp \
    abstractoutput.cpp \
    drawingoutput.cpp

HEADERS  += mainwindow.h \
    classifier.h \
    processor.h \
    opennetdialog.h \
    util.h \
    frameprobevsurface.h \
    abstractclassifier.h \
    abstractdetector.h \
    abstractclassifierbuilder.h \
    abstractdetectorbuilder.h \
    caffenetbuilder.h \
    abstractoutput.h \
    drawingoutput.h

FORMS    += mainwindow.ui \
    opennetdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../builds/py-faster-rcnn/caffe-fast-rcnn/build/lib/release/ -lcaffe
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../builds/py-faster-rcnn/caffe-fast-rcnn/build/lib/debug/ -lcaffe
else:unix: LIBS += -L$$PWD/../../../builds/py-faster-rcnn/caffe-fast-rcnn/build/lib/ -lcaffe

INCLUDEPATH += $$PWD/../../../builds/py-faster-rcnn/caffe-fast-rcnn/include
DEPENDPATH += $$PWD/../../../builds/py-faster-rcnn/caffe-fast-rcnn/include

# Caffe from aur
#win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lcaffe
#else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lcaffe
#else:unix: LIBS += -L/usr/lib/ -lcaffe

#INCLUDEPATH += /opt/caffe/include
#DEPENDPATH += /opt/caffe/include

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lcuda
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lcuda
else:unix: LIBS += -L/usr/lib/ -lcuda

INCLUDEPATH += /opt/cuda/include
DEPENDPATH += /opt/cuda/include

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lglog
else:unix: LIBS += -L/usr/lib/ -lglog

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lboost_system
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lboost_system
else:unix: LIBS += -L/usr/lib/ -lboost_system

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lopencv_core
else:unix: LIBS += -L/usr/lib/ -lopencv_core

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lopencv_imgproc
else:unix: LIBS += -L/usr/lib/ -lopencv_imgproc

win32:CONFIG(release, debug|release): LIBS += -L/usr/lib/release/ -lopencv_imgcodecs
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/lib/debug/ -lopencv_imgcodecs
else:unix: LIBS += -L/usr/lib/ -lopencv_imgcodecs

INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lopencv_xobjdetect
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lopencv_xobjdetect
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lopencv_xobjdetect
