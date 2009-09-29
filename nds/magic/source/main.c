#include <PA9.h>

#define TRUE 1
#define FALSE 0

void clear_screens();
void clear_screen(int no);

int get_nb_players();
void get_players_name(int nb, char **names);

int main() {
    time_t INIT_TIME;
    time_t nb_sec_passed;
    struct tm *time_passed;

    int nb_players;
    char **names;

    int i;

    PA_Init();
    PA_InitVBL();

    PA_InitText(0, 0);
    PA_InitText(1, 0);

    //~ nb_players = get_nb_players();
    nb_players = 3;

    names = malloc(sizeof(char *) * nb_players);
    for (i=0; i<nb_players; i++)
        names[i] = malloc(sizeof(char) * 8);

    get_players_name(nb_players, names);

    INIT_TIME = time(NULL);

    while (true) {
        nb_sec_passed = time(NULL) - INIT_TIME;
        time_passed = localtime(&nb_sec_passed);

        clear_screens();
        PA_OutputText(1, 0, 0, "Now");
        PA_OutputText(1, 2, 1, "%02d:%02d:%02d %04d/%02d/%02d", PA_RTC.Hour, PA_RTC.Minutes, PA_RTC.Seconds, PA_RTC.Year, PA_RTC.Month, PA_RTC.Day);
        PA_OutputText(1, 0, 3, "Time played");
        PA_OutputText(1, 2, 4, "%02d:%02d:%02d", time_passed->tm_hour, time_passed->tm_min, time_passed->tm_sec);

        if (Stylus.Held)
            PA_OutputText(0, 0, 0, "Stylus X : %03d\nStylus Y : %03d", Stylus.X, Stylus.Y);
        else
            PA_OutputText(0, 0, 0, "Stylus X : %03d\nStylus Y : %03d", 0, 0);

        for (i=0; i<nb_players; i++)
            PA_OutputText(0, 0, i+4, "Player %d: %s", i+1, names[i]);

        PA_WaitForVBL();
    }

    return 0;
}

void clear_screens() {
    clear_screen(0);
    clear_screen(1);
}

void clear_screen(int no) {
    int y;

    for (y=0; y<24; y++) {
        PA_OutputText(no, 0, y, "                                ");
    }
}

int get_nb_players() {
    int nb = 0;

    while (nb < 10) {
        PA_OutputText(0, 0, 0, "Frames: %d", nb);
        PA_WaitForVBL();
        nb++;
    }

    return nb;
}

void get_players_name(int nb, char **names) {
    s32 pos = 0;
    char letter = 0;
    int player = 0;

    PA_InitKeyboard(1);
    PA_KeyboardIn(25, 100);

    while (player < nb) {
        clear_screen(1);

        letter = PA_CheckKeyboard();

        if (letter > 31 && pos < 7) {
            names[player][pos] = letter;
            pos++;
        } else if ((letter == PA_BACKSPACE) && pos) {
            pos--;
            names[player][pos] = ' ';
        } else if (letter == '\n') {
            names[player][8] = '\0';
            player++;
            pos = 0;
        }

        PA_OutputSimpleText(1, 8, 11, names[player]);
        PA_OutputText(1, 0, 0, "Player %d/%d", player+1, nb);

        PA_WaitForVBL();
    }

    PA_KeyboardOut();
}
