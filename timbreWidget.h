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

#ifndef TIMBREWIDGET_H
#define TIMBREWIDGET_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QSlider>
#include <QDial>
#include <QLabel>
#include <QSignalMapper>

class TimbreWidget : public QWidget {
    Q_OBJECT

public:
    TimbreWidget(int _numHarmonics, QWidget *parent = 0);
    ~TimbreWidget();

    void getValues(QVector<int> &amplitudesOut, QVector<int> &phasesOut);
    void setValues(QVector<int> &amplitudesIn, QVector<int> &phasesIn);
public slots:
    void reset();
    void updateValues();
private slots:
    void valueChanged(int tmp);
signals:
    void settingsChanged(QVector<int> &amplitudes, QVector<int> &phases);
private:
    int numHarmonics;
    QHBoxLayout *hbox;

    QVector<QSlider *> sliders;
    QVector<QDial *> dials;
    QVector<int> amplitudes;
    QVector<int> phases;

    QSignalMapper *signalMapper;
};

#endif // TIMBREWIDGET_H
