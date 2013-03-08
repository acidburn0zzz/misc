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

#ifndef __VUE_H__
#define __VUE_H_

#include <QMainWindow>

class QAction;
class QComboBox;
class QFileDialog;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QMenu;
class QSpinBox;
class QString;
class QTabWidget;
class QVBoxLayout;
class QWidget;

class SRAMFile;
class Game;

class Vue : public QMainWindow {
    Q_OBJECT

public:
    Vue(QWidget *parent = 0);
    Vue(const char *fn, QWidget *parent = 0);
    ~Vue();

private slots:
    void open(QString fn = "");
    void save();
    QString selectFile();
    void selectGame(int n);

private:
    QString _name, _imageFile;

    QWidget *centralWidget;
    QTabWidget *tabPersos;
    QWidget *wPersos;

    QMenu   *mnuFichier;
    QAction *actOuvrir;
    QAction *actSauvegarder;
    QAction *actQuitter;
    QMenu   *mnuAide;
    QAction *actAbout;
    QAction *actAboutQt;

    QVBoxLayout *centralLayout;
    QHBoxLayout *hlayPerso;
    QGridLayout *gridStatsBase;
    QGridLayout *gridStats;
    QGridLayout *gridItems;

    QLabel *lblImage;
    QLabel *lblLevel;
    QLabel *lblExp;
    QLabel *lblCurrentHP;
    QLabel *lblMaxHP;
    QLabel *lblCurrentMP;
    QLabel *lblMaxMP;
    QLabel *lblPower;
    QLabel *lblStamina;
    QLabel *lblSpeed;
    QLabel *lblMagic;
    QLabel *lblHit;
    QLabel *lblEvade;
    QLabel *lblMagicDef;
    QLabel *lblHelmet;
    QLabel *lblArmor;
    QLabel *lblWeapon;
    QLabel *lblRelic;

    QSpinBox *sbLevel;
    QSpinBox *sbExp;
    QSpinBox *sbCurrentHP;
    QSpinBox *sbMaxHP;
    QSpinBox *sbCurrentMP;
    QSpinBox *sbMaxMP;
    QSpinBox *sbPower;
    QSpinBox *sbStamina;
    QSpinBox *sbSpeed;
    QSpinBox *sbMagic;
    QSpinBox *sbHit;
    QSpinBox *sbEvade;
    QSpinBox *sbMagicDef;
    QComboBox *cmbHelmet;
    QComboBox *cmbArmor;
    QComboBox *cmbWeapon;
    QComboBox *cmbRelic;

    SRAMFile *_sramFile;
    Game *_game;

    void init();
    void creerMenus();
    void fillSpecificPerso(int perso);
    void selectItems(int perso);
    void afficherInformations();
    void updateGame();
};

#endif //__VUE_H__
