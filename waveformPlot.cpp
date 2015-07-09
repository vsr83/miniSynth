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

#include "waveformPlot.h"
#include "waveform.h"

WaveformPlot::WaveformPlot(QWidget *parent) : QWidget(parent) {
    waveForm = 0;
    border = 5;
    setMode(Waveform::MODE_SIN);
}

WaveformPlot::~WaveformPlot() {
    delete waveForm;
    waveForm = 0;
}

void
WaveformPlot::setMode(int _mode) {
    mode = _mode;
    delete waveForm;
    waveForm = new LinearSynthesis(mode);

    refreshPixmap();
}

void
WaveformPlot::setTimbre(QVector<int> &amplitudes, QVector<int> &phases) {
    waveForm->setTimbre(amplitudes, phases);
    refreshPixmap();
}

void
WaveformPlot::refreshPixmap() {
    pixmap = QPixmap(size());
    pixmap.fill();

    QPainter painter(&pixmap);

    QPen penWave, penGrid, penBorder;

    penWave.setStyle(Qt::SolidLine);
    penWave.setWidth(2);
    penWave.setColor(QColor(0, 0, 0));

    penGrid.setStyle(Qt::DashLine);
    penGrid.setWidth(1);
    penGrid.setColor(QColor(150, 150, 150));

    penBorder.setStyle(Qt::SolidLine);
    penBorder.setWidth(1);
    penBorder.setColor(QColor(0, 0, 0));


    unsigned int ww, wh;
    ww = width()  - 2*border;
    wh = height() - 2*border;

    painter.setPen(penGrid);
    painter.drawLine(0, border + wh/2, width(), border + wh/2);
    painter.setPen(penBorder);
    painter.drawLine(border, border, width()-border, border);
    painter.drawLine(border, height() - border, width()-border, height() - border);
    painter.drawLine(border, border, border, height() - border);
    painter.drawLine(width() - border, border, width() - border, height() - border);

    painter.setPen(penWave);
    qreal dt = 1 / ((qreal)ww);

    Q_ASSERT(ww > 0);

    qreal *vals = new qreal[ww];

    qreal valmax = 0;
    for (unsigned int x= 0; x < ww; x++) {
        qreal t   = dt*((qreal)x),
              val = waveForm->evalTimbre(2*M_PI * t);

        vals[x] = val;
        if (qAbs(val) > valmax) valmax = qAbs(val);
    }
    for (unsigned int x= 0; x < ww-1; x++) {
        qreal val      = vals[x]/valmax,
              val_next = vals[x+1]/valmax;
        unsigned int y      = (unsigned int)((val + 1) * (qreal)wh/(2)),
                     y_next = (unsigned int)((val_next + 1) * (qreal)wh/(2));

        painter.drawLine(x+border, y+border, x+1+border, y_next+border);
    }
    delete [] vals;
    update();
}

void
WaveformPlot::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    Q_UNUSED(event);
}

void
WaveformPlot::resizeEvent(QResizeEvent *event) {
    refreshPixmap();
    Q_UNUSED(event);
}
