#ifndef __FICHIERS_H__
#define __FICHIERS_H__

void get_high_scores(int *nb_players, char names[20][30], int scores[20]);
void save_high_scores(int nb_players, char names[20][30], int scores[20]);

void get_words(char words[30][25]);
void save_words(char words[30][25]);

#endif /*__FICHIERS_H__*/
