#ifndef __PENDU_H__
#define __PENDU_H__

enum {COL_DEF_GREEN = 1, COL_DEF_YELLOW, COL_DEF_RED, COL_DEF_BLUE};

enum {MB_OK, MB_YESNO};
enum {ANS_NONE, ANS_YES, ANS_NO};

void msleep(int ms);
int message_box(char *message, int type);
void quit(int ret, char *errmsg);

#endif /*__PENDU_H__*/
