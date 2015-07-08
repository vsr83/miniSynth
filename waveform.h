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

#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <qmath.h>
#include <math.h>
#include <QVector>
#include <QObject>

class Waveform  {
public:
    Waveform(unsigned int mode, unsigned int size=4096);
    ~Waveform();

    qreal waveSin(qreal t);
    qreal waveSaw(qreal t);
    qreal waveSqu(qreal t);

    qreal eval(qreal t);

    enum {MODE_SIN, MODE_SAW, MODE_SQU};
private:
    qreal *waveBank;
    unsigned int bankSize;
    int mode;
};

#endif // WAVEFORM_H
