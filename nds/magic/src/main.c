#include <PA9.h>

#include "all_gfx.h"

//~ #define TRUE 1
//~ #define FALSE 0

void clear_screens();
void clear_screen(int no);

void init();
int get_nb_players();
void get_players_name(int nb, char **names);
void main_game(int nb, char **names);

int main() {
    int nb_players, i;
    char **names = NULL;

    init();

    nb_players = get_nb_players();

    names = malloc(sizeof(char *) * nb_players);
    for (i=0; i<nb_players; i++)
        names[i] = malloc(sizeof(char) * 8);

    get_players_name(nb_players, names);

    main_game(nb_players, names);

    return 0;
}

void init() {
    PA_Init();
    PA_InitVBL();

    PA_InitText(0, 0);
    PA_InitText(1, 0);

    PA_LoadSpritePal(0, // Screen
            0, // Palette number
			(void*)btn_palette);	// Palette name
}

void main_game(int nb_players, char **names) {
    time_t INIT_TIME;
    time_t nb_sec_passed;
    struct tm *time_passed;
    int *pts, i;

    pts = malloc(sizeof(int) * nb_players);

    for (i=0; i<nb_players; i++) {
        PA_CreateSprite(0, 4*i+0, (void*)btn_moins_1, OBJ_SIZE_32X32, 1, 0, 64*i+0, 128);
        PA_CreateSprite(0, 4*i+1, (void*)btn_plus_1, OBJ_SIZE_32X32, 1, 0, 64*i+32, 128);
        PA_CreateSprite(0, 4*i+2, (void*)btn_moins_5, OBJ_SIZE_32X32, 1, 0, 64*i+0, 160);
        PA_CreateSprite(0, 4*i+3, (void*)btn_plus_5, OBJ_SIZE_32X32, 1, 0, 64*i+32, 160);
    }

RESET:
    INIT_TIME = time(NULL);

    for (i=0; i<nb_players; i++)
        pts[i] = 20;

    while (true) {
        nb_sec_passed = time(NULL) - INIT_TIME;
        time_passed = localtime(&nb_sec_passed);

        /* Reset */
        if (Pad.Newpress.Start)
            goto RESET;

        /* Modifications au score */
        for (i=0; i<nb_players; i++) {
            if (PA_SpriteTouched(4*i+0) && Stylus.Newpress)
                pts[i]--;
            if (PA_SpriteTouched(4*i+1) && Stylus.Newpress)
                pts[i]++;
            if (PA_SpriteTouched(4*i+2) && Stylus.Newpress)
                pts[i] -= 5;
            if (PA_SpriteTouched(4*i+3) && Stylus.Newpress)
                pts[i] += 5;

            if (pts[i] < 0)
                pts[i] = 0;
        }

        /* Affichage du temps */
        clear_screens();
        PA_OutputText(1, 0, 0, "Now");
        PA_OutputText(1, 2, 1, "%02d:%02d:%02d %04d/%02d/%02d", PA_RTC.Hour, PA_RTC.Minutes, PA_RTC.Seconds, PA_RTC.Year, PA_RTC.Month, PA_RTC.Day);
        PA_OutputText(1, 0, 3, "Time played");
        PA_OutputText(1, 2, 4, "%02d:%02d:%02d", time_passed->tm_hour, time_passed->tm_min, time_passed->tm_sec);

        /* Noms */
        for (i=0; i<nb_players; i++)
            PA_OutputText(0, 8*i, 0, "%s", names[i]);

        /* Points */
        for (i=0; i<nb_players; i++)
            PA_OutputText(0, 8*i+3, 2, "%02d", pts[i]);

        PA_WaitForVBL();
    }
}

int get_nb_players() {
    int i, nb=0, ok=false;

    PA_CreateSprite(0, // Screen
			0, // Sprite number
			(void*)btn_2_players, // Sprite name
			OBJ_SIZE_64X32, // Sprite size
			1, // 256 color mode
			0, // Sprite palette number
			16, 80); // X and Y position on the screen
    PA_CreateSprite(0, 1, (void*)btn_3_players, OBJ_SIZE_64X32, 1, 0, 96, 80);
    PA_CreateSprite(0, 2, (void*)btn_4_players, OBJ_SIZE_64X32, 1, 0, 176, 80);

    PA_OutputText(0, 7, 0, "Nombre de joueurs");

    while (!ok) {
        for (i=0; i<3; i++) {
            if (PA_SpriteTouched(i)) {
                nb = i+2;
                ok = true;
            }
        }

        PA_WaitForVBL();
    }

    PA_DeleteSprite(0, 0);
    PA_DeleteSprite(0, 1);
    PA_DeleteSprite(0, 2);
    clear_screen(0);

    return nb;
}

void get_players_name(int nb_players, char **names) {
    s32 pos = 0;
    char letter = 0;
    int player = 0;

    PA_InitKeyboard(1);
    PA_KeyboardIn(25, 100);

    while (player < nb_players) {
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
        PA_OutputText(1, 0, 0, "Player %d/%d", player+1, nb_players);

        PA_WaitForVBL();
    }

    clear_screen(1);

    PA_KeyboardOut();
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
