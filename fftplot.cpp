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

#include "fftplot.h"

#ifdef USE_FFTW

#include <QStylePainter>
#include <QDebug>
#include <QPen>
#include <qmath.h>
#include <math.h>

FFTPlot::FFTPlot(QWidget *parent) : QWidget(parent) {
    fftsize = 0;
    fftampl = 0;
    minfreq = 42;
    maxfreq = 10000;

    minampl = 1e-4;
    maxampl = 0.5;
}

FFTPlot::~FFTPlot() {
    delete [] fftampl;
}

void
FFTPlot::refreshPixmap() {
    pixmap = QPixmap(size());
    pixmap.fill();

    QPainter painter(&pixmap);
    if (fftsize != 0) {
        QPen penWave, penGrid, penBorder;

        penWave.setStyle(Qt::SolidLine);
        penWave.setWidth(2);
        penWave.setColor(QColor(0, 0, 0));

        penGrid.setStyle(Qt::DashLine);
        penGrid.setWidth(1);
        penGrid.setColor(QColor(150, 150, 150));

        penBorder.setStyle(Qt::SolidLine);
        penBorder.setWidth(1);
        penBorder.setColor(QColor(150, 150, 150));

        int ww = width(), wh = height();

        qreal minfl = (qreal) log10((double) minfreq),
              maxfl = (qreal) log10((double) maxfreq);
        qreal minampll = (qreal) log10((double)minampl),
              maxampll = (qreal) log10((double)maxampl);
        qreal minampldB = minampll * 20,
              maxampldB = maxampll * 20;

        qreal sx = (qreal)ww / (maxfl - minfl),
              sy = (qreal)wh / (maxampldB - minampldB);

       // qreal dx = ((qreal)width())/(qreal)fftsize*4;
       // painter.drawLine(0, 0, width(), height());

        painter.setPen(penGrid);
        for (int fl = qFloor(minfl); fl < qCeil(maxfl); fl++) {
            for (int df = 1; df < 10; df++) {
                qreal x = sx*(qreal)(log10((double)qPow(10, fl)*df) - minfl);
                painter.drawLine((int)x, 0, (int)x, wh);
            }
        }
        for (int al = qFloor(minampll); al < qCeil(maxampll); al++) {
            for (int da = 1; da < 10; da++) {
                qreal y = wh-(sy*20*(qreal)(log10(qPow(10, al)*da) - minampll));
                painter.drawLine(0, y, ww, y);
            }
        }

        painter.setPen(penBorder);
        for (int fl = qFloor(minfl); fl < qCeil(maxfl); fl++) {
            qreal x = sx*(qreal)(log10((double)qPow(10, fl)) - minfl);
            painter.drawLine((int)x, 0, (int)x, wh);
        }
        for (int fl = qFloor(minfl); fl < qCeil(maxfl); fl++) {
            qreal x = sx*(qreal)(log10((double)qPow(10, fl)) - minfl);
            painter.drawLine((int)x, 0, (int)x, wh);
        }

        painter.setPen(penWave);

        for (int ind = 1; ind < fftsize/2; ind++) {
            qreal prevampl = wh-(sy*20*(qreal)(log10((double)fftampl[ind-1]) - minampll)),
                  ampl =     wh-(sy*20*(qreal)(log10((double)fftampl[ind]) - minampll));
            qreal x     = sx*(qreal)(log10((double)ind) - minfl),
                  prevx = sx*(qreal)(log10((double)(ind+1)) - minfl);
            QPointF pstart(prevx, prevampl),
                    pend(x, ampl);
            painter.drawLine(pstart, pend);
//            qDebug() << dx*(qreal)ind << 100*ampl;
//            qDebug() << (int)(100*prevampl);
        }
        update();
    }
}

void
FFTPlot::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    Q_UNUSED(event);
}

void
FFTPlot::resizeEvent(QResizeEvent *event) {
    refreshPixmap();
    Q_UNUSED(event);
}

void
FFTPlot::fftUpdate(fftw_complex *out, unsigned int size) {
//    qDebug() << "fftUpdate";

    if (fftsize != size) {
        if (fftsize > 0) {
            delete [] fftampl;
        }
        fftsize = size;
        fftampl = new qreal[fftsize];
    }
    for (unsigned int ind=0; ind < fftsize; ind++) {
        qreal re = out[ind][0], im = out[ind][1];
        fftampl[ind] = qSqrt(re*re + im*im)/fftsize;
    }
    refreshPixmap();
}

#endif
