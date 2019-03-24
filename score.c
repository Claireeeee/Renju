#include "defs.h"

//score.c

//棋局估分：行列+对角线四个方向遍历，
int statescore(int player,int (*board)[15])
{                             
    int score=0;
    int live2=0;
    int dead3=0;
    int live3=0;
    int dead4=0;
    int live4=0;
    int win=0;      
    
    rowscore(0, player, board, &live2, &dead3, &live3, &dead4, &live4,&win);//行
    if (win!=0) return win;
    rowscore(1, player, board, &live2, &dead3, &live3, &dead4, &live4,&win);//列
    if (win!=0) return win;
    diascore(0, player, board, &live2, &dead3, &live3, &dead4, &live4,&win);//y=x
    if (win!=0) return win;
    diascore(1, player, board, &live2, &dead3, &live3, &dead4, &live4,&win);//y=-x
    if (win!=0) return win;
    
    score=dead3*50+dead4*2000+live2*50+live3*2000;                         //打分
    wincheck(&win,&dead4,&live3,&live4);
    return win+score;
    
}

//如果出现活4、44、34、33或死4，记录为level为1、2、3、4、5，加在score的个位
int wincheck(int *win,int *dead4,int *live3,int *live4){
    if ((*live4!=0&&(*live3!=0||*dead4!=0))||(*live4>1)) *win=1;
    else if (*live4!=0||*dead4>1) *win=2;
    else if((*live3)>=1&&*dead4>=1) *win=3;
    else if (*live3>1) *win=4;
    else if (*dead4>0) *win=5;
    if (*win!=0) return 1;
    else return 0;
}



//行列扫描，每次取5个，用m1m7函数分析
void rowscore(int hov,int player,int (*board)[15],int *live2,int *dead3,int *live3,int *dead4,int *live4,int *win)
{
    int a=0,b=0;                        //hov：水平或垂直，0为水平  
    int m1,m2,m3,m4,m5,m6,m7;           //m1~m5为待分析的一组，m6、m7为该组的前一位和后一位
    int cir,trian,wall;
    if(player==0) {cir=9;trian=11;wall=10;}
    else {cir=10;trian=12;wall=9;}
    for (a=0; a<15; a++) {
        for (b=0; b<11; b++){
            if (hov==0){
                if (board[a][b]<9&&board[a][b+1]<9&&board[a][b+2]<9) continue;  //连续3位为空则跳过
                if (player==0&&b>0&&(board[a][b-1]==cir||board[a][b-1]==trian)) {
                    continue;
                }
                if (b==0) m6=wall;
                else m6=board[a][b-1];
                if (player==0&&(m6==cir||m6==trian)) {
                    continue;
                }
                if(b+5<15) m7=board[a][b+5];
                else m7=wall;
                if (player==0&&(m7==cir||m7==trian)) {
                    continue;
                }
            }
            
            else{   //hov==1
                if (board[b][a]<9&&board[b+1][a]<9&&board[b+2][a]<9) {continue;}
                if (player==0&&b>0&&(board[b-1][a]==cir||board[b-1][a]==trian)) {
                    continue;
                }
                if (b==0) m6=wall;
                else m6=board[b-1][a];
                if(b+5<15) m7=board[b+5][a];
                else m7=wall;
                if (player==0&&(m7==cir||m7==trian)) {
                    continue;
                }
            }
            if (hov==0){
                m1=board[a][b];
                m2=board[a][b+1];
                m3=board[a][b+2];
                m4=board[a][b+3];
                m5=board[a][b+4];
            }
            else{
                m1=board[b][a];
                m2=board[b+1][a];
                m3=board[b+2][a];
                m4=board[b+3][a];
                m5=board[b+4][a];
            }
            if(m1==wall||m2==wall||m3==wall||m4==wall||m5==wall) continue;
            if (player==0) {                 //黑子时检查x0000x0，此时因为长连禁手，该棋型为死四（而不是活四）
                if(m7<9&&(m1<9)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian)){  //x0000x
                    if (b+6<15)
                    {
                        if (hov==0)
                        {
                            if (board[a][b+6]==cir||board[a][b+6]==trian) {  //x0000x0
                                (*dead4)++;
                            }
                            else {(*live4)++;}
                        }
                        else
                        {
                            if (board[b+6][a]==cir||board[b+6][a]==trian) {
                                (*dead4)++;
                            }
                            else {(*live4)++;}
                        }
                    }
                    else {(*live4)++;}
                    b+=3;
                    continue;
                }
            }
            int k;
            m1m7(m1, m2, m3, m4, m5, m6, m7, player,live2, dead3, live3, dead4, live4,win,&b,&k);
            if ((*win)!=0) {
                return;
            }
        }
    }
}

//对角扫描
//有一部分重复代码，可以抽一个独立函数出来
void diascore(int lor,int player,int (*board)[15],int *live2,int *dead3,int *live3,int *dead4,int *live4,int *win)
{
    int a,b;
    int m1,m2,m3,m4,m5,m6,m7;
    int cir,trian,wall;
    if(player==0) {cir=9;trian=11;wall=10;}
    else {cir=10;trian=12;wall=9;}
    
    for (a=10;a>=0;a-- ){
        int i=a;
        for (b=0;i+4<=14; b++,i++){   
            if (lor==0){
                if (board[b][i]<9&&board[b+1][i+1]<9&&board[b+2][i+2]<9) {continue;}
                if (player==0&&b>0&&i>0&&(board[b-1][i-1]==cir||board[b-1][i-1]==trian)) {
                    continue;
                }
                if (b==0||i==0) m6=wall;
                else m6=board[b-1][i-1];
                if(i+5<15) m7=board[b+5][i+5];
                else m7=wall;
                if (player==0&&b>0&&(m7==cir||m7==trian)) {
                    continue;
                }
            }
            else  //lor==1
            {
                if (board[14-b][i]<9&&board[14-b-1][i+1]<9&&board[14-b-2][i+2]<9) continue;
                if (player==0&&b>0&&a>0&&(board[14-b+1][i-1]==cir||board[14-b+1][i-1]==trian)) {
                    continue;
                }
                if (b==0||i==0) m6=wall;
                else m6=board[14-b+1][i-1];
                
                if(i+5<15) m7=board[14-(b+5)][i+5];
                
                else m7=wall;
                if (player==0&&b>0&&(m7==cir||m7==trian)) {
                    continue;
                }
            }
            if (lor==0){
                m1=board[b][i];
                m2=board[b+1][i+1];
                m3=board[b+2][i+2];
                m4=board[b+3][i+3];
                m5=board[b+4][i+4];
            }
            else{
                m1=board[14-b][i];
                m2=board[14-(b+1)][i+1];
                m3=board[14-(b+2)][i+2];
                m4=board[14-(b+3)][i+3];
                m5=board[14-(b+4)][i+4];
            }
            if(m1==wall||m2==wall||m3==wall||m4==wall||m5==wall) continue;
            if (player==0) {
                if(m7<9&&(m1<9)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian)){  //x0000x
                    if (i+6<15)
                    {
                        if (lor==0)
                        {
                            if (board[b+6][i+6]==cir||board[b+6][i+6]==trian) {
                                (*dead4)++;
                            }
                            else {(*live4)++;}
                        }
                        else
                        {
                            if(board[14-(b+6)][i+6]==cir||board[14-(b+6)][i+6]==trian) {
                                (*dead4)++;
                            }
                            else {(*live4)++;}
                        }
                    }
                    else {(*live4)++;}
                    b+=4;i+=4;
                    continue;
                }
            }
            m1m7(m1, m2, m3, m4, m5, m6, m7, player,live2, dead3, live3, dead4, live4,win,&b,&i);
            if ((*win)!=0) {
                return;
            }
        }
    }
    if (a<0){
        int c,i;
        for (c=1;c<=10; c++){
            for (b=c,i=0;b+4<=14;b++,i++){
                if(lor==0){
                    if (board[b][i]<9&&board[b+1][i+1]<9&&board[b+2][i+2]<9) continue;
                    if(player==0&&b>0&&i>0&&(board[b-1][i-1]==cir||board[b-1][i-1]==trian))
                        continue;
                    else if(i==0) m6=wall;
                    else m6=board[b-1][i-1];
                }
                else {
                    if (board[14-b][i]<9&&board[14-b-1][i+1]<9&&board[14-b-2][i+2]<9) continue;
                    if(player==0&&b>0&&i>0&&(board[14-b+1][i-1]==cir||board[14-b+1][i-1]==trian))
                        continue;
                    else if(i==0) m6=wall;
                    else m6=board[14-b+1][i-1];
                }
                if (lor==0){
                    
                    m1=board[b][i];
                    m2=board[b+1][i+1];
                    m3=board[b+2][i+2];
                    m4=board[b+3][i+3];
                    m5=board[b+4][i+4];
                }
                else{
                    
                    m1=board[14-b][i];
                    m2=board[14-(b+1)][i+1];
                    m3=board[14-(b+2)][i+2];
                    m4=board[14-(b+3)][i+3];
                    m5=board[14-(b+4)][i+4];
                }
                if(m1==wall||m2==wall||m3==wall||m4==wall||m5==wall) continue;
                if (b+5<=14){
                    if (lor==0)
                    {
                        m7=board[b+5][i+5];
                    }
                    else m7=board[14-(b+5)][i+5];
                }
                else {m7=wall;}
                if (player==0&&b>0&&(m7==cir||m7==trian)) {
                    continue;
                }
                if (player==0) {
                    if(m7<9&&(m1<9)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian)){  //x0000x
                        if (b+6<15){
                            if (lor==0){
                                if (board[b+6][i+6]==cir||board[b+6][i+6]==trian) {
                                    (*dead4)++;
                                }
                                else {(*live4)++;}
                            }
                            else{
                                if(board[14-(b+6)][i+6]==cir||board[14-(b+6)][i+6]==trian) {
                                    (*dead4)++;
                                }
                                else {(*live4)++;}
                            }
                        }
                        else {(*live4)++;}
                        b+=4;i+=4;
                        continue;
                    }
                }
                m1m7(m1, m2, m3, m4, m5, m6, m7, player,live2, dead3, live3, dead4,live4, win,&b,&i);
                if ((*win)!=0) {
                    return;
                }
            }
        }
    }
}


//5枚连续棋子的棋型分析
void m1m7(int m1,int m2,int m3,int m4,int m5,int m6,int m7,int player,int *live2,int *dead3,int *live3,int *dead4,int *live4,int *win,int *b,int *a)
{
    int cir,trian,wall;
    if(player==0) {cir=9;trian=11;wall=10;}
    else {cir=10;trian=12;wall=9;}
    
    //00000  五
    if ((m1==cir||m1==trian)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian)) {
        *win=7;
        return;
    }
    //x0000  活四、死四
    else if((m1<9)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian))
    {
        if (m7<9) {   //x0000x
            (*live4)++;
            (*b)+=4;(*a)+=4;
        }
        else if (m7==wall) {  //x0000|
            (*dead4)++;
            (*b)+=4;(*a)+=4;
        }
        else if(m7==cir||m7==trian){*win=7;return;}
    }
    
    else if ((m1==cir||m1==trian)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5<9))
    {//0000x
        if (m6<9) {   //x0000x
            (*live4)++;
            (*b)+=4;(*a)+=4;
        }
        else if (m6==wall) {  //|0000x
            (*dead4)++;
            (*b)+=4;(*a)+=4;
        }
    }
    //死4
    else if ((m1==cir||m1==trian)&&(m2==cir||m2==trian)&&(m3<9)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian)) {//00x00
        (*dead4)++;
        (*b)+=1;(*a)+=1;
    }
    else if ((m1==cir||m1==trian)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4<9)&&(m5==cir||m5==trian)) {//000x0
        (*dead4)++;
        (*b)+=2;(*a)+=2;
    }
    else if ((m1==cir||m1==trian)&&(m4==cir||m4==trian)&&(m2<9)&&(m3==cir||m3==trian)&&(m5==cir||m5==trian)) {//0x000
        (*dead4)++;
    }
    //活3
    else if ((m1<9)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4<9)&&(m5==cir||m5==trian))
    {//x00x0
        if(m7<9)       //x00x0x
        {
            (*live3)++;
            (*b)+=3;(*a)+=3;
        }
        else if(m7==wall)         //x00x0|
        {
            (*dead3)++;
            (*b)+=5;(*a)+=5;
        }
    }
    else if ((m1<9)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5<9))
    {//x000x
        if(m7<9||m6<9) {(*live3)++;(*b)+=4;(*a)+=5;}
        else {(*dead3)++;(*b)+=5;(*a)+=5;}
    }
    else if ((m1<9)&&(m2==cir||m2==trian)&&(m3<9)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian))
    {   //x0x00
        if(m7<9)       //x0x00x
        {
            (*live3)++;
            (*b)+=2;(*a)+=2;
        }
        else if(m7==wall)           //x0x00|
        {
            (*dead3)++;
            (*b)+=5;(*a)+=5;
        }
    }
    //死3
    else if ((m1==cir||m1==trian)&&(m2<9)&&(m3<9)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian)) {//0xx00
        (*dead3)++;
    }
    else if ((m1==cir||m1==trian)&&(m2==cir||m2==trian)&&(m3<9)&&(m4<9)&&(m5==cir||m5==trian)) {//00xx0
        (*dead3)++;
    }
    else if ((m1==cir||m1==trian)&&(m2<9)&&(m3==cir||m3==trian)&&(m4<9)&&(m5==cir||m5==trian)) {//0x0x0
        (*dead3)++;
    }
    else if ((m6==10||m6==12)&&(m2==cir||m2==trian)&&(m3==cir||m3==trian)&&(m5<9)&&(m1==cir||m1==trian)&&(m4<9)) {//|000xx
        (*dead3)++;
    }
    else if ((m1<9)&&(m2<9)&&(m3==cir||m3==trian)&&(m4==cir||m4==trian)&&(m5==cir||m5==trian))
    {//xx000
        if(m7<9){
            (*live3)++;
            (*b)+=4;(*a)+=4;}
        else if(m7==wall){(*dead3)++;(*b)+=5;(*a)+=5;}
    }
    //活2
    else if ((m1<9)&&(m3<9)&&(m2==cir||m4==trian)&&(m5<9)&&(m4==cir||m4==trian))
    {//xx0x0
        if (m7<9)//xx0x0x
        {
            (*live2)++;
            (*b)+=2;(*a)+=2;
        }
    }
    else if ((m1<9)&&(m4<9)&&(m3==cir||m3==trian)&&(m5<9)&&(m2==cir||m2==trian))
    {//x00xx
        if(m6<9) {(*live2)++;(*b)+=2;(*a)+=2;}  //xx00xx
    }
    else if ((m1<9)&&(m2<9)&&(m3==cir||m3==trian)&&(m5<9)&&(m4==cir||m4==trian))
    {//xx00x
        if(m6<9&&m7>=9&&m7!=cir&&m7!=trian) {(*live2)++;(*b)+=5;(*a)+=5;}
    }
    else if ((m1<9)&&(m3<9)&&(m2==cir||m2==trian)&&(m4<9)&&(m5==cir||m5==trian))
    {//x0xx0
        
        if (m7<9) //x0xx0x
        {
            (*live2)++;
            (*b)+=2;(*a)+=2;
        }
        else if(m7!=cir&&m7!=trian) {(*b)+=5;(*a)+=5;}
    }
    
    else if ((m1<9)&&(m3<9)&&(m2==cir||m2==trian)&&(m5<9)&&(m4==cir||m4==trian))
    {//x0x0x
        if(m7<9) {(*live2)++;(*b)+=2;(*a)+=2;}
    }
    else return;
}

