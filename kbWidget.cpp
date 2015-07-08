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

#include "kbWidget.h"
#include <QStylePainter>
#include <QString>
#include <QDebug>

KBMIDIChannel::KBMIDIChannel() {
    name = QString("Empty");
    for (unsigned int key = 0; key < 256; key++) {
        keyvel[key] = 0;
    }
}

KBMIDIChannel::~KBMIDIChannel() {
}

void
KBWidget::paintEvent(QPaintEvent *) {
//    qDebug() << "paintEvent";
    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}

void
KBWidget::resizeEvent(QResizeEvent *event) {
//    qDebug() << "resizeEvent";
    refreshPixmap();
    Q_UNUSED(event);
}

void
KBWidget::refreshPixmap() {
//    qDebug() << "refreshPixmap";
    pixmap = QPixmap(size());
    pixmap.fill();

    QPainter painter(&pixmap);

    unsigned int ww = width(), wh = height();
    Q_UNUSED(wh);

    unsigned int nkeys = maxnote - minnote + 1;
    // 1-12 -> 1, 13-24 -> 2
    unsigned int noctaves = (nkeys+11)/12;

    unsigned int minoctave = minnote/12;
    unsigned int minnote12 = minnote%12;
    double startx = 0;
    switch(minnote12) {
        case 0: startx = 0; break;
        case 1: startx = 0; break;
        case 2: startx = 1; break;
        case 3: startx = 1; break;
        case 4: startx = 2; break;
        case 5: startx = 3; break;
        case 6: startx = 3; break;
        case 7: startx = 4; break;
        case 8: startx = 4; break;
        case 9: startx = 5; break;
        case 10: startx = 5; break;
        case 11: startx = 6; break;
    }

    double octavew = ((double)ww - 1)/((double)noctaves + (1 - startx)*0.1428);
    KBMIDIChannel midichan = channels[0];

    for (unsigned int stage = 1; stage <= 2; stage++) {
        for (unsigned int note = minnote; note <= maxnote; note++) {
            unsigned int octave = note/12 - minoctave;
            unsigned int note12 = (note+60)%12;
            unsigned int layer = 0;

            QRectF rect;
            switch (note12) {
            case 0: layer = 1;  rect = QRectF(octave*octavew +  0*octavew/7, 0, octavew/7, height()-1);break;
            case 1: layer = 2;  rect = QRectF(octave*octavew + 0.65*octavew/7, 0, 0.7*octavew/7, height()*0.6-1);break;
            case 2: layer = 1;  rect = QRectF(octave*octavew + 1*octavew/7, 0, octavew/7, height()-1);break;
            case 3: layer = 2;  rect = QRectF(octave*octavew + 1.65*octavew/7, 0, 0.7*octavew/7, height()*0.6-1);break;
            case 4: layer = 1;  rect = QRectF(octave*octavew + 2*octavew/7, 0, octavew/7, height()-1);break;
            case 5: layer = 1;  rect = QRectF(octave*octavew + 3*octavew/7, 0, octavew/7, height()-1);break;
            case 6: layer = 2;  rect = QRectF(octave*octavew + 3.65*octavew/7, 0, 0.7*octavew/7, height()*0.6-1);break;
            case 7: layer = 1;  rect = QRectF(octave*octavew + 4*octavew/7, 0, octavew/7, height()-1);break;
            case 8: layer = 2;  rect = QRectF(octave*octavew + 4.65*octavew/7, 0, 0.7*octavew/7, height()*0.6-1);break;
            case 9: layer = 1;  rect = QRectF(octave*octavew + 5*octavew/7, 0, octavew/7, height()-1);break;
            case 10:layer = 2;  rect = QRectF(octave*octavew + 5.65*octavew/7, 0, 0.7*octavew/7, height()*0.6-1);break;
            case 11:layer = 1;  rect = QRectF(octave*octavew + 6*octavew/7, 0, octavew/7, height()-1);break;
            }
            rect.translate(1-startx*0.1429*octavew, 0);

            keyRects[note] = rect;
            keyLayer[note] = layer;

            if (stage == layer) {
                if (stage == 1) {
                    painter.fillRect(rect, QColor("white"));
                } else if (stage == 2) {
                    painter.fillRect(rect, QColor("gray"));
                }
                if (midichan.keyvel[note]) {
                    painter.fillRect(rect, QColor("red"));
                }
                painter.drawRect(rect);
            }
        }
    }
    update();
}

KBWidget::KBWidget(QVector<int> _chanlist, unsigned int _minnote, unsigned int _maxnote,
                   QWidget *parent) : QWidget(parent) {
    minnote = _minnote;
    maxnote = _maxnote;

    chanlist = _chanlist;
    channels.reserve(16);
    for (unsigned int chan = 1; chan <= 16; chan++) {
        KBMIDIChannel midichan;
        channels[chan] = midichan;
    }
    nchans = chanlist.length();
}

KBWidget::KBWidget() {
    chanlist.clear();
    chanlist.push_back(1);
    minnote = 33;
    maxnote = 120;

    mouse_curnote = mouse_startx = mouse_starty = mouse_startbutton = 0;

    qDebug() << "KBWidget";
    channels.reserve(16);
    for (unsigned int chan = 1; chan <= 16; chan++) {
        KBMIDIChannel midichan;
        channels.push_back(midichan);
    }
    nchans = chanlist.length();
    qDebug() << "KBWidget-";
}

KBWidget::~KBWidget() {
}

void
KBWidget::note_on(unsigned char chan, unsigned char note, unsigned char vel) {
    Q_ASSERT(chan >= 1 && chan <= 16);
    KBMIDIChannel midichan = channels[chan - 1];
    midichan.keyvel[note] = vel;
    channels[chan - 1] = midichan;
    refreshPixmap();
    emit sig_note_on(chan, note, vel);
//    qDebug() << "KBnoteon" << chan << note << vel;
}

void
KBWidget::note_off(unsigned char chan, unsigned char note) {
    Q_ASSERT(chan >= 1 && chan <= 16);
    KBMIDIChannel midichan = channels[chan - 1];
    midichan.keyvel[note] = 0;
    channels[chan - 1] = midichan;
    refreshPixmap();
    emit sig_note_off(chan, note);
//    qDebug() << "KBnoteoff" << chan << note;
}

unsigned char
KBWidget::note_vel(unsigned char chan, unsigned char note) {
    Q_ASSERT(chan >= 1 && chan <= 16);
    KBMIDIChannel midichan = channels[chan - 1];
    return midichan.keyvel[note];
}

void
KBWidget::mousePressEvent(QMouseEvent *event) {
    if (mouse_startbutton) return;

    if (event->button() == Qt::LeftButton) {
        mouse_startbutton = event->button();
        mouse_startx = event->x();
        mouse_starty = event->y();

        int note = findKey(mouse_startx, mouse_starty);
        if (note > 0) {
            mouse_curnote = note;
            note_on(1, note, 255);
        }
    } else if (event->button() == Qt::RightButton){
        int note = findKey(event->x(), event->y());
        if (note > 0) {
            if (note_vel(1, note) == 0) {
                note_on(1, note, 255);
            } else {
                note_off(1, note);
            }
        }
    }
}

void
KBWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (mouse_startbutton == event->button()) {
        unsigned int note = findKey(mouse_startx, mouse_starty);
        if (note > 0 && mouse_curnote == note) {
            note_off(1, note);
            mouse_startx = mouse_starty = mouse_startbutton = 0;
        }
    } else {
        return;
    }
}

void
KBWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!mouse_startbutton) return;

    unsigned int note = findKey(event->x(), event->y());
    if (mouse_curnote != note && note > 0) {
        note_off(1, mouse_curnote);
        note_on(1, note, 255);
        mouse_startx = event->x();
        mouse_starty = event->y();
        mouse_curnote = note;
    }
}

unsigned int
KBWidget::findKey(unsigned int x, unsigned int y) {
    int key = 0;

//    qDebug() << x << y;

    for (unsigned int layer = 1; layer <= 2; layer++) {
        for (unsigned int note = minnote; note < maxnote; note++) {
            if (layer == keyLayer[note]) {
                QRectF keyRect = keyRects[note];

//                qDebug() << note << keyRect.left() << keyRect.right()
//                         << keyRect.bottom() << keyRect.top()
//                         << x << y;

                if (x > keyRect.left() && x <= keyRect.right() &&
                    y > keyRect.top() && y <= keyRect.bottom()) {
                    key = note;
                }
            }
        }
    }
    return key;
}
