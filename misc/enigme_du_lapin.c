#include <stdio.h>

int lapin(int x) {
    if (x == 0) {
        return 1;
    } else if (x == 1) {
        return 1;
    } else {
        return lapin(x-1) + lapin(x-2)*12;
    }
}

int main() {
    int i;
    for (i=0; i<=10; i++) {
        printf("%d: %d\n", i, lapin(i));
    }
    
    return 0;
}
