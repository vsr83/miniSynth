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

#include "ADSRplot.h"

#include <QPen>
#include <QPainter>
#include <QStylePainter>

ADSRPlot::ADSRPlot(QWidget *parent) : QWidget(parent) {
    border = 15;
}

ADSRPlot::~ADSRPlot() {
}

void
ADSRPlot::setValues(ADSREnvelope &_envelope) {
    envelope = _envelope;
    refreshPixmap();
}

void
ADSRPlot::refreshPixmap() {
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

    qreal total = 1.5*(qreal)(envelope.attackTime
                            + envelope.decayTime
                            + envelope.releaseTime);
    unsigned int attackx = border
                         + (unsigned int)((qreal)ww * (qreal)envelope.attackTime / total),
                 decayx  = attackx
                         + (unsigned int)((qreal)ww * (qreal)envelope.decayTime / total),
                 releasex= border
                         + (unsigned int)((qreal)ww * (1-(qreal)envelope.releaseTime / total));


    unsigned int initialy = height() - border
                          - (unsigned int)((qreal)wh * envelope.initialAmpl),
                 peaky    = height() - border
                          - (unsigned int)((qreal)wh * envelope.peakAmpl),
                 sustainy = height() - border
                          - (unsigned int)((qreal)wh * envelope.sustainAmpl);

    painter.setPen(penGrid);
    painter.drawLine(border, initialy, ww+border, initialy);
    painter.drawLine(border, sustainy, ww+border, sustainy);
    painter.drawLine(border, peaky, ww+border, peaky);
    painter.drawLine(attackx,  border/2, attackx,  wh + border);
    painter.drawLine(decayx,   border/2, decayx,   wh + border);
    painter.drawLine(releasex, border/2, releasex, wh + border);

    painter.setPen(penBorder);
    painter.drawText(0, initialy, tr("I"));
    painter.drawText(0, sustainy, tr("S"));
    painter.drawText(0, peaky,    tr("P"));
    painter.drawText(attackx/2,  wh+border*2, tr("A"));
    painter.drawText((attackx + decayx)/2,   wh+border*2, tr("D"));
    painter.drawText((ww + border + releasex)/2, wh+border*2, tr("R"));

    painter.drawLine(border, height() - border, width()-border, height() - border);
    painter.drawLine(border, border, border, height() - border);

    painter.setPen(penWave);
    painter.drawLine(border, initialy, attackx, peaky);
    painter.drawLine(attackx, peaky, decayx, sustainy);
    painter.drawLine(decayx, sustainy, releasex, sustainy);
    painter.drawLine(releasex, sustainy, border+ww, border+wh);

    update();
}

void
ADSRPlot::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
    Q_UNUSED(event);
}

void
ADSRPlot::resizeEvent(QResizeEvent *event) {
    refreshPixmap();
    Q_UNUSED(event);
}
