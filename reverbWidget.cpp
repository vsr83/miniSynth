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


#include "reverbWidget.h"
#include <QDebug>

ReverbWidget::ReverbWidget(unsigned int _samplingRate, QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout;
    setLayout(gridLayout);

    samplingRate = _samplingRate;

/*    delayLabel = new QLabel(tr("Delay :"));
    delaySlider = new QSlider(Qt::Horizontal);
    delaySlider->setRange(0, 1000);
    delaySlider->setValue(0);
    delayValueLabel = new QLabel(tr("0"));

    attnLabel = new QLabel(tr("Attenuation :"));
    attnSlider = new QSlider(Qt::Horizontal);
    attnSlider->setRange(0, 100);
    attnSlider->setValue(0);
    attnValueLabel = new QLabel(tr("0"));
*/
    activeBox = new QCheckBox(tr("Active"));
    activeBox->setChecked(false);
    delayLabel = new QLabel(tr("Delay"));
    attnLabel = new QLabel(tr("Attenuation"));

    gridLayout->addWidget(activeBox,        0, 0, 1, 1);
    delayDial = new QDial;
    attnDial = new QDial;

    delayDial->setMaximumSize(75, 75);
    attnDial->setMaximumSize(75, 75);

    delayDial->setRange(1000, 10000);
    delayDial->setValue(8000);

    attnDial->setRange(0, 500);
    attnDial->setValue(100);

    connect(delayDial, SIGNAL(valueChanged(int)), this, SLOT(updateReverb(int)));
    connect(attnDial, SIGNAL(valueChanged(int)), this, SLOT(updateReverb(int)));
    connect(activeBox, SIGNAL(clicked(bool)), this, SLOT(updateReverb(bool)));

    gridLayout->addWidget(delayDial, 0, 1, 1, 1, Qt::AlignHCenter);
    gridLayout->addWidget(attnDial, 0, 2, 1, 1, Qt::AlignHCenter);
    gridLayout->addWidget(delayLabel, 1, 1, 1, 1, Qt::AlignHCenter);
    gridLayout->addWidget(attnLabel, 1, 2, 1, 1, Qt::AlignHCenter);

    QLabel *emptyLabel = new QLabel(tr(" "));
    gridLayout->addWidget(emptyLabel, 0, 3);
    gridLayout->setColumnStretch(3, 1);
    /*
    gridLayout->addWidget(delayLabel,       0, 1, 1, 1);
    gridLayout->addWidget(delaySlider     , 0, 2, 1, 1);
    gridLayout->addWidget(attnValueLabel,   0, 3, 1, 1);
    gridLayout->addWidget(attnLabel,        1, 1, 1, 1);
    gridLayout->addWidget(attnSlider,       1, 2, 1, 1);
    gridLayout->addWidget(delayValueLabel,  1, 3, 1, 1);

    gridLayout->setColumnMinimumWidth(3, 50);
    */
}

void
ReverbWidget::updateReverb(bool value) {
    updateReverb(10);
}

void
ReverbWidget::importReverb(Reverb &rev) {
    delayDial->setValue(rev.delay);
    attnDial->setValue((int)(rev.attenuation*100));
    activeBox->setChecked(rev.active);
    emit setReverb(rev);
}

void
ReverbWidget::updateReverb(int value) {
    Reverb rev;
    Q_UNUSED(value);

    rev.active = activeBox->checkState() == Qt::Checked;
    rev.attenuation = 0.01 * attnDial->value();
    rev.delay = delayDial->value();
    rev.samplingRate = samplingRate;


    qDebug() << rev.attenuation << rev.delay;
    emit setReverb(rev);
}

ReverbWidget::~ReverbWidget() {
}
