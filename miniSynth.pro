#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T06:14:26
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += midi_alsa
#CONFIG += use_fftw
QMAKE_CXXFLAGS = -O3

use_fftw {
DEFINES += USE_FFTW
LIBS += -lfftw3 -lm
}

midi_alsa {
DEFINES += MIDI_ALSA
LIBS += -lasound
}

TARGET = miniSynth
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    ADSRenvelope.cpp \
    ADSRplot.cpp \
    ADSRwidget.cpp \
    kbWidget.cpp \
    linearSynthesis.cpp \
    midiThread.cpp \
    timbreWidget.cpp \
    waveform.cpp \
    waveformPlot.cpp \
    waveformWidget.cpp \
    outputQt.cpp \
    modulationWidget.cpp \
    modulation.cpp \
    fftplot.cpp

HEADERS  += mainWindow.h \
    ADSRenvelope.h \
    ADSRplot.h \
    ADSRwidget.h \
    kbWidget.h \
    linearSynthesis.h \
    midiThread.h \
    timbreWidget.h \
    waveform.h \
    waveformPlot.h \
    waveformWidget.h \
    outputQt.h \
    modulationWidget.h \
    modulation.h \
    fftplot.h
