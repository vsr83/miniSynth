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

#ifndef MODULATIONWIDGET_H
#define MODULATIONWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include "waveformWidget.h"
#include "modulation.h"

class ModulationWidget : public QWidget {
    Q_OBJECT
public:
    ModulationWidget(QWidget *parent = 0);
    ~ModulationWidget();
signals:
    void setModulation(Modulation &modulation);
private slots:
    void parametersChanged(int);
private:
    WaveformWidget *waveformWidget;

    QGridLayout *gridLayout;
    QSlider *FMfreqSlider, *FMamplSlider;
    QLabel  *FMfreqLabel, *FMamplLabel, *FMtitle;
    QCheckBox *FMpropfreq, *FMenvelope;

    QSlider *AMfreqSlider, *AMamplSlider, *AMtimeSlider;
    QLabel  *AMfreqLabel, *AMamplLabel, *AMtitle, *AMtimeLabel;
    QCheckBox *AMpropfreq, *AMenvelope;
};

#endif // MODULATIONWIDGET_H
