/* miniSynth - A Simple Software Synthesizer
   Copyright (C) 2015 Ville Räisänen <vsr at vsr.name>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include <QAudioDeviceInfo>
#include <QAudioOutput>

#include "timbreWidget.h"
#include "waveformWidget.h"
#include "waveformPlot.h"
#include "ADSRwidget.h"
#include "kbWidget.h"
#include "outputQt.h"
#include "modulationWidget.h"
#include "fftplot.h"
#include "filterWidget.h"
#include "reverbWidget.h"
#include "presetWidget.h"

#ifdef MIDI_ALSA
#include "midiThread.h"
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setPreset(Preset &preset);
private:
    unsigned int bufferSize;

    TimbreWidget     *timbreWidget;
    WaveformWidget   *waveformWidget;
    WaveformPlot     *waveformPlot;
    ADSRWidget       *envelopeWidget;
    ModulationWidget *modulationWidget;
    KBWidget         *kbWidget;
#ifdef USE_FFTW
    FFTPlot          *fftPlot;
#endif
    FilterWidget     *filterWidget;
    ReverbWidget     *reverbWidget;
    PresetWidget     *presetWidget;

    Generator       *m_generator;
    QAudioDeviceInfo m_device;
    QAudioFormat     m_format;
    QByteArray       m_buffer;
    QAudioOutput    *m_audioOutput;
    QIODevice       *m_output;

    QGridLayout *gridLayout;
#ifdef MIDI_ALSA
    MidiThread *midiThread;
#endif
};

#endif // MAINWINDOW_H
