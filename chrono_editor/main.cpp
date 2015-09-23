/***************************************************************************
 * Copyright (C) 2010 Lemay, Mathieu                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 *                                                                         *
 * You can contact the original author at acidrain1@gmail.com              *
 ***************************************************************************/

#include <iostream>
#include <iomanip>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "gui.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTranslator tr;
    MainWindow *win;

    QString locale = QLocale::system().name().section('_', 0, 0);
    tr.load(QString("chrono_editor_") + locale);
    app.installTranslator(&tr);

    if (argc == 2) {
        win = new MainWindow(argv[1]);
    } else {
        win = new MainWindow();
    }

    win->show();

    return app.exec();
}
