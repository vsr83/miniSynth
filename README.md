This is a implementation of a very simple software synthesizer using Qt for the graphical user interface (GUI) and ALSA for MIDI control. 

This code implements linear synthesis with controllable timbre, ADSR envelope and various adjustable FIR filters. MIDI control is available only on GNU/Linux platforms with ALSA since Qt does not provide platform-independent support for MIDI. On all platforms, a virtual keyboard can be controlled with the mouse. Use the right button to sustain notes. Clipping can be avoided by adjusting the timbre appropriately.

In order to enable the spectrum plot, you need libfftw and to enable the option use_fftw in miniSynth.pro. The current version consumes a significant amount of CPU time due to rapid updates to the FFTPlot graph. 

The computation of the waveform is implemented in waveform.cpp, waveform.h, linearSynthesis.cpp and linearSynthesis.h. The files ADSRenvelope.cpp and ADSRenvelope.h implement data structures and computation for the ADSR envelope. Assembly of the raw audio data is performed in outputQt.cpp and outputQt.h. The MIDI control is implemented with ALSA in midiThread.cpp and midiThread.h.

The files ADSRplot.\*, ADSRwidget.\*, fftplot.\*, filterWidget.\*, kbWidget.\*, mainWindow.\*, modulationWidget.\*, waveformPlot.\*, waveformWidget.\* implement the widgets for the GUI. Interaction with the QAudioOutput is implemented in mainWindow.\*.

![ScreenShot](https://raw.github.com/vsr83/miniSynth/master/scrshot.png)
