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

#include "timbreWidget.h"
#include <QDebug>
#include <math.h>

TimbreWidget::TimbreWidget(int _numHarmonics, QWidget *parent)
    : QWidget(parent), numHarmonics(_numHarmonics) {
    reset();

    hbox = new QHBoxLayout;
    setLayout(hbox);

    for (int harmonic = 0; harmonic < _numHarmonics; harmonic++) {
        QVBoxLayout *vbox = new QVBoxLayout;
        hbox->addLayout(vbox);

        QLabel *label = new QLabel(QString::number(harmonic+1));
        vbox->addWidget(label);
        vbox->setAlignment(label, Qt::AlignHCenter);

        QSlider *slider = new QSlider(Qt::Vertical);
        vbox->addWidget(slider);
        slider->setMinimum(0);
        slider->setMaximum(100);
        slider->setMinimumHeight(100);
        slider->setTickPosition(QSlider::TicksBothSides);
        vbox->setAlignment(slider, Qt::AlignHCenter);

        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

        QDial *dial = new QDial;
        vbox->addWidget(dial);
        dial->setMinimum(-180);
        dial->setMaximum(180);
        dial->setMaximumSize(30, 30);
        vbox->setAlignment(dial, Qt::AlignHCenter);

        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

        sliders.push_back(slider);
        dials.push_back(dial);
    }
    updateValues();
}

void
TimbreWidget::reset() {
    amplitudes = QVector<int>(numHarmonics, 0);
    phases = QVector<int>(numHarmonics, 0);
    amplitudes[0] = 100;
}

void
TimbreWidget::updateValues() {
    for (int harm = 0; harm < numHarmonics; harm++) {
         sliders[harm]->setValue(amplitudes[harm]);
         dials[harm]->setValue(phases[harm]);
    }
}

void
TimbreWidget::valueChanged(int tmp) {
    amplitudes.clear();
    phases.clear();

    for (int harm = 0; harm < numHarmonics; harm++) {
        amplitudes.push_back(sliders[harm]->value());
        phases.push_back(dials[harm]->value());
    }
    emit settingsChanged(amplitudes, phases);
    Q_UNUSED(tmp);
}

void
TimbreWidget::setValues(QVector<int> &amplitudesIn, QVector<int> &phasesIn) {
    amplitudes = amplitudesIn;
    phases = phasesIn;
    updateValues();
}

void
TimbreWidget::getValues(QVector<int> &amplitudesOut, QVector<int> &phasesOut) {
    amplitudesOut = amplitudes;
    phasesOut = phases;
}

TimbreWidget::~TimbreWidget() {
}
