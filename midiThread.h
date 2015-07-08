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

#ifdef MIDI_ALSA

#ifndef MIDITHREAD_H
#define MIDITHREAD_H

#include <QThread>
#include <QString>
#include <alsa/asoundlib.h>

class MidiThread : public QThread {
    Q_OBJECT
public:
    MidiThread(const QString &_portname);
    void stop();

    QString portname;
signals:
    void note_on (unsigned char chan, unsigned char note, unsigned char vel);
    void note_off(unsigned char chan, unsigned char note);
protected:
    void run();
private:
    volatile bool stopped;
    snd_rawmidi_t * midi_in;
};

#endif // MIDITHREAD_H

#endif
