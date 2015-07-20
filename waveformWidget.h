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

#include <QWidget>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "waveform.h"

#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

class WaveformWidget : public QWidget {
    Q_OBJECT
public:
    WaveformWidget(QWidget *parent = 0);
    ~WaveformWidget();

    void setMode(int mode);
    int  getMode();
public slots:
private slots:
    void toggleSin (bool st);
    void toggleSqu (bool st);
    void toggleSaw (bool st);
    void toggleSaw2(bool st);
signals:
    void modeSelected(int mode);
private:
    int oscMode;

    QVBoxLayout *vbox;
    QHBoxLayout *hbox;
    QRadioButton *sinButton, *squButton, *sawButton, *saw2Button;
};

#endif // WAVEFORMWIDGET_H
