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
#include <QWidget>
#include <QPixmap>

class FFTPlot : public QWidget {
    Q_OBJECT
public:
    FFTPlot(QWidget *parent = 0);
    ~FFTPlot();
public slots:
    void fftUpdate(fftw_complex *out, unsigned int size);
protected:
    void paintEvent (QPaintEvent  *event);
    void resizeEvent(QResizeEvent *event);
private:
    void refreshPixmap();
    QPixmap pixmap;
    unsigned int fftsize;
    qreal *fftampl;

    int minfreq, maxfreq;
    qreal minampl, maxampl;
};


#endif

#endif // FFTPLOT_H
