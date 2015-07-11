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

#include "mainWindow.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QFrame>

#include <QDebug>

#ifdef MIDI_ALSA
#include "midiThread.h"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    timbreWidget   = new TimbreWidget(8);
    waveformWidget = new WaveformWidget;
    waveformPlot   = new WaveformPlot;
    envelopeWidget = new ADSRWidget;
    kbWidget       = new KBWidget;

    connect(waveformWidget, SIGNAL(modeSelected(int)),
            waveformPlot, SLOT(setMode(int)));
    connect(timbreWidget, SIGNAL(settingsChanged(QVector<int>&,QVector<int>&)),
            waveformPlot, SLOT(setTimbre(QVector<int>&,QVector<int>&)));

    gridLayout = new QGridLayout;
    QWidget * gridWidget = new QWidget();
    gridWidget->setLayout(gridLayout);
    setCentralWidget(gridWidget);

    QGroupBox *waveFormGroup = new QGroupBox(tr("Oscillator"));
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(waveformWidget);
    waveFormGroup->setLayout(hbox);
    gridLayout->addWidget(waveFormGroup, 1, 0);

    QGroupBox *envelopeGroup = new QGroupBox(tr("Envelope"));
    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addWidget(envelopeWidget);
    envelopeGroup->setLayout(hbox2);
    gridLayout->addWidget(envelopeGroup, 1, 1);

    QGroupBox *timbreGroup = new QGroupBox(tr("Timbre"));
    QHBoxLayout *hbox3 = new QHBoxLayout;
    hbox3->addWidget(timbreWidget);
    hbox3->addWidget(waveformPlot);
    timbreGroup->setLayout(hbox3);
    gridLayout->addWidget(timbreGroup, 2, 0, 1, 2);


    modulationWidget = new ModulationWidget;
    QGroupBox *modulationGroup = new QGroupBox(tr("Modulation"));
    QHBoxLayout *hbox4 = new QHBoxLayout;
    hbox4->addWidget(modulationWidget);
    modulationGroup->setLayout(hbox4);

    gridLayout->addWidget(modulationGroup, 3, 0, 1, 2);

    gridLayout->addWidget(kbWidget, 4, 0, 1, 2);
    gridLayout->setRowMinimumHeight(4, 100);

#ifdef USE_FFTW
    fftPlot = new FFTPlot();
    gridLayout->addWidget(fftPlot, 0, 0, 1, 2);
    gridLayout->setRowMinimumHeight(0, 150);
#else

#endif

    bufferSize = 8192;

    m_format.setSampleRate(44100);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_buffer = QByteArray(bufferSize*2, 0);

    m_audioOutput = 0;
    delete m_audioOutput;
    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_audioOutput->setBufferSize(bufferSize);
    m_generator   = new Generator(m_format, this);
    m_generator->start();
    m_audioOutput->start(m_generator);
    m_audioOutput->setVolume(1);

    connect(kbWidget, SIGNAL(sig_note_on(unsigned char,unsigned char,unsigned char)),
            m_generator, SLOT(noteOn(unsigned char,unsigned char,unsigned char)));
    connect(kbWidget, SIGNAL(sig_note_off(unsigned char,unsigned char)),
            m_generator, SLOT(noteOff(unsigned char,unsigned char)));
    connect(timbreWidget, SIGNAL(settingsChanged(QVector<int>&,QVector<int>&)),
            m_generator, SLOT(setTimbre(QVector<int>&,QVector<int>&)));
    connect(waveformWidget, SIGNAL(modeSelected(int)),
            m_generator, SLOT(setMode(int)));
    connect(envelopeWidget, SIGNAL(envelopeChanged(ADSREnvelope&)),
            m_generator, SLOT(setEnvelope(ADSREnvelope&)));
    connect(modulationWidget, SIGNAL(setModulation(Modulation &)),
            m_generator, SLOT(setModulation(Modulation &)));
#ifdef USE_FFTW
    connect(m_generator, SIGNAL(fftUpdate(fftw_complex*,uint)),
            fftPlot, SLOT(fftUpdate(fftw_complex*,uint)));
#endif
#ifdef MIDI_ALSA
    midiThread = new MidiThread(tr("hw:1,0,0"));
    midiThread->start();
    connect(midiThread, SIGNAL(note_on(unsigned char,unsigned char,unsigned char)),
            kbWidget, SLOT(note_on(unsigned char,unsigned char,unsigned char)));
    connect(midiThread, SIGNAL(note_off(unsigned char,unsigned char)),
            kbWidget, SLOT(note_off(unsigned char,unsigned char)));

    connect(midiThread, SIGNAL(note_on(unsigned char,unsigned char,unsigned char)),
            this, SLOT(note_on(unsigned char,unsigned char,unsigned char)));
    connect(midiThread, SIGNAL(note_off(unsigned char,unsigned char)),
            this, SLOT(note_off(unsigned char,unsigned char)));
#endif
}

MainWindow::~MainWindow()
{

}
