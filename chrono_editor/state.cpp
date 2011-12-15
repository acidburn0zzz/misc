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
#include <fstream>
//#include <memory>
#include <cstdlib>
#include <cstring>

#include <zlib.h>

#include "state.h"

using namespace std;

State::State(const char *file) {
    _chars = NULL;
    _buffer = NULL;

    _fileName = new char[strlen(file) + 1];
    strcpy(_fileName, file);

    _file.exceptions(ifstream::eofbit | ifstream::failbit | ifstream::badbit);

    if (readFile() != 0) {
        exit(EXIT_FAILURE);
    }
}

State::~State() {
    if (_fileName) delete _fileName;
    if (_buffer) delete _buffer;
    if (_chars) delete [] _chars;
}

Character *State::getChars() {
    return _chars;
}

s_items State::getItems() {
    return _items;
}

void State::setItems(s_items items) {
    _items = items;
}

unsigned int State::getGold() {
    return _gold;
}

void State::setGold(unsigned int gold) {
    if (gold > 9999999)
        gold = 9999999;
    _gold = gold;
}

unsigned int State::getTime() {
    return (_time.min1*10 + _time.min2) + (_time.hour1*10 + _time.hour2)*60;
}

void State::setTime(unsigned int time) {
    int min, hour;
    if (time > 99*60 + 59)
        time = 99*60 + 59;

    min = time % 60;
    hour = time / 60;

    _time.hour1 = hour / 10;
    _time.hour2 = hour % 10;
    _time.min1 = min / 10;
    _time.min2 = min % 10;
}

int State::readFile() {
    //Ouverture du fichier
    try {
        _file.open(_fileName, ios_base::in | ios_base::binary);
    } catch (fstream::failure e) {
        cerr << "Impossible d'ouvrir le fichier \"" << _fileName << "\"" << endl;
        return -1;
    }

    _fileSize = getFileSize();

    //Lecture du fichier dans le buffer
    _buffer = new unsigned char[_fileSize];

    //Remettre le getPtr au debut
    _file.seekg(0, ios_base::beg);
    _file.read((char*)_buffer, _fileSize);
    _file.close();

    //Verification du type de sauvegarde
    if (_buffer[0] == 'Z' && _buffer[1] == 'S' && _buffer[2] == 'N' && _buffer[3] == 'E' && _buffer[4] == 'S') {
        _savetype = ZSNES;
        _offset = 0x3212;
    } else if (_buffer[0] == 0x1f && _buffer[1] == 0x8b) { //format gzip utilise par snes9x
        _savetype = SNES9X;
        _offset = 0x131c1;
        uncompressGzSave();
    } else { //format incompatible
        cerr << "Format de sauvegarde incompatible" << endl;
        return -1;
    }

    //Lecture des struct s_ioCharacter
    s_ioCharacter *chars = new s_ioCharacter[7];
    memcpy(chars, _buffer + _offset, sizeof(s_ioCharacter)*7);

    //Creation des objets Character
    if (_chars)
        delete [] _chars;

    _chars = new Character[7];

    for (int i=0; i<7; i++) {
        _chars[i] = Character(chars[i]);
    }

    delete [] chars;

    //Lecture des items
    _offset -= 0x1ff;
    memcpy(&_items, _buffer + _offset, sizeof(s_items));

    //Lecture de l'or
    _offset += 0x853;
    memcpy(&_gold, _buffer + _offset, sizeof(int));

    //Lecture du temps
    _offset -= 0x2851; //TODO: Fonctionne pas si 99:59, peut etre autres cas
    memcpy(&_time, _buffer + _offset, sizeof(s_time));

    return 0;
}

int State::writeFile() {
    //Reinitialisation de l'offset
    if (_savetype == ZSNES) {
        _offset = 0x3212;
    } else if (_savetype == SNES9X) {
        _offset = 0x131c1;
    }

    //Actualisation du buffer avec les nouvelles valeurs
    //Persos
    s_ioCharacter *chars = new s_ioCharacter[7];
    for (int i=0; i<7; i++) {
        chars[i] = _chars[i].getIOCharacter();
    }
    memcpy(_buffer + _offset, _chars, sizeof(s_ioCharacter)*7);

    //Items
    _offset -= 0x1ff;
    memcpy(_buffer + _offset, &_items, sizeof(s_items));

    //Or
    _offset += 0x853;
    memcpy(_buffer + _offset, &_gold, sizeof(int));

    //Temps
    _offset -= 0x2851;
    memcpy(_buffer + _offset, &_time, sizeof(s_time));

    //Ecriture du fichier
    if (_savetype == ZSNES) {
        try {
            _file.open(_fileName, ios_base::out | ios_base::binary);
        } catch (fstream::failure e) {
            cerr << "Impossible d'ouvrir le fichier \"" << _fileName << "\"" << endl;
            return -1;
        }
        _file.write((char*)_buffer, _fileSize);
        _file.close();
    } else if (_savetype == SNES9X) {
        gzFile gzSave;
        gzSave = gzopen(_fileName, "wb");
        gzwrite(gzSave, _buffer, _fileSize);
        gzclose(gzSave);
    }

    return 0;
}

int State::getFileSize() {
    int s;

    _file.seekg(0, ios_base::end);
    s = _file.tellg();
    _file.seekg(0, ios_base::beg);

    return s;
}

int State::uncompressGzSave() {
    //On va chercher la taille du fichier decompresse
    //Les 4 derniers bytes du fichier
    unsigned int size;
    memcpy(&size, _buffer + _fileSize - 4, sizeof(int));
    _fileSize = size;

    //On va de-gzipper dans le buffer
    delete _buffer;
    _buffer = new unsigned char[size];

    gzFile gzSave;
    gzSave = gzopen(_fileName, "rb");
    gzread(gzSave, _buffer, _fileSize);
    gzclose(gzSave);

    return 0;
}
