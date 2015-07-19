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

#ifndef REVERBWIDGET_H
#define REVERBWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QDial>

#include "reverb.h"

class ReverbWidget : public QWidget {
    Q_OBJECT
public:
    ReverbWidget(unsigned int _samplingRate, QWidget *parent = 0);
    ~ReverbWidget();

    QSlider *delaySlider, *attnSlider;
    QLabel  *delayLabel, *attnLabel;
    QLabel  *delayValueLabel, *attnValueLabel;

    QDial *delayDial, *attnDial;

    QCheckBox *activeBox;
    QGridLayout *gridLayout;
private slots:
    void updateReverb(int value);
    void updateReverb(bool value);
signals:
    void setReverb(Reverb &rev);
private:
    qreal samplingRate;
};

#endif // REVERBWIDGET_H
