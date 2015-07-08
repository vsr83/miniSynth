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

#ifndef ADSRWIDGET_H
#define ADSRWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>

#include "ADSRplot.h"
#include "ADSRenvelope.h"

class ADSRWidget : public QWidget {
    Q_OBJECT
public:
    ADSRWidget(QWidget *parent = 0);
    ~ADSRWidget();
signals:
    void envelopeChanged(ADSREnvelope &_envelope);
private slots:
    void valueChangedInt(int val);
    void valueChangedDouble(double val);
private:
    QSpinBox       *attackSB, *decaySB, *releaseSB;
    QDoubleSpinBox *initialSB, *peakSB, *sustainSB;

    QGridLayout *gridLayout;
    ADSRPlot *plot;

    ADSREnvelope envelope;
};

#endif // ADSRWIDGET_H
