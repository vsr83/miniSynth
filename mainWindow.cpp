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

    timbreWidget   = new TimbreWidget(8, this);
    waveformWidget = new WaveformWidget(this);
    waveformPlot   = new WaveformPlot(this);
    envelopeWidget = new ADSRWidget(this);
    kbWidget       = new KBWidget(this);

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
    gridLayout->addWidget(waveFormGroup, 0, 0);

    QGroupBox *envelopeGroup = new QGroupBox(tr("Envelope"));
    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addWidget(envelopeWidget);
    envelopeGroup->setLayout(hbox2);
    gridLayout->addWidget(envelopeGroup, 0, 1);

    QGroupBox *timbreGroup = new QGroupBox(tr("Timbre"));
    QHBoxLayout *hbox3 = new QHBoxLayout;
    hbox3->addWidget(timbreWidget);
    hbox3->addWidget(waveformPlot);
    timbreGroup->setLayout(hbox3);
    gridLayout->addWidget(timbreGroup, 1, 0, 1, 2);


    modulationWidget = new ModulationWidget(this);
    QGroupBox *modulationGroup = new QGroupBox(tr("Modulation"));
    QHBoxLayout *hbox4 = new QHBoxLayout;
    hbox4->addWidget(modulationWidget);
    modulationGroup->setLayout(hbox4);

    gridLayout->addWidget(modulationGroup, 2, 0, 1, 2);

    gridLayout->addWidget(kbWidget, 3, 0, 1, 2);
    gridLayout->setRowMinimumHeight(3, 100);

    filterWidget = new FilterWidget(this);
    QGroupBox *filterGroup = new QGroupBox(tr("Filter"));
    QHBoxLayout *hbox5 = new QHBoxLayout;
    hbox5->addWidget(filterWidget);
    filterGroup->setLayout(hbox5);
    gridLayout->addWidget(filterGroup, 2, 2, 1, 2);

    reverbWidget = new ReverbWidget(44100, this);
    QGroupBox *reverbGroup = new QGroupBox(tr("Reverb"));
    QHBoxLayout *hbox6 = new QHBoxLayout;
    hbox6->addWidget(reverbWidget);
    reverbGroup->setLayout(hbox6);
    gridLayout->addWidget(reverbGroup, 3, 2, 1, 1);

    presetWidget = new PresetWidget(this);
    QGroupBox *presetGroup = new QGroupBox(tr("Presets"));
    QHBoxLayout *hbox7 = new QHBoxLayout;
    hbox7->addWidget(presetWidget);
    presetGroup->setLayout(hbox7);
    gridLayout->addWidget(presetGroup, 3, 3, 1, 1);


    fftPlot = new FFTPlot(3, 8192./44100.);//2*2048.0/44100);
    gridLayout->addWidget(fftPlot, 0, 2, 2, 2);
    gridLayout->setColumnMinimumWidth(2, 300);
    gridLayout->setColumnMinimumWidth(3, 300);

    QPen thinPen;
    thinPen.setStyle(Qt::SolidLine);
    thinPen.setWidth(2);
    thinPen.setColor(QColor(180, 180, 180));
    fftPlot->setPen(0, thinPen);

    QPen filterPen;
    filterPen.setStyle(Qt::SolidLine);
    filterPen.setWidth(1);
    filterPen.setColor(QColor(180, 0, 0));
    fftPlot->setPen(2, filterPen);

    bufferSize = 8192*2;

    m_format.setSampleRate(44100);
    m_format.setChannelCount(2);
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
    qWarning() << "Default device :" << m_device.deviceName();
    m_buffer = QByteArray(bufferSize*2, 0);

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
    connect(reverbWidget, SIGNAL(setReverb(Reverb&)), m_generator,
            SLOT(setReverb(Reverb&)));

    connect(filterWidget, SIGNAL(parametersChanged(FilterParameters&)),
            m_generator, SLOT(setFilter(FilterParameters&)));

    connect(presetWidget, SIGNAL(setPreset(Preset&)), this, SLOT(setPreset(Preset&)));
#ifdef USE_FFTW
    connect(m_generator, SIGNAL(fftUpdate(fftw_complex*,uint, uint)),
            fftPlot, SLOT(fftUpdate(fftw_complex*,uint, uint)));
#else
    connect(m_generator, SIGNAL(fftUpdate(std::complex<qreal>*,uint, uint)),
            fftPlot, SLOT(fftUpdate(std::complex<qreal>*,uint, uint)));
#endif
#ifdef MIDI_ALSA
    midiThread = new MidiThread(tr("hw:1,0,0"));
    midiThread->start();
    connect(midiThread, SIGNAL(note_on(unsigned char,unsigned char,unsigned char)),
            kbWidget, SLOT(note_on(unsigned char,unsigned char,unsigned char)));
    connect(midiThread, SIGNAL(note_off(unsigned char,unsigned char)),
            kbWidget, SLOT(note_off(unsigned char,unsigned char)));
#endif
}

void
MainWindow::setPreset(Preset &preset) {
    qDebug() << "setPreset";
    modulationWidget->importModulation(preset.mod);
    reverbWidget->importReverb(preset.rev);
    waveformWidget->setMode(preset.waveformMode);
    timbreWidget->setValues(preset.timbreAmplitudes, preset.timbrePhases);
    envelopeWidget->importEnvelope(preset.env);
    filterWidget->importFilter(preset.filt);
    m_generator->setFilter(preset.filt);
}

MainWindow::~MainWindow()
{
    m_audioOutput->stop();
    m_generator->stop();
    delete m_generator;
    delete fftPlot;
    delete m_audioOutput;
#ifdef MIDI_ALSA
    midiThread->stop();
    delete midiThread;
#endif
}
