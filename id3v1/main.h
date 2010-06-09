#ifndef _MAIN_H_
#define _MAIN_H_

struct ID3 {
    unsigned char tag[3];
    unsigned char title[30];
    unsigned char artist[30];
    unsigned char album[30];
    unsigned char year[4];
    unsigned char comment[29];
    unsigned char track;
    unsigned char genre;
};

#endif /*_MAIN_H_*/
