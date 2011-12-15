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

#ifndef __STATE_H__
#define __STATE_H__

#include <fstream>

#include "character.h"
#include "struct.h"

enum type {ZSNES, SNES9X};

class State {
private:
    //Valeurs de la partie
    Character *_chars;
    s_items _items;
    unsigned int _gold;
    s_time _time;

    //Informations sur le fichier
    unsigned char *_buffer;
    char *_fileName;
    unsigned int _fileSize;
    std::fstream _file;
    char _savetype;
    int _offset;

public:
    State(const char *file);
    ~State();

    Character *getChars();
    s_items getItems();
    void setItems(s_items items);
    unsigned int getGold();
    void setGold(unsigned int gold);
    unsigned int getTime(); //Retourne le temps en minutes
    void setTime(unsigned int time); //Param : temps en minutes
    //void setChars(Character *chars);

    int readFile();
    int writeFile();

    int getFileSize();

    int uncompressGzSave();
};

#endif //__STATE_H__
