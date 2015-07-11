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


#include "outputQt.h"
#include <qendian.h>
#include <QDebug>
#include <QFile>

Generator::Generator(const QAudioFormat &_format, QObject *parent) : QIODevice(parent) {
    format = _format;
    linSyn = new LinearSynthesis(Waveform::MODE_SIN);
    curtime = 0;

    env.attackTime = 100;
    env.decayTime = 400;
    env.releaseTime = 100;

    env.initialAmpl = 0;
    env.peakAmpl = 1;
    env.sustainAmpl = 0.8;

    mod_waveform = new Waveform(Waveform::MODE_SIN);

    use_convolution = true;
    convBuffer_size = 44100;
    convBuffer      = new qreal[convBuffer_size];
    convImpulse     = new qreal[convBuffer_size];
    convBuffer_ind  = 0;
    for (unsigned int indconv = 0; indconv < convBuffer_size; indconv++) {
        convBuffer[indconv]  = 0;
        convImpulse[indconv] = 0;
    }
/*
    QFile file("hst.mat");
    if (!file.open(QIODevice::ReadOnly)) {
        exit(0);
    }
    QTextStream in(&file);

    int ind = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << line.toDouble();
        convImpulse[ind++] = 20*line.toDouble();
        if (ind == convBuffer_size) break;
    }
*/
#ifdef USE_FFTW
    fftwIn  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*convBuffer_size);
    fftwOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*convBuffer_size);
    fftwPlan= fftw_plan_dft_1d(convBuffer_size, fftwIn, fftwOut,
                                FFTW_FORWARD, FFTW_ESTIMATE);
#endif
}

Generator::~Generator() {
    delete linSyn;
    delete [] convBuffer;
    delete [] convImpulse;

#ifdef USE_FFTW
    fftw_destroy_plan(fftwPlan);
    fftw_free(fftwIn);
    fftw_free(fftwOut);
#endif
}

void
Generator::start() {
    open(QIODevice::ReadOnly);
}

void
Generator::stop() {
    close();
}

void
Generator::addWave(unsigned char note, unsigned char vel) {
    Wave wav;
    wav.state = wav.STATE_ATTACK;
    wav.note  = note;
    wav.vel   = vel;
    wav.state_age = 0;

    waveList.push_back(wav);
}

qint64
Generator::readData(char *data, qint64 len) {
    generateData(len);
    memcpy(data, m_buffer.constData(), len);
    curtime += (qreal)len/(44100*2);
    return len;
}

qint64
Generator::writeData(const char *data, qint64 len) {
    Q_UNUSED(data);
    Q_UNUSED(len);
    return 0;
}

qint64
Generator::bytesAvailable() const {
    return m_buffer.size() + QIODevice::bytesAvailable();
}

void
Generator::noteOn(unsigned char chan, unsigned char note, unsigned char vel) {
    qDebug() << "NOTEON" << chan << note << vel;
    if (vel > 0) vel = 255;
    addWave(note, vel);
    Q_UNUSED(chan);
}

void
Generator::noteOff(unsigned char chan, unsigned char note) {
    QMutableListIterator<Wave> i(waveList);

    while (i.hasNext()) {
        Wave wav = i.next();
        if (wav.note == note) {
            wav.state = Wave::STATE_RELEASE;
            wav.state_age = 0;
        }
        i.setValue(wav);
    }
    Q_UNUSED(chan);
}

void
Generator::setMode(int _mode) {
    delete linSyn;
    linSyn = new LinearSynthesis(_mode);
    curtime = 0;
}

void
Generator::setTimbre(QVector<int> &amplitudes, QVector<int> &phases) {
    linSyn->setTimbre(amplitudes, phases);
}

void
Generator::generateData(qint64 len) {
    QMutableListIterator<Wave> i(waveList);

    unsigned int numSamples = len/2;
    m_buffer.resize(len);
    qreal attackTime  = 0.001*(qreal)env.attackTime,
          decayTime   = 0.001*(qreal)env.decayTime,
          releaseTime = 0.001*(qreal)env.releaseTime;
    Q_UNUSED(decayTime);

    QVector<qreal> floatData = QVector<qreal>(numSamples, 0);
    //qDebug() << numSamples;

    while (i.hasNext()) {
        Wave wav = i.next();
        qreal freq = 8.175 * qPow(2, ((qreal)wav.note)/12);
        qreal ampl = 0.5*((qreal)wav.vel)/256;

        qreal age = wav.state_age;

        for (unsigned int sample = 0; sample < numSamples; sample++) {
            qreal t = curtime + (qreal)sample / 44100;
            qreal envt = age + (qreal)sample / 44100;

            switch(wav.state) {
            case ADSREnvelope::STATE_ATTACK:
                //qDebug() << "ATTACK" << envt << env.attackTime;
                if (envt > attackTime) {
                    age -= attackTime;
                    wav.state = ADSREnvelope::STATE_DECAY;
                    wav.state_age -= attackTime;
                    qDebug() << "Decay";
                }
                break;
            case ADSREnvelope::STATE_RELEASE:
                if (envt > releaseTime) {
                    qDebug() << "OFF";
                    age = 0;
                    wav.state = ADSREnvelope::STATE_OFF;
                }
                break;
            }
            if (wav.state == ADSREnvelope::STATE_OFF) {
                i.remove();
            } else {
                qreal freqmod = 0, amod = 0;

                if (mod.FM_freq > 0) {
                    if (mod.propFreq) {
                        freqmod = mod.FM_ampl
                                * mod_waveform->eval(2*M_PI*mod.FM_freq*freq*t);
                    } else {
                        freqmod = mod.FM_ampl
                                * mod_waveform->eval(2*M_PI*mod.FM_freq*t);
                    }
                }
                if (mod.AM_freq > 0) {
                    amod = mod.AM_ampl * mod_waveform->eval(2*M_PI*mod.AM_freq*t);
                }

                qreal envVal = env.eval(envt, wav.state);
                qreal newVal = envVal * (ampl + amod)
                             * linSyn->evalTimbre(2*M_PI*(freq+freqmod)*t);
                qreal oldVal = floatData[sample];

                floatData[sample] = newVal + oldVal;
            }
        }
        if (wav.state != ADSREnvelope::STATE_OFF) {
            wav.state_age += (qreal)numSamples/44100;
            i.setValue(wav);
        }
    }

    for (int sample = 0; sample < numSamples; sample++) {
        convBuffer[convBuffer_ind] = floatData[sample];
/*
        qreal out = 0;
        for (int convind = 0; convind < convBuffer_size; convind ++) {
            // The term convBuffer_size keeps the left side non-negative and avoids
            // negative results from the modulo operator.
            if (convImpulse[convind] != 0) {
                int bufind = (convBuffer_ind + convBuffer_size - convind) % convBuffer_size;

                out += convImpulse[convind] * convBuffer[bufind];
            }
        }
        floatData[sample] = out;
        */
        convBuffer_ind = (convBuffer_ind + 1) % convBuffer_size;
    }
#ifdef USE_FFTW
    if (numSamples > 4096) {
        for (unsigned int convind = 0; convind < convBuffer_size; convind++) {
            fftwIn[convind][0] = convBuffer[convind];
            fftwIn[convind][1] = 0;
        }
        fftw_execute(fftwPlan);
        emit fftUpdate(fftwOut, convBuffer_size);
    }
#endif

    const int channelBytes = format.sampleSize() / 8;
    const int sampleBytes  = format.channelCount() * channelBytes;
    Q_UNUSED(sampleBytes);

    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());

    for (unsigned int sample = 0; sample < numSamples; sample++) {
        if (floatData[sample] > 1) floatData[sample] = 1;
        if (floatData[sample] < -1) floatData[sample] = -1;
        qint16 value = static_cast<qint16>(floatData[sample] * 32767);
        qToLittleEndian<qint16>(value, ptr);
        ptr += channelBytes;
    }
}

void
Generator::setEnvelope(ADSREnvelope &_env) {
    env = _env;
}

void
Generator::setModulation(Modulation &modulation) {
    if (modulation.mode != mod_waveform->mode) {
//        qDebug() << "MODE" << modulation.mode;
        delete mod_waveform;
        mod_waveform = new Waveform(modulation.mode);
    }
    mod = modulation;
//    qDebug() << mod.propFreq
//             << mod.AM_ampl << mod.AM_freq
//             << mod.FM_ampl << mod.FM_freq;
}
