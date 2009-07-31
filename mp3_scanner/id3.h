#ifndef __ID3_H__
#define __ID3_H__

typedef struct id3v1_s id3v1_s;
struct id3v1_s {
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[29];
    unsigned char track;
    unsigned char genre;
};

typedef struct id3_file_s id3_file_s;
struct id3_file_s {
    FILE *file;
    char isID3;
};

id3_file_s *id3_file_open(char *fn);
void id3_file_close(id3_file_s *id3f);
id3v1_s *id3_file_read(id3_file_s *id3f);

#endif /*__ID3_H__*/
