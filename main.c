#include "defs.h"
extern int kidindex;

int main() {
    int (*board)[15]=boardinit();
    int mode;
    printf("Choose mode(man-man: 0, man-machine: 1):\n");
    scanf("%d",&mode);
    //machine_machine();
    //return 0;
    if (mode==1) {
    man_machine(board);
    }
    else man_man(board);
}

