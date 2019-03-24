
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int (*boardtype)[15];


//score.c
void m1m7(int m1,int m2,int m3,int m4,int m5,int m6,int m7,int player,int *live2,int *dead3,int *live3,int *dead4,int *live4,int *win,int *b,int *a);
void rowscore(int hov,int player,int (*board)[15],int *live2,int *dead3,int *live3,int *dead4,int *live4,int *win);
void diascore(int lor,int player,int (*board)[15],int *live2,int *dead3,int *live3,int *dead4,int *live4,int *win);
int statescore(int player,int (*board)[15]);
int wincheck(int *win,int *dead4,int *live3,int *live4);


//board&move.c
boardtype boardinit();          
void boardprint(boardtype board);   
int opmove(int (*board)[15],int oplayer,int *previoustep,int *opstp);  
int imove(int (*board)[15],int mplayer,int *myprevioustep,int a,int b);  


//forbi.c
int m1m5(int m1,int m2,int m3,int m4,int m5,int m6,int m7,int *live3,int *dol4,int *b,int *a);
int forbi(int A,int B,int (*board)[15]);    


//nextstep.c
#define LINE 13
struct stateinfo{
    int ab[4][2];
    int score;
    int kidscore[LINE];
    struct stateinfo* kid[LINE];
    struct stateinfo* parent;
    int must;
    int layer;
    int kidsnum;
};
int layerfill(struct stateinfo* step0,int boardvalue[3],int (*board)[15],int oripalyer,int yrn);
int kidinfobuild(int layer,int a,int b,int score);
int kidslect(int (*board)[15],int player,struct stateinfo* step0,int a,int b,int *minscore,int *minkidindex,int *skidindex,int opmust[3],int *forbivalue,int (*forbiab)[2]);
int nodemax10(int (*board)[15],int player,struct stateinfo* step0,int *opstp,int oriplayer);
int minmaxkid(int max_ornot,struct stateinfo *step0);
int nodevalue(int (*board)[15],struct stateinfo *step0,int oriplayer);
int nextstep(int player,int (*board)[15],int *nestp,int *opstp);
int forbifill(int forbivalue[],int (*forbiab)[2],int (*board) [15],int yrn);


//mode.c
void man_machine(int (*board)[15]);
void man_man(int (*board)[15]);
void machine_machine();



