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

#include "filterWidget.h"
#include <QDebug>
#include <qmath.h>

FilterWidget::FilterWidget(QWidget *parent) : QWidget(parent) {
    filterParameters.samplingRate= 44100;
    filterParameters.freq1       = 0;
    filterParameters.freq2       = 4000;
    filterParameters.size        = 100;
    filterParameters.type        = Filter::FILTER_OFF;
    filterParameters.window_type = Filter::WINDOW_RECT;

    filterGroup = new QButtonGroup;
    offButton = new QRadioButton(tr("Through"));
    LPButton  = new QRadioButton(tr("Low-pass"));
    HPButton  = new QRadioButton(tr("High-pass"));
    BPButton  = new QRadioButton(tr("Band-pass"));
    BSButton  = new QRadioButton(tr("Band-stop"));
    filterGroup->addButton(offButton);
    filterGroup->addButton(LPButton);
    filterGroup->addButton(HPButton);
    filterGroup->addButton(BPButton);
    filterGroup->addButton(BSButton);
    filterGroup->setId(offButton, 0);
    filterGroup->setId(LPButton,  1);
    filterGroup->setId(HPButton,  2);
    filterGroup->setId(BPButton,  3);
    filterGroup->setId(BSButton,  4);

    offButton->setChecked(true);

    windowGroup = new QButtonGroup;
    RectButton     = new QRadioButton(tr("Rectangular"));
    HanningButton  = new QRadioButton(tr("Hanning"));
    HammingButton  = new QRadioButton(tr("Hamming"));
    BlackmanButton = new QRadioButton(tr("Blackman"));
    KaiserButton   = new QRadioButton(tr("Kaiser"));
    windowGroup->addButton(RectButton);
    windowGroup->addButton(HanningButton);
    windowGroup->addButton(HammingButton);
    windowGroup->addButton(BlackmanButton);
    windowGroup->addButton(KaiserButton);
    windowGroup->setId(RectButton,     0);
    windowGroup->setId(HanningButton,  1);
    windowGroup->setId(HammingButton,  2);
    windowGroup->setId(BlackmanButton, 3);
    windowGroup->setId(KaiserButton,   4);
    KaiserButton->setEnabled(false);
    RectButton->setChecked(true);

    connect(filterGroup, SIGNAL(buttonReleased(int)), this, SLOT(filterSelect(int)));
    connect(windowGroup, SIGNAL(buttonReleased(int)), this, SLOT(windowSelect(int)));

    typeLabel   = new QLabel(tr("Type"));
    windowLabel = new QLabel(tr("Window"));
    paramLabel  = new QLabel(tr("FILTER "));
    paramLabel2  = new QLabel(tr(" "));

    freqLabel1 = new QLabel(tr("Cutoff 1 :"));
    freqLabel2 = new QLabel(tr("Cutoff 2 :"));
    freqSlider1 = new QSlider(Qt::Horizontal);
    freqSlider2 = new QSlider(Qt::Horizontal);
    cutoffLabel1 = new QLabel("");
    cutoffLabel2 = new QLabel("");
    freqSlider1->setRange(1, 127);
    freqSlider2->setRange(1, 127);
    freqSlider1->setValue(33);
    freqSlider2->setValue(120);

    connect(freqSlider1, SIGNAL(valueChanged(int)), this, SLOT(setFreq1(int)));
    connect(freqSlider2, SIGNAL(valueChanged(int)), this, SLOT(setFreq2(int)));

    sizeSpinBox = new QSpinBox;
    sizeSpinBox->setRange(10, 1000);
    sizeSpinBox->setValue(filterParameters.size);
    sizeLabel = new QLabel(tr("Filter Size :"));

    connect(sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setSize(int)));

    gridLayout = new QGridLayout();
    gridLayout->addWidget(offButton, 1, 0, 1, 1);
    gridLayout->addWidget(LPButton, 2, 0, 1, 1);
    gridLayout->addWidget(HPButton, 3, 0, 1, 1);
    gridLayout->addWidget(BPButton, 4, 0, 1, 1);
    gridLayout->addWidget(BSButton, 5, 0, 1, 1);
    gridLayout->addWidget(RectButton,     1, 2, 1, 1);
    gridLayout->addWidget(HanningButton,  2, 2, 1, 1);
    gridLayout->addWidget(HammingButton,  3, 2, 1, 1);
    gridLayout->addWidget(BlackmanButton, 4, 2, 1, 1);
    gridLayout->addWidget(KaiserButton,   5, 2, 1, 1);

    gridLayout->setRowMinimumHeight(6, 70);
    gridLayout->addWidget(freqLabel1,  2, 4, 1, 1);
    gridLayout->addWidget(freqLabel2,  3, 4, 1, 1);
    gridLayout->addWidget(freqSlider1, 2, 5, 1, 1);
    gridLayout->addWidget(freqSlider2, 3, 5, 1, 1);
    gridLayout->addWidget(cutoffLabel1, 2, 6, 1, 1);
    gridLayout->addWidget(cutoffLabel2, 3, 6, 1, 1);
    gridLayout->addWidget(sizeLabel,   4, 4, 1, 1);
    gridLayout->addWidget(sizeSpinBox, 4, 5, 1, 1);
    gridLayout->setColumnMinimumWidth(6, 80);

    gridLayout->addWidget(typeLabel,   0, 0, 1, 1);
    gridLayout->addWidget(windowLabel, 0, 2, 1, 1);
    gridLayout->addWidget(paramLabel, 0, 4, 1, 2);
    gridLayout->addWidget(paramLabel2, 1, 4, 1, 2);
    gridLayout->setColumnMinimumWidth(3, 20);
    gridLayout->setColumnMinimumWidth(1, 10);
    setLayout(gridLayout);

    updateParameters();
}

void
FilterWidget::setFreq1(int note) {
    filterParameters.freq1 = 8.175 * 0.5 * qPow(2, ((qreal)note)/12);
    emit parametersChanged(filterParameters);
    updateParameters();
    if (filterParameters.freq2 < filterParameters.freq1) {
        freqSlider2->setValue(note);
    }
}

void
FilterWidget::setFreq2(int note) {
    filterParameters.freq2 = 8.175 * 0.5 * qPow(2, ((qreal)note)/12);
    emit parametersChanged(filterParameters);
    updateParameters();
    if (filterParameters.freq2 < filterParameters.freq1) {
        freqSlider1->setValue(note);
    }
}

void
FilterWidget::setSize(int size) {
    filterParameters.size = (unsigned int) size;
    emit parametersChanged(filterParameters);
    updateParameters();
}

void
FilterWidget::filterSelect(int button) {
    qDebug() << button;
    switch (button) {
    case 0:
        filterParameters.type = Filter::FILTER_OFF;
        break;
    case 1:
        filterParameters.type = Filter::FILTER_LOWPASS;
        break;
    case 2:
        filterParameters.type = Filter::FILTER_HIGHPASS;
        break;
    case 3:
        filterParameters.type = Filter::FILTER_BANDPASS;
        break;
    case 4:
        filterParameters.type = Filter::FILTER_BANDSTOP;
        break;
    }
    emit parametersChanged(filterParameters);
    updateParameters();
}

void
FilterWidget::windowSelect(int button) {
    qDebug() << button;
    switch (button) {
    case 0:
        filterParameters.window_type = Filter::WINDOW_RECT;
        break;
    case 1:
        filterParameters.window_type = Filter::WINDOW_HANNING;
        break;
    case 2:
        filterParameters.window_type = Filter::WINDOW_HAMMING;
        break;
    case 3:
        filterParameters.window_type = Filter::WINDOW_BLACKMAN;
        break;
    case 4:
        filterParameters.window_type = 5;
        break;
    }
    emit parametersChanged(filterParameters);
    updateParameters();
}

FilterWidget::~FilterWidget() {
}

void
FilterWidget::updateParameters() {
    qreal tw = 0;
    QString filterName, windowName, freqLabel;

    switch (filterParameters.type) {
    case Filter::FILTER_OFF:
        filterName = "OFF";
        break;
    case Filter::FILTER_LOWPASS:
        filterName = "LOW-PASS";
        freqLabel = QString("f_c %1 Hz").arg(filterParameters.freq1);
        break;
    case Filter::FILTER_HIGHPASS:
        filterName = "HIGH-PASS";
        freqLabel = QString("f_c %1 Hz").arg(filterParameters.freq1);
        break;
    case Filter::FILTER_BANDPASS:
        filterName = "BAND-PASS";
        freqLabel = QString("f_min %1 Hz, f_max %2 Hz")
                    .arg(filterParameters.freq1).arg(filterParameters.freq2);
        break;
    case Filter::FILTER_BANDSTOP:
        filterName = "BAND-STOP";
        freqLabel = QString("f_c %1").arg(filterParameters.freq1);
        freqLabel = QString("[f_min, f_max] %1, %2 Hz")
                    .arg(filterParameters.freq1).arg(filterParameters.freq2);
        break;
    }

    switch (filterParameters.window_type) {
    case Filter::WINDOW_RECT:
        tw = filterParameters.samplingRate*(0.9/filterParameters.size);
        windowName = "RECT";
        break;
    case Filter::WINDOW_HANNING:
        tw = filterParameters.samplingRate*(3.1/filterParameters.size);
        windowName = "HANNING";
        break;
    case Filter::WINDOW_HAMMING:
        tw = filterParameters.samplingRate*(3.3/filterParameters.size);
        windowName = "HAMMING";
        break;
    case Filter::WINDOW_BLACKMAN:
        tw = filterParameters.samplingRate*(5.5/filterParameters.size);
        windowName = "BLACKMAN";
        break;
    }

    QString str2 = QString("Transition Width %1 Hz")
                   .arg(QString::number(tw, 'f', 1));
    paramLabel2->setText(str2);

    QString coffstr1 = QString(" %1 Hz")
                       .arg(QString::number(filterParameters.freq1, 'f', 1));
    QString coffstr2 = QString(" %1 Hz")
                       .arg(QString::number(filterParameters.freq2, 'f', 1));
    cutoffLabel1->setText(coffstr1);
    cutoffLabel2->setText(coffstr2);
}
