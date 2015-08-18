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

#ifndef FFTPLOT_H
#define FFTPLOT_H

#ifdef USE_FFTW
#include <fftw3.h>
#else
#include "fft.h"
#endif

#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QPen>

class FFTPlot : public QWidget {
    Q_OBJECT
public:
    FFTPlot(unsigned int _numPlots, qreal _periodLength, QWidget *parent = 0);
    ~FFTPlot();
    void setPen(unsigned int ind_plot, QPen pen);

public slots:
#ifdef USE_FFTW
    void fftUpdate     (fftw_complex *out, unsigned int size, unsigned int ind_dataset);
#else
    void fftUpdate     (std::complex<qreal> *out, unsigned int size, unsigned int ind_dataset);
#endif
protected:
    void paintEvent (QPaintEvent  *event);
    void resizeEvent(QResizeEvent *event);
private:
    void refreshPixmap();
    QPixmap pixmap;

    QVector <unsigned int> fftSizes;
    QVector <qreal *>      fftAmpls;
    QVector <QPen>         fftPens;

    unsigned int numPlots;
    QPen defaultPen;

    qreal periodLength;
    unsigned int minfreq, maxfreq;
    qreal minampl, maxampl;
};


#endif // FFTPLOT_H
