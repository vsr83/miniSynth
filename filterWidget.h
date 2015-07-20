#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H
#include <QWidget>
#include <QGridLayout>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
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

#include <QButtonGroup>
#include <QLabel>

#include "filter.h"

class FilterWidget : public QWidget {
    Q_OBJECT
public:
    FilterWidget(QWidget *parent = 0);
    ~FilterWidget();
    void importFilter(FilterParameters &param);
signals:
    void parametersChanged(FilterParameters &param);
private slots:
    void windowSelect(int button);
    void filterSelect(int button);
    void setFreq1    (int freq);
    void setFreq2    (int freq);
    void setSize     (int size);
    void setFFTTimer (int len);
private:
    void updateParameters();
    QGridLayout *gridLayout;
    QLabel *typeLabel, *windowLabel, *freqLabel;

    QLabel  *freqLabel1,  *freqLabel2, *paramLabel, *paramLabel2, *sizeLabel;
    QLabel *cutoffLabel1, *cutoffLabel2, *fftTimeLabel;
    QSlider *freqSlider1, *freqSlider2;

    QSpinBox     *sizeSpinBox, *fftTimeSpinBox;
    QButtonGroup *filterGroup, *windowGroup;
    QRadioButton *LPButton, *HPButton, *BPButton, *BSButton, *offButton;
    QRadioButton *RectButton, *HanningButton, *HammingButton, *BlackmanButton, *KaiserButton;

    FilterParameters filterParameters;
};

#endif // FILTERWIDGET_H
