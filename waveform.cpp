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

#include "waveform.h"
#include <QCoreApplication>
#include <QDebug>

Waveform::Waveform(unsigned int mode, unsigned int size) {
    waveBank = new qreal[size];
    bankSize = size;

    for (unsigned int sample = 0; sample < bankSize; sample++) {
        qreal t = (2*M_PI * (qreal)sample) / ((qreal)bankSize);

        switch(mode) {
        case MODE_SIN:
            waveBank[sample] = waveSin(t);
            break;
        case MODE_SAW:
            waveBank[sample] = waveSaw(t);
            break;
        case MODE_SQU:
            waveBank[sample] = waveSqu(t);
            break;
        }
    }
}

Waveform::~Waveform() {
    delete [] waveBank;
}

qreal
Waveform::waveSin(qreal t) {
    return qSin(t);
}

qreal
Waveform::waveSaw(qreal t) {
    qreal tmod = (qreal)(fmod((double)t, 2*M_PI) - M_PI);
    return tmod / M_PI;
}

qreal
Waveform::waveSqu(qreal t) {
    qreal tmod = (qreal)fmod((double)t, 2*M_PI);
    if (tmod < M_PI) {
        return 1;
    } else {
        return -1;
    }
}

// Perform linear interpolation between points of the wave bank.

qreal
Waveform::eval(qreal t) {
    qreal tmod = fmod((double)t, 2*M_PI);
    if (tmod < 0) tmod += 2*M_PI;

    qreal indF = ((qreal)bankSize) * tmod / (2*(qreal)M_PI);

    if (indF == (qreal)bankSize) indF = 0;

    if (indF< 0) qDebug() << t << tmod << indF;
    if (indF >= (qreal)bankSize) qDebug() << indF << bankSize;

    Q_ASSERT(indF >= 0);
    Q_ASSERT(indF < (qreal)bankSize);

    unsigned int ind_min = (unsigned int) qFloor(indF);
    unsigned int ind_max = (unsigned int) qCeil(indF);

    Q_ASSERT(ind_min <= ind_max);
    Q_ASSERT(ind_max <= bankSize);
    Q_ASSERT(ind_min < bankSize);

    qreal indmod = indF - (qreal)ind_min;
    Q_ASSERT(indmod < 1 && indmod >= 0);

    qreal value_next, value_prev;

    if (ind_min == ind_max) {
        return waveBank[ind_min];

    } else if (ind_max == bankSize) {
        value_prev = waveBank[ind_min];
        value_next = waveBank[0];
    } else if (ind_min < ind_max) {
        Q_ASSERT(ind_max < bankSize);
        value_prev = waveBank[ind_min];
        value_next = waveBank[ind_max];
    } else {
        // This shouldn't be reached;
        qCritical("Wave Bank Interpolation Failed");
        QCoreApplication::exit(-1);
    }
    return indmod * value_next + (1-indmod) * value_prev;
}
