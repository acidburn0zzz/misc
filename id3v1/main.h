#ifndef _MAIN_H_
#define _MAIN_H_

struct ID3 {
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[29];
    char track;
    char genre;
};

#endif //_MAIN_H_
