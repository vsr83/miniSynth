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

#include <QStylePainter>
#include <QDebug>
#include <QPen>
#include <qmath.h>
#include <math.h>

FFTPlot::FFTPlot(unsigned int _numPlots, qreal _periodLength, QWidget *parent)
    : QWidget(parent),
      fftSizes(_numPlots, 0),
      fftAmpls(_numPlots, 0),
      fftPens (_numPlots) {

    numPlots = _numPlots;
    periodLength = _periodLength;

    defaultPen.setStyle(Qt::SolidLine);
    defaultPen.setWidth(2);
    defaultPen.setColor(QColor(0, 0, 0));
    for (unsigned int ind_plot = 0; ind_plot < numPlots; ind_plot++) {
        fftPens[ind_plot] = defaultPen;
    }

    minfreq = 10;
    maxfreq = 10000;

    minampl = 1e-6;
    maxampl = 0.201;
}

FFTPlot::~FFTPlot() {
    for (unsigned int ind_plot = 0; ind_plot < numPlots; ind_plot++) {
        qreal * fftampl = fftAmpls[ind_plot];
        delete [] fftampl;
        fftAmpls[ind_plot] = (qreal *)0;
    }
}

void
FFTPlot::setPen(unsigned int ind_plot, QPen pen) {
    Q_ASSERT(ind_plot < numPlots);
    fftPens[ind_plot] = pen;
}

void
FFTPlot::refreshPixmap() {
    pixmap = QPixmap(size());
    pixmap.fill();

    qDebug() << "refresh";

    int borderx = 40, bordery = 20, padding = 10, ticksize = 5;

    QPainter painter(&pixmap);
    QPen penWave, penGrid, penBorder;

//    painter.setRenderHint(QPainter::Antialiasing, true);


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

    qreal sx = (qreal)(ww - borderx - 2*padding) / (maxfl - minfl),
          sy = (qreal)(wh - bordery - 2*padding) / (maxampldB - minampldB);

    for (int fl = qFloor(minfl); fl < qCeil(maxfl); fl++) {
        qreal x = padding + borderx
                + sx*(qreal)(log10((double)qPow(10, fl)) - minfl);
        QString str = QString("%1 Hz").arg(qPow(10, fl));
        painter.drawText((int)x, wh-padding, str);
    }
    for (int al = qFloor(minampll); al < qCeil(maxampll); al++) {
        qreal y = wh - padding - bordery
                - (sy*20*(qreal)(log10(qPow(10, al)) - minampll));
        QString str = QString("%1 dB").arg(-20-20*al);
        painter.drawText((int)padding, y+10, str);
    }
    painter.setPen(penBorder);
    painter.drawLine(borderx+padding, padding, ww-padding, padding);
    painter.drawLine(ww-padding, padding, ww-padding, wh-bordery-padding);
    painter.drawLine(borderx+padding, wh-bordery-padding, ww-padding, wh-bordery-padding);

    for (int fl = qFloor(minfl); fl < qCeil(maxfl); fl++) {
        qreal x = padding+borderx+sx*(qreal)(log10((double)qPow(10, fl)) - minfl);
        painter.drawLine((int)x, padding, (int)x, wh-bordery-padding+ticksize);
    }
    for (int al = qFloor(minampll); al < qCeil(maxampll); al++) {
        qreal y = wh-padding-bordery-(sy*20*(qreal)(log10(qPow(10, al)) - minampll));
        painter.drawLine((int)borderx+padding-ticksize, y, (int)ww-padding, y);
    }

    painter.setClipRect(borderx+padding, padding,
                        ww-borderx-padding*2, wh-bordery-padding*2);
    painter.setClipping(true);


    painter.setPen(penGrid);
    for (int fl = qFloor(minfl); fl < qCeil(maxfl); fl++) {
        for (int df = 1; df < 10; df++) {
            qreal x = borderx + padding
                    + sx*(qreal)(log10((double)qPow(10, fl)*df) - minfl);
            painter.drawLine((int)x, padding, (int)x, wh-bordery-padding);
        }
    }
    for (int al = qFloor(minampll); al < qCeil(maxampll); al++) {
        for (int da = 1; da < 10; da++) {
            qreal y = wh - bordery - padding
                    - (sy*20*(qreal)(log10(qPow(10, al)*da) - minampll));
            painter.drawLine(borderx+padding, y, ww-padding, y);
        }
    }

    for (unsigned int ind_plot = 0; ind_plot < numPlots; ind_plot++) {
        qreal * fftampl = fftAmpls[ind_plot];
        painter.setPen(fftPens[ind_plot]);

        if (fftSizes[ind_plot] != 0 && fftampl != 0) {
            for (unsigned int ind = 1; ind < fftSizes[ind_plot]/2; ind++) {
                qreal prevampl, ampl;

                prevampl = wh-padding-bordery
                         - (sy*20*(qreal)(log10((double)fftampl[ind-1]) - minampll));
                ampl     = wh-padding-bordery
                         - (sy*20*(qreal)(log10((double)fftampl[ind]  ) - minampll));

                qreal x     = padding+borderx+(sx)*(qreal)(log10( ((double)ind)/periodLength) - minfl),
                      prevx = padding+borderx+(sx)*(qreal)(log10( ((double)(ind-1))/periodLength) - minfl);
                QPointF pstart(prevx, prevampl),
                        pend(x, ampl);

                // Prevent infinitely long line to -infinity
                if (ind == 1) {
                    prevx = padding+borderx;
                }
            if (x > padding && prevx > padding)
                painter.drawLine(pstart, pend);
//            else {
//               qDebug() << ind_plot << ind << prevx << prevampl << x << ampl
//                         << fftampl[ind - 1] << fftampl[ind];
//            }
//            if (ind_plot == 2 && ind == 1) {
//                    pstart.setX(pstart.x() - padding);
//                    pend.setX(pend.x() - padding);
//                    painter.drawLine(pstart, pend);
//                }
            }
        }
    }

   update();

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

#ifdef USE_FFTW
void
FFTPlot::fftUpdate(fftw_complex *out, unsigned int size, unsigned int ind_dataset) {
    Q_ASSERT(ind_dataset < numPlots);

    if (fftSizes[ind_dataset] != size) {
        if (fftSizes[ind_dataset] > 0) {
            delete [] fftAmpls[ind_dataset];
        }
        fftSizes[ind_dataset] = size;
        fftAmpls[ind_dataset] = new qreal[size];
    }
    qreal *fftAmpl = fftAmpls[ind_dataset];
    for (unsigned int ind=0; ind < fftSizes[ind_dataset]; ind++) {
        qreal re = out[ind][0], im = out[ind][1];

        fftAmpl[ind] = 0.5*qSqrt(re*re + im*im)/fftSizes[ind_dataset];
        if (fftAmpl[ind] == 0) {
            fftAmpl[ind] = minampl;
        }
    }
    refreshPixmap();
}
#else
void
FFTPlot::fftUpdate(std::complex<qreal> *out, unsigned int size, unsigned int ind_dataset) {
    Q_ASSERT(ind_dataset < numPlots);

//    qDebug() << ind_dataset << size;

    if (fftSizes[ind_dataset] != size) {
        if (fftSizes[ind_dataset] > 0) {
            delete [] fftAmpls[ind_dataset];
        }
        fftSizes[ind_dataset] = size;
        fftAmpls[ind_dataset] = new qreal[size];
    }
    qreal *fftAmpl = fftAmpls[ind_dataset];
    for (unsigned int ind=0; ind < fftSizes[ind_dataset]; ind++) {
        qreal re = out[ind].real(), im = out[ind].imag();

        fftAmpl[ind] = 0.5*qSqrt(re*re + im*im)/fftSizes[ind_dataset];
        if (fftAmpl[ind] == 0) {
            fftAmpl[ind] = minampl;
        }
    }
    refreshPixmap();
}
#endif

