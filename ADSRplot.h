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

#ifndef ADSRPLOT_H
#define ADSRPLOT_H

#include <QWidget>
#include <QPixmap>
#include "ADSRenvelope.h"

class ADSRPlot : public QWidget {
    Q_OBJECT
public:
    ADSRPlot(QWidget *parent = 0);
    ~ADSRPlot();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
public slots:
    void setValues(ADSREnvelope &_envelope);
private:
    void refreshPixmap();
    ADSREnvelope envelope;

    QColor bgColor;
    QPixmap pixmap;
    int border;
};

#endif // ADSRPLOT_H
