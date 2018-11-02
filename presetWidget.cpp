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

#include "presetWidget.h"
#include <QDebug>

PresetWidget::PresetWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout(this);

    buttonSinwave   = new QPushButton(tr("Sin Wave"));
    buttonElOrgan   = new QPushButton(tr("Electric Organ"));
    buttonChOrgan   = new QPushButton(tr("Church Organ"));
    buttonBrass     = new QPushButton(tr("Synth Brass"));
    buttonSynthPad  = new QPushButton(tr("Synth Pad"));
    buttonSynthLead = new QPushButton(tr("Horrible"));

    gridLayout->addWidget(buttonSinwave,   0, 0, 1, 1);
    gridLayout->addWidget(buttonElOrgan,   1, 0, 1, 1);
    gridLayout->addWidget(buttonChOrgan,   2, 0, 1, 1);
    gridLayout->addWidget(buttonBrass,     0, 1, 1, 1);
    gridLayout->addWidget(buttonSynthPad,  1, 1, 1, 1);
    gridLayout->addWidget(buttonSynthLead, 2, 1, 1, 1);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(buttonSinwave);
    buttonGroup->addButton(buttonElOrgan);
    buttonGroup->addButton(buttonChOrgan);
    buttonGroup->addButton(buttonBrass);
    buttonGroup->addButton(buttonSynthPad);
    buttonGroup->addButton(buttonSynthLead);
    buttonGroup->setId(buttonSinwave,   0);
    buttonGroup->setId(buttonElOrgan,   1);
    buttonGroup->setId(buttonChOrgan,   2);
    buttonGroup->setId(buttonBrass,     3);
    buttonGroup->setId(buttonSynthPad,  4);
    buttonGroup->setId(buttonSynthLead, 5);

    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(presetClicked(int)));

    setLayout(gridLayout);

    PresetElOrgan.timbreAmplitudes[0] = 80;
    PresetElOrgan.timbreAmplitudes[1] = 40;
    PresetElOrgan.timbreAmplitudes[2] = 25;
    PresetElOrgan.timbreAmplitudes[3] = 20;
    PresetElOrgan.timbreAmplitudes[4] = 20;
    PresetElOrgan.timbreAmplitudes[5] = 10;
    PresetElOrgan.timbreAmplitudes[7] = 80;
    PresetElOrgan.env.attackTime = 50;
    PresetElOrgan.env.sustainAmpl = 0.6;
    PresetElOrgan.rev.active = true;
    PresetElOrgan.rev.attenuation = 2;
    PresetElOrgan.mod.AM_ampl = 0.1;
    PresetElOrgan.mod.AM_freq = 10;

    PresetChOrgan.timbreAmplitudes[0] = 50;
    PresetChOrgan.timbreAmplitudes[1] = 15;
    PresetChOrgan.timbreAmplitudes[3] = 15;
    PresetChOrgan.timbreAmplitudes[7] = 50;
    PresetChOrgan.rev.active = true;
    PresetChOrgan.rev.attenuation = 1.2;
    PresetChOrgan.env.attackTime = 50;

    PresetBrass.timbreAmplitudes[0] = 85;
    PresetBrass.timbreAmplitudes[1] = 40;
    PresetBrass.timbreAmplitudes[2] = 15;
    PresetBrass.mod.FM_ampl = 0.006;
    PresetBrass.mod.FM_freq = 0.5;
    PresetBrass.mod.propFreq = true;
    PresetBrass.mod.useEnvelope = true;
    PresetBrass.env.sustainAmpl = 0.4;
    PresetBrass.env.decayTime = 400;
    PresetBrass.env.attackTime = 100;
    PresetBrass.rev.active = true;
    PresetBrass.rev.attenuation = 1.8;

    PresetSynthPad.timbreAmplitudes[0] = 70;
    PresetSynthPad.timbreAmplitudes[2] = 20;
    PresetSynthPad.timbreAmplitudes[4] = 15;
    PresetSynthPad.env.attackTime = 200;
    PresetSynthPad.env.decayTime = 400;
    PresetSynthPad.rev.active = true;
    PresetSynthPad.rev.attenuation = 0.9;

    PresetSynthLead.timbreAmplitudes[0] = 85;
    PresetSynthLead.waveformMode = Waveform::MODE_SAW2;
    PresetSynthLead.mod.FM_ampl = 1;
    PresetSynthLead.mod.FM_freq = 2.05;
    PresetSynthLead.mod.propFreq = true;
    PresetSynthLead.mod.useEnvelope = false;
    PresetSynthLead.env.sustainAmpl = 0.9;
    PresetSynthLead.rev.active = true;
    PresetSynthLead.rev.attenuation = 0.3;
    PresetSynthLead.mod.mode = Waveform::MODE_SAW2;
    PresetSynthLead.filt.size = 10;
    PresetSynthLead.filt.type = Filter::FILTER_LOWPASS;

    presets.push_back(PresetSinwave);
    presets.push_back(PresetElOrgan);
    presets.push_back(PresetChOrgan);
    presets.push_back(PresetBrass);
    presets.push_back(PresetSynthPad);
    presets.push_back(PresetSynthLead);
}

void
PresetWidget::presetClicked(int button) {
    qDebug() << button;
    emit setPreset(presets[button]);
}

PresetWidget::~PresetWidget() {
}
