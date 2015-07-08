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

#include "ADSRwidget.h"
#include <QLabel>

ADSRWidget::ADSRWidget(QWidget *parent) : QWidget(parent) {
    envelope.attackTime = 100;
    envelope.decayTime = 400;
    envelope.releaseTime = 100;

    envelope.initialAmpl = 0;
    envelope.peakAmpl = 1;
    envelope.sustainAmpl = 0.8;

    QLabel *attackLabel = new QLabel(tr("Attack:"));
    attackSB = new QSpinBox();
    attackSB->setRange(0, 1000);
    attackSB->setSuffix(tr(" ms"));
    attackSB->setValue(envelope.attackTime);

    QLabel *decayLabel = new QLabel(tr("Decay: "));
    decaySB = new QSpinBox();
    decaySB->setRange(0, 1000);
    decaySB->setSuffix(tr(" ms"));
    decaySB->setValue(envelope.decayTime);

    QLabel *releaseLabel = new QLabel(tr("Release: "));
    releaseSB = new QSpinBox();
    releaseSB->setRange(0, 1000);
    releaseSB->setSuffix(tr(" ms"));
    releaseSB->setValue(envelope.releaseTime);

    QLabel *initialLabel = new QLabel(tr("  Initial: "));
    initialSB = new QDoubleSpinBox();
    initialSB->setValue(envelope.initialAmpl);
    initialSB->setRange(0, 1);
    initialSB->setSingleStep(0.01);

    QLabel *peakLabel = new QLabel(tr("  Peak: "));
    peakSB = new QDoubleSpinBox();
    peakSB->setValue(envelope.peakAmpl);
    peakSB->setRange(0, 1);
    peakSB->setSingleStep(0.01);

    QLabel *sustainLabel = new QLabel(tr("  Sustain: "));
    sustainSB = new QDoubleSpinBox();
    sustainSB->setValue(envelope.sustainAmpl);
    sustainSB->setRange(0, 1);
    sustainSB->setSingleStep(0.01);

    gridLayout = new QGridLayout;
    setLayout(gridLayout);

    plot = new ADSRPlot;

    gridLayout->addWidget(attackLabel,  0, 0);
    gridLayout->addWidget(attackSB,     0, 1);
    gridLayout->addWidget(decayLabel,   1, 0);
    gridLayout->addWidget(decaySB,      1, 1);
    gridLayout->addWidget(releaseLabel, 2, 0);
    gridLayout->addWidget(releaseSB,    2, 1);

    gridLayout->addWidget(initialLabel,  0, 2);
    gridLayout->addWidget(initialSB,     0, 3);
    gridLayout->addWidget(peakLabel,     1, 2);
    gridLayout->addWidget(peakSB,        1, 3);
    gridLayout->addWidget(sustainLabel,  2, 2);
    gridLayout->addWidget(sustainSB,     2, 3);

    gridLayout->addWidget(plot, 0, 4, 4, 1);
    gridLayout->setColumnMinimumWidth(4, 150);

    connect(attackSB,  SIGNAL(valueChanged(int)), this, SLOT(valueChangedInt(int)));
    connect(decaySB,   SIGNAL(valueChanged(int)), this, SLOT(valueChangedInt(int)));
    connect(releaseSB, SIGNAL(valueChanged(int)), this, SLOT(valueChangedInt(int)));
    connect(initialSB, SIGNAL(valueChanged(double)),
            this, SLOT(valueChangedDouble(double)));
    connect(peakSB, SIGNAL(valueChanged(double)),
            this, SLOT(valueChangedDouble(double)));
    connect(sustainSB, SIGNAL(valueChanged(double)),
            this, SLOT(valueChangedDouble(double)));

    plot->setValues(envelope);
}

ADSRWidget::~ADSRWidget() {
}

void
ADSRWidget::valueChangedInt(int val) {
    envelope.attackTime  = attackSB->value();
    envelope.decayTime   = decaySB->value();
    envelope.releaseTime = releaseSB->value();

    envelope.initialAmpl = (qreal)initialSB->value();
    envelope.peakAmpl    = (qreal)peakSB->value();
    envelope.sustainAmpl = (qreal)sustainSB->value();

    plot->setValues(envelope);
    emit envelopeChanged(envelope);
    Q_UNUSED(val);
}

void
ADSRWidget::valueChangedDouble(double val) {
    valueChangedInt(0);
    Q_UNUSED(val);
}
