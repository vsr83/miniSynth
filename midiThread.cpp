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

#include "midiThread.h"
#include <iostream>
#include <QDebug>

MidiThread::MidiThread(const QString &_portname) {
    portname = _portname;
    stopped = false;
}

void
MidiThread::run() {
  stopped = false;

  //const char* portname = "hw:1,0,0";
  int mode = SND_RAWMIDI_SYNC;

  int status = snd_rawmidi_open(&midi_in, NULL, portname.toStdString().c_str(), mode);
  if (status < 0) {
    qWarning() << "Problem opening MIDI input: "
               << snd_strerror(status);
    return;
  }
  while (1) {
    unsigned char buffer[3];
    int status = snd_rawmidi_read(midi_in, buffer, 3);
    if (status < 0) {
        std::cerr << "Problem reading MIDI input: "
                  << snd_strerror(status)
                  << std::endl;
    }

    unsigned char func = buffer[0],
                  data1 = buffer[1],
                  data2 = buffer[2];

    if (func >= 128 && func <= 143) {
        unsigned char channel = func - 127;
        unsigned char note    = data1;
        unsigned char velocity= data2;

        Q_UNUSED(velocity);
        emit note_off(channel, note);
    }
    if (func >= 144 && func <= 159) {
        unsigned char channel = func - 143;
        unsigned char note    = data1;
        unsigned char velocity= data2;

        if (velocity > 0) {
            emit note_on(channel, note, velocity);
        } else {
            emit note_off(channel, note);
        }
    }
  }
}
void
MidiThread::stop() {
    qDebug() << "Closing MIDI";
    snd_rawmidi_close(midi_in);
    stopped = true;
}

#endif
