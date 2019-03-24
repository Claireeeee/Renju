
#include "defs.h"
//棋盘用二维int数组实现， ┍ ┑ ┯ ┕ ┙ ┷ ┝ ┥ ┿ ● ○ ▲ △对应 0 1 2 3 4 5 6 7 8 9 10 11 12


//构建空棋盘对应的二维int数组
boardtype boardinit()           
{
    int (*board)[15]=(boardtype)malloc(sizeof(int)*15*15);
    for (int a=0; a<15; a++) {
        for (int b=0;b<15; b++){
            if (a==0){
                if (b==0){
                    board[a][b]=0;
                }
                else if (b==14) board[a][b]=1;
                else board[a][b]=2;
            }
            if (a==14) {
                if (b==0)
                {
                    board[a][b]=3;
                }
                else if (b==14) board[a][b]=4;
                else board[a][b]=5;
            }
            if (0<a&&a<14) {
                if (b==0)
                {
                    board[a][b]=6;
                }
                else if (b==14) board[a][b]=7;
                else board[a][b]=8;
            }
        }
    }
    return board;
}

//棋盘输出
void boardprint(int (*board)[15]){               
    for (int a=0;a<15; a++) {
        for (int b=0; b<15; b++){
            if (b==0){
                printf("%2d",15-a);
            }
            int i=board[a][b];
            if (i==0) {
                printf("┌");                      
            }
            else if (i==1) printf("┐");
            else if (i==2) printf("┬");
            else if (i==3) printf("└");
            else if (i==4) printf("┘");
            else if (i==5) printf("┴");
            else if (i==6) printf("├");
            else if (i==7) printf("┤");
            else if (i==8) printf("┼");
            else if (i==9) printf("●");
            else if (i==10) printf("○");
            else if (i==11) printf("▲");
            else if (i==12) printf("△");
            
        }
        printf("\n");
    }
    printf("  ABCDEFGHIJKLMNO\n");
}

//对手落子（读取输入，修改棋盘数组）  
//player指黑or白：0和1  previoustep记录前一步
int opmove(int (*board)[15],int oplayer,int *previoustep,int *opstp){   
    char blank,b1;
    int a,b;
    setbuf(stdin, NULL);
    printf("Please enter your next step(like \"b 5\". enter \'s 0\' to stop):");
    scanf("%c%c%d",&b1,&blank,&a);
    if (b1=='s') {
        return 0;
    }
    a=15-a;
    b=b1-'a';
    opstp[0]=a;opstp[1]=b;
    if (oplayer==0){
        board[a][b]=11;         //落三角，修改前一个三角为圆
        if (previoustep[0]!=15) {
            board[previoustep[0]][previoustep[1]]=9;
        }
        else {previoustep[0]=a;previoustep[1]=b;}
    }
    else{
        board[a][b]=12;          
        if (previoustep[0]!=15) {
            board[previoustep[0]][previoustep[1]]=10;
        }
        else {previoustep[0]=a;previoustep[1]=b;}
    }
    previoustep[0]=a;
    previoustep[1]=b;
    return 1;
}


//己方落子，棋盘修改
int imove(int (*board)[15],int mplayer,int *myprevioustep,int a,int b)
{
    if (mplayer==0){
        board[a][b]=11;
        if (myprevioustep[0]!=15) {
            board[myprevioustep[0]][myprevioustep[1]]=9;
        }
    }
    else{
        board[a][b]=12;
        if (myprevioustep[0]!=15) {
            board[myprevioustep[0]][myprevioustep[1]]=10;
        }
    }
    myprevioustep[0]=a;
    myprevioustep[1]=b;
    
    boardprint(board);
    printf("my step was %c %d\n",b+'a',15-a);
    if (statescore(mplayer,board)%10==7) {
        printf("I win.\n");
        return 1;
    }
    return 0;
}
