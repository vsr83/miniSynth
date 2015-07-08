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

#include <QApplication>
#include <QWidget>
#include "mainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow mwin;
    mwin.show();

    a.setStyleSheet("QGroupBox {border : 1px solid gray; border-radius: 9px;}"
                    "QGroupBox::title {subcontrol-origin: margin; left: 20px;top: -3px ;padding: 0 0px 0 0px;}");

    return a.exec();
}
