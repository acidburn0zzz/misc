#include <stdio.h>
#include <string.h>

static void purger() {
    char c;
    
    while ((c = getchar()) != '\n' && c != EOF) {}
    
} 

static void remove_line_feed(char *chaine)
{
    char *p = strchr(chaine, '\n');

    if (p) {
        *p = 0;
    } else {
        purger();
    }
}   

int main(int argc, char **argv) {
    char a[10], b[10];
    FILE *f;
    
    printf("Pointeurs a %p (%i), b %p (%i)\n", a, sizeof a, b, sizeof b);
    
    memset(a, 0, sizeof a);
    memset(b, 0, sizeof b);
    
    printf("Entrez un mot : ");
    fgets(a, sizeof a, stdin);
    remove_line_feed(a);
    printf("Le mot est : %s\n", a);
    
    printf("Entrez un mot : ");
    fgets(b, sizeof b, stdin);
    printf("Le mot est : %s\n", b);
    
    f = fopen("test", "w+b");
    fwrite(a, sizeof a, 1, f);
    fclose(f);
    return 0;
}
