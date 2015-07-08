This is a implementation of a very simple software synthesizer using Qt for
the graphical user interface (GUI) and ALSA for MIDI control.

This code implements linear synthesis with controllable timbre and ADSR 
envelope. MIDI control is available only on GNU/Linux platforms with ALSA since
Qt does not provide platform-independent MIDI support. On other platforms a 
virtual keyboard can be controlled with the mouse. 

The current version suffers from some delay problems probably associated with
the class QAudioOutput.

The computation of the waveform is implemented with waveform.cpp, waveform.h, 
linearSynthesis.cpp and linearSynthesis.h. The files ADSRenvelope.cpp and 
ADSRenvelope.h implement data structures and computation of the ADSR envelope.
Assembly of the raw audio data is performed in outputQt.cpp and outputQt.h.
The MIDI control is implemented using ALSA in midiThread.cpp and midiThread.h.

The files ADSRplot.*, ADSRwidget.*, kbWidget.*, mainWindow.*, waveformPlot.*, 
waveformWidget.* implement the widgets for the GUI. Interaction with the
QAudioOutput is implemented in mainWindow.*.

![ScreenShot](https://raw.github.com/vsr83/miniSynth/master/scrshot.png)