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
