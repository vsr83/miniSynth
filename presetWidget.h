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

#ifndef PRESETWIDGET_H
#define PRESETWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QVector>
#include "preset.h"

class PresetWidget : public QWidget {
    Q_OBJECT
public:
    PresetWidget(QWidget *parent = 0);
    ~PresetWidget();
private slots:
    void presetClicked(int button);
signals:
    void setPreset(Preset &preset);
private:
    QGridLayout *gridLayout;
    Preset PresetSinwave, PresetElOrgan,  PresetChOrgan,
           PresetBrass,   PresetSynthPad, PresetSynthLead;
    QPushButton *buttonSinwave, *buttonElOrgan, *buttonChOrgan,
                *buttonBrass, *buttonSynthPad, *buttonSynthLead;
    QButtonGroup *buttonGroup;

    QVector<Preset> presets;
};

#endif // PRESETWIDGET_H
