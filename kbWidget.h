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

#ifndef KBWIDGET_H
#define KBWIDGET_H

#include <QWidget>
#include <QPen>
#include <QPixmap>
#include <QColor>
#include <QVector>
#include <QString>
#include <QMouseEvent>

class KBMIDIChannel {
public:
    KBMIDIChannel();
    ~KBMIDIChannel();

    QString name;
    unsigned char keyvel[256];
};

//MidC = 60 C_4 261.6Hz

class KBWidget : public QWidget {
    Q_OBJECT
public:
    KBWidget(QWidget *parent = 0);
    KBWidget(QVector<int> _channels,
             unsigned int _minnote = 33, unsigned int _maxnote = 120,
             QWidget *parent = 0);
    ~KBWidget();
    QPixmap pixmap;
signals:
    void sig_note_on(unsigned char chan, unsigned char note, unsigned char vel);
    void sig_note_off(unsigned char chan, unsigned char note);
public slots:
    void note_on (unsigned char chan, unsigned char note, unsigned char vel);
    void note_off(unsigned char chan, unsigned char note);
protected:
    void paintEvent       (QPaintEvent *event);
    void resizeEvent      (QResizeEvent *event);
    void mousePressEvent  (QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void keyPressEvent    (QKeyEvent *event);
private:
    unsigned char note_vel (unsigned char chan, unsigned char note);
    unsigned int findKey(unsigned int x, unsigned int y);

    void refreshPixmap();
    unsigned int minnote, maxnote;
    unsigned int nchans;
    QVector<KBMIDIChannel> channels;
    QVector<int> chanlist;

    QRectF keyRects[256];
    unsigned int keyLayer[256];

    unsigned int mouse_startx, mouse_starty, mouse_startbutton, mouse_curnote;
};

#endif // KBWIDGET_H
