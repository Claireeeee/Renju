#include "defs.h"
extern int kidindex;
extern int ran;

//人机
void man_machine(int (*board)[15])
{
    static int opprevioustep[2]={15,15};
    static int myprevioustep[2]={15,15};
    int mplayer=0;
    int oplayer=0;
    printf("Choose player(black: 0, white: 1):\n");   //模式选择
    scanf("%d",&oplayer);
    mplayer=!oplayer;
    if (mplayer==0) {
        imove(board,mplayer, myprevioustep,7,7);     //黑子先落
    }    
    int opstp[2];
    int nestp[2];
    for (; ; ) {
        if(opmove(board,oplayer,opprevioustep,opstp)==0) {     //读取输入
            printf("Stop.\n");
            break;
        } 
        else boardprint(board);
        if (oplayer==0&&forbi(opstp[0],opstp[1],board)==1) {   //禁手判断
            printf("Forbiden.\n");
            break;
        }
        if (statescore(oplayer,board)%10==7) {                 //输赢判断
            printf("You win.\n");
            break;
        }
        nextstep(mplayer,board,nestp,opstp);                   //己方落子
        boardprint(board);
        printf("nextstep\n");
        if(imove(board,mplayer,myprevioustep,nestp[0],nestp[1])==1) break;
        kidindex=0;
    }
}

//人人
void man_man(int (*board)[15]){
    int step[2];
    int bpre[2]={15.15};
    int wpre[2]={15,15};
    for(;;){
        printf("Player Black,");           //黑子
        if(opmove(board, 0,bpre,step)==0){
            printf("Stop.\n");
            break;
        }
        else { boardprint(board); printf("Player Black's step was %c %d\n",step[1]+96+1,15-step[0]);}
        if (statescore(0,board)%10==7) {       //输赢判断
            printf("Player Black win.\n");
            break;
        }
        if (forbi(step[0],step[1],board)==1) {    //禁手判断
            printf("Black Forbiden. White win.\n");
            break;
        }
        printf("Player White,");            //白子
        if(opmove(board,1,wpre,step)==0){
            printf("Stop.\n");
            break;
        }
        else {boardprint(board); printf("Player White's step was %c %d\n",step[1]+96+1,15-step[0]);}
        if (statescore(1,board)%10==7) {      //输赢判断
            printf("Player White win.\n");
            break;
        }
    }
    
}
//自己下，调试用
void machine_machine(){
    static int bprevioustep[2]={15,15};
    static int wprevioustep[2]={15,15};
    int bstp[2]={7,7};
    int wstp[2];
    boardtype board=boardinit();
    imove(board,0, bprevioustep,7,7);
    int m=0;
    for (; ; ) {
        if (m==0) {
            m=1;
            nextstep(1,board,wstp,bstp);
            if(imove(board,1,wprevioustep,wstp[0],wstp[1])==1) break;
            kidindex=0;
            ran=0;
            nextstep(0,board,bstp,wstp);
            if(imove(board,0,bprevioustep,bstp[0],bstp[1])==1) break;
            kidindex=0;
        }
        else{
            nextstep(1,board,wstp,bstp);
            if(imove(board,1,wprevioustep,wstp[0],wstp[1])==1) break;
            kidindex=0;
            nextstep(0,board,bstp,wstp);
            if(imove(board,0,bprevioustep,bstp[0],bstp[1])==1) break;
            kidindex=0;
        }
        
    }
}

