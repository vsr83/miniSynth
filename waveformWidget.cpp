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

#include "waveformWidget.h"
#include <QDebug>

WaveformWidget::WaveformWidget(QWidget *parent) : QWidget(parent) {
    vbox = new QVBoxLayout(this);

    sinButton = new QRadioButton(tr("Sinusoidal"));
    squButton = new QRadioButton(tr("Square"));
    sawButton = new QRadioButton(tr("Saw"));

    connect(sinButton, SIGNAL(toggled(bool)), this, SLOT(toggleSin(bool)));
    connect(sawButton, SIGNAL(toggled(bool)), this, SLOT(toggleSaw(bool)));
    connect(squButton, SIGNAL(toggled(bool)), this, SLOT(toggleSqu(bool)));

    setLayout(vbox);
    vbox->addWidget(sinButton);
    vbox->addWidget(squButton);
    vbox->addWidget(sawButton);

    oscMode = Waveform::MODE_SIN;
    sinButton->toggle();
}

int
WaveformWidget::getMode() {
    return oscMode;
}

void
WaveformWidget::toggleSin(bool st) {
    if (st) {
        oscMode = Waveform::MODE_SIN;
        emit modeSelected(oscMode);
    }
}

void
WaveformWidget::toggleSqu(bool st) {
    if (st) {
        oscMode = Waveform::MODE_SQU;
        emit modeSelected(oscMode);
    }
}

void
WaveformWidget::toggleSaw(bool st) {
    if (st) {
        oscMode = Waveform::MODE_SAW;
        emit modeSelected(oscMode);
    }
}

WaveformWidget::~WaveformWidget() {
}
