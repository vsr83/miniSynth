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

#include "modulationWidget.h"
#include <QDebug>

ModulationWidget::ModulationWidget(QWidget *parent) : QWidget(parent) {
    gridLayout = new QGridLayout;

    FMtitle     = new QLabel(tr("FM -"));
    FMfreqLabel = new QLabel(tr("Frequency :"));
    FMamplLabel = new QLabel(tr("Amplitude :"));
    FMfreqSlider= new QSlider(Qt::Horizontal);
    FMamplSlider= new QSlider(Qt::Horizontal);
    FMpropfreq  = new QCheckBox(tr("Proportional to Frequency"));

    AMtitle     = new QLabel(tr("AM -"));
    AMfreqLabel = new QLabel(tr("Frequency :"));
    AMamplLabel = new QLabel(tr("Amplitude :"));
    AMfreqSlider= new QSlider(Qt::Horizontal);
    AMamplSlider= new QSlider(Qt::Horizontal);

    waveformWidget = new WaveformWidget;

    gridLayout->addWidget(waveformWidget, 0, 0, 4, 1);
    gridLayout->addWidget(FMtitle, 0,1,1,2);
    gridLayout->addWidget(AMtitle, 0,3,1,2);
    gridLayout->addWidget(FMfreqLabel,  1, 1, 1, 1);
    gridLayout->addWidget(FMamplLabel,  2, 1, 1, 1);
    gridLayout->addWidget(FMfreqSlider, 1, 2, 1, 1);
    gridLayout->addWidget(FMamplSlider, 2, 2, 1, 1);
    gridLayout->addWidget(FMpropfreq,   3, 1, 1, 2);
    FMfreqSlider->setRange(0, 1000);
    FMamplSlider->setRange(-100, 0);
    FMamplSlider->setValue(-100);
    FMamplSlider->setTickInterval(20);
    FMamplSlider->setTickPosition(QSlider::TicksBothSides);

    gridLayout->addWidget(AMfreqLabel,  1, 3, 1, 1);
    gridLayout->addWidget(AMamplLabel,  2, 3, 1, 1);
    gridLayout->addWidget(AMfreqSlider, 1, 4, 1, 1);
    gridLayout->addWidget(AMamplSlider, 2, 4, 1, 1);
    AMfreqSlider->setRange(0, 1000);
    AMamplSlider->setRange(-100, 0);
    AMamplSlider->setValue(-100);
    AMamplSlider->setTickInterval(20);
    AMamplSlider->setTickPosition(QSlider::TicksBothSides);

    connect(FMpropfreq, SIGNAL(stateChanged(int)), this,
            SLOT(parametersChanged(int)));
    connect(FMfreqSlider, SIGNAL(valueChanged(int)), this,
            SLOT(parametersChanged(int)));
    connect(AMfreqSlider, SIGNAL(valueChanged(int)), this,
            SLOT(parametersChanged(int)));
    connect(FMamplSlider, SIGNAL(valueChanged(int)), this,
            SLOT(parametersChanged(int)));
    connect(AMamplSlider, SIGNAL(valueChanged(int)), this,
            SLOT(parametersChanged(int)));
    connect(waveformWidget, SIGNAL(modeSelected(int)), this,
            SLOT(parametersChanged(int)));

    setLayout(gridLayout);
}

ModulationWidget::~ModulationWidget() {
}

void
ModulationWidget::parametersChanged(int) {
    Modulation mod;
    mod.mode = waveformWidget->getMode();

    qreal AM_ampl_dB = (qreal)AMamplSlider->value();
    qreal FM_ampl_dB = (qreal)FMamplSlider->value();

    mod.AM_freq = 0.01 * AMfreqSlider->value();
    mod.AM_ampl =  qPow(10, AM_ampl_dB/20);

    mod.FM_freq = 0.01 * FMfreqSlider->value();
    mod.propFreq = FMpropfreq->checkState() == Qt::Checked;
    mod.FM_ampl = qPow(10, FM_ampl_dB/20);

    QString strFM;
    QTextStream(&strFM) << "FM - Freq : " << mod.FM_freq
                        << " Ampl : " << FM_ampl_dB << " dB";
    FMtitle->setText(strFM);

    QString strAM;
    QTextStream(&strAM) << "AM - Freq : " << mod.AM_freq
                        << " Ampl : " << AM_ampl_dB << " dB";
    AMtitle->setText(strAM);

    emit setModulation(mod);
}
