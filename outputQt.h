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

#ifndef OUTPUTQT_H
#define OUTPUTQT_H

#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QByteArray>
#include <QIODevice>

#include <QList>
#include <QMutableListIterator>
#include <QMutex>

#include "linearSynthesis.h"
#include "ADSRenvelope.h"
#include "modulation.h"
#include "filter.h"
#include "ADSRenvelope.h"
#include "reverb.h"

#ifdef USE_FFTW
#include <fftw3.h>
#else
#include "fft.h"
#endif

// The state of each active note is described with an Wave object. Wave
// objects are assembled into the QList<Wave> waveList and removed once
// they reach the state STATE_OFF.

class Wave {
public:
    enum {STATE_OFF, STATE_ATTACK, STATE_DECAY, STATE_RELEASE};
    unsigned char note, vel, state;
    qreal state_age, age;
    ADSREnvelope env;
};

// The synthesizer is implemented as a QIODevice and is connected to
// a QAudioOutput in mainWindow.cpp. QAudioOutput reads data from the
// synthersizer using the function readData(data, size). readData
// returns maximum of 2048 samples generated with generateData(len).

class Generator : public QIODevice {
    Q_OBJECT
public:
    Generator(const QAudioFormat &_format, QObject *parent = 0);
    ~Generator();

    void start    ();
    void stop     ();
    void setState ();

    void addWave (unsigned char note, unsigned char vel);

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

    void generateData(qint64 len);
signals:
#ifdef USE_FFTW
    void fftUpdate(fftw_complex *out, unsigned int size, unsigned int ind_dataset);
#else
    void fftUpdate(std::complex<qreal> *out, unsigned int size, unsigned int ind_dataset);
#endif
public slots:
    void noteOn   (unsigned char chan, unsigned char note, unsigned char vel);
    void noteOff  (unsigned char chan, unsigned char note);

    // Slots for manipulation of the current patch.
    void setMode      (int _mode);
    void setTimbre    (QVector<int> &amplitudes, QVector<int> &phases);
    void setEnvelope  (ADSREnvelope &env);
    void setModulation(Modulation &modulation);
    void setFilter    (FilterParameters &filtParam);
    void setReverb    (Reverb &_rev);
private:
    QAudioFormat format;
    QByteArray m_buffer;

    // State of the syntheziser
    qreal curtime;
    QList<Wave> waveList;

    // Parameters of the current patch
    LinearSynthesis *linSyn;
    ADSREnvelope     defaultEnv;
    Modulation       mod;
    Waveform        *mod_waveform;
    Filter          *filter;
    Reverb           rev;

    // convBuffer and filtBuffer are circular buffers used to store the
    // previous convBuffer_size samples before and after convolution.
    // Both buffers are used as inputs for the FFT in the computation of
    // data for the fftUpdate signal. The data in convBuffer is also used
    // in the computation of convolution. convBuffer_ind tells the location
    // of the last added sample in the buffer.

    qreal *convBuffer, *filtBuffer, *delayBuffer;
    unsigned int convBuffer_size, delayBuffer_size;
    quint32 convBuffer_ind, delayBuffer_ind;

    // Impulse response for the current filter.

    qreal *convImpulse;
    unsigned int convImpulse_size;

    qreal fftTimer;

    QMutex mutex;
#ifdef USE_FFTW
    fftw_complex *fftwIn, *fftwOut;
    fftw_plan     fftwPlan;
#else
    std::complex<qreal> *fftData;
    unsigned int fftLength;
#endif
};

#endif // OUTPUTQT_H
