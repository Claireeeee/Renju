#include "defs.h"

struct stateinfo kids[170000];
int kidindex=0;
int ran=0;


//两次博弈，构建一个4层树，每层数量为LINE（15秒内LINE可取13左右）
int nextstep(int player,int (*board)[15],int *nestp,int *opstp){
    
    if (ran==0) {                    //最早一步下花月浦月定式
        ran=1;
        int a=opstp[0]; int b=opstp[1];
        if (player==0) {
            if (abs(a-7)>1||abs(b-7)>1) {
                a=6;b=6;
                int sboardvalue=board[a][b];
                board[a][b]=9;
                int minscore=statescore(0, board);
                board[a][b]=sboardvalue;
                nestp[0]=a;nestp[1]=b;
                for (;a<8;a++) {
                    for (b=6;b<8;b++) {
                        sboardvalue=board[a][b];
                        board[a][b]=9;
                        if ((a!=7||b!=7)&&statescore(0,board)>minscore) {
                            nestp[0]=a;nestp[1]=b;
                        }
                        board[a][b]=sboardvalue;
                    }
                }
            }
            else if(abs(a-7)+abs(b-7)>1){
                nestp[0]=a;nestp[1]=14-b;
            }
            else{
                if (a==7) {nestp[0]=8;nestp[1]=b;}
                else {nestp[0]=a;nestp[1]=6;}
            }
        }
        else {nestp[0]=6;nestp[1]=7;}
    }
    else{
        kids[kidindex].kidsnum=0;
        kids[kidindex].must=0;
        kids[kidindex].layer=0;
        struct stateinfo * step0=&kids[kidindex++];
        int a=nodemax10(board,player,step0,opstp,player);    //构建树
        printf("node10max\n");
        if (a==1) {                                          //a=1代表首步即可下出必胜局，可直接返回
            nestp[0]=(*step0).ab[0][0];
            nestp[1]=(*step0).ab[0][1];
            return 0;
        }
        else{
            int i=nodevalue(board,step0,player);      //向上传值
            printf("nodevalue\n");
            step0=(*step0).kid[i];
            nestp[0]=(*step0).ab[0][0];
            nestp[1]=(*step0).ab[0][1];
        }
    }
    return 0;
}

//nodemax10函数负责树的构建：从opstp记录的对方当前步开始，蛇形遍历，记录LINE个最高分，递归分析，到第四层
int nodemax10(int (*board)[15],int player,struct stateinfo* step0,int *opstp,int oriplayer){
    int minscore=-1;
    int minkidindex=0;
    int skidindex=0;
    int layer=(*step0).layer;
    int opmust[12]={-1,-1,5,0,-1,-1,0,0,0,-1,-1,5}; 
    
    int boardvalue[4];
    layerfill(step0,boardvalue,board,oriplayer,1);
    if (layer>0) {
        opstp[0]=(*step0).ab[layer-1][0];
        opstp[1]=(*step0).ab[layer-1][1];              //对方当前落子
    }
    int forbivalue[10];
    int forbiab[10][2];
    if(player==0) forbifill(forbivalue, forbiab,board,1);   //禁手处填为白子

    int a=opstp[0];int b=opstp[1];                    //确定蛇形遍历最大边长
    int Ls[4]={14-a,a,14-b,b};
    int L=Ls[0];
    for (int i=1;i<4;i++) {
        if (Ls[i]>L) {
            L=Ls[i];
        }
    }
    for(int l=1,dire=0;l<=2*L;l++,dire=!dire){     //蛇形遍历
        for (int i=0;i<l;i++) {
            if (dire==0) {
                if (a+1<15&&a+1<opstp[0]+(double)l/2+1) {
                    a=a+1;
                }
                else break;
            }
            else {
                if (a-1>=0&&a-1>opstp[0]-(double)l/2-1) {
                    a=a-1;
                }
                else break;
            }
            if (board[a][b]>=9){
                continue;
            }
            else{
    //kidselect函数负责分析每个子的状况，包括：己方必胜、对方必胜、禁手，破除禁手、分数等的判断
                int a1=kidslect(board,player,step0,a,b,&minscore,&minkidindex,&skidindex,opmust,forbivalue,forbiab);
                if (a1==1||a1==-1) {   //己方胜,或对方胜，直接落子
                    layerfill(step0,boardvalue,board,oriplayer,0);
                    if(player==0) forbifill(forbivalue, forbiab,board,0);
                    if (layer==0) {
                        (*step0).ab[0][0]=a;
                        (*step0).ab[0][1]=b;
                    }
                    if (a1==1||layer==0) {
                        return 1;
                    }
                    else if(layer<3){
                        nodemax10(board,!player,(*step0).kid[0],opstp,oriplayer);
                    }
                    return a1;;
                }
            }
        }
        for (int i=0;i<l;i++) {
            if (dire==0) {
                if (b+1<15&&b+1<opstp[1]+(double)l/2+1) {
                    b=b+1;
                }
                else break;
            }
            else {
                if (b-1>=0&&b-1>opstp[1]-(double)l/2-1) {
                    b=b-1;
                }
                else break;
            }
            if (board[a][b]>=9) continue;
            else {
                int a1=kidslect(board,player,step0,a,b,&minscore,&minkidindex,&skidindex,opmust,forbivalue,forbiab);
                if (a1==1||a1==-1) {
                    layerfill(step0,boardvalue,board,oriplayer,0);
                    if(player==0) forbifill(forbivalue, forbiab,board,0);
                    if (layer==0) {
                        (*step0).ab[0][0]=a;
                        (*step0).ab[0][1]=b;
                    }
                    if (a1==1||layer==0) {
                        return 1;
                    }
                    else if(layer<3){
                        nodemax10(board,!player,(*step0).kid[0],opstp,oriplayer);
                    }
                    return a1;;
                }
            }
        }
    }
    layerfill(step0,boardvalue,board,oriplayer,0);
    if(player==0) forbifill(forbivalue, forbiab,board,0);
    if (opmust[9]!=-1) {               //遍历过程记录的己方必胜步
        int c=0;int d=0;
        for (;c<layer;c++) {         
            for (d=0;d<2;d++) {
                kids[kidindex].ab[c][d]=(*step0).ab[c][d];
            }
        }
        int fscore=1000000;
        kidinfobuild(layer,opmust[9],opmust[10],fscore);
        
        (*step0).kid[0]=&kids[kidindex++];
        (*step0).must=1;
        (*step0).score=fscore;
        if (layer==0) {
            (*step0).ab[0][0]=opmust[9];
            (*step0).ab[0][1]=opmust[10];
        }
        return 1;
    }
    else if (opmust[0]!=-1||opmust[4]!=-1) {   //对方必胜步或破禁手步
        int c=0;int d=0;
        for (;c<layer;c++) {      
            for (d=0;d<2;d++) {
                kids[kidindex].ab[c][d]=(*step0).ab[c][d];
            }
        }
        if (opmust[4]!=-1&&opmust[7]==0) {
            c=opmust[4];
            d=opmust[5];
        }
        else{
            c=opmust[0];
            d=opmust[1];
        }
        kidinfobuild(layer,c,d,6);
        (*step0).kid[0]=&kids[kidindex++];
        (*step0).must=1;
        (*step0).score=6;
        if (layer==0) {
            (*step0).ab[0][0]=c;
            (*step0).ab[0][1]=d;
            return 1;
        }
        else if(layer<3){
            nodemax10(board,!player,(*step0).kid[0],opstp,oriplayer);    //递归，切换player，分析下一步
        }
        return 2;
    }
    else{
        (*step0).kidsnum=skidindex;
        if (layer<3) {
            for (int i=0;i<skidindex;i++) {
                nodemax10(board,!player,(*step0).kid[i],opstp,oriplayer);
            }
        }
        return 0;
    }
}


//构建树时用到的中间函数。用来填充棋盘到当前分析步
int layerfill(struct stateinfo* step0,int boardvalue[4],int (*board)[15],int oriplayer,int yrn){
    
    int layer=(*step0).layer;
    if (yrn==1) {
        for (int i=0,m=0; i<layer; i++,m=!m){
            int a=0;int b=0;
            a=(*step0).ab[i][0];        
            b=(*step0).ab[i][1];
            boardvalue[i]=board[a][b];
            if (m==0) {                             
                board[a][b]=(oriplayer==0)?9:10;
            }
            else{
                board[a][b]=(oriplayer==0)?10:9;
            }
        }
    }
    else{
        for (int i=0; i<layer; i++) {        
            int a=0;int b=0;
            a=(*step0).ab[i][0];
            b=(*step0).ab[i][1];
            board[a][b]=boardvalue[i];
        }
    }
    return 0;
    
}

//build a stateinfo for selected nextstep
int kidinfobuild(int layer,int a,int b,int score){
    
    kids[(kidindex)].ab[layer][0]=a;       
    kids[(kidindex)].ab[layer][1]=b;
    kids[(kidindex)].score=score;
    kids[(kidindex)].must=0;
    kids[(kidindex)].layer=layer+1;
    kids[(kidindex)].kidsnum=0;
    kids[(kidindex)].kid[0]=NULL;
    return 0;
}



//构建树时用到的分析每个落子的函数
//这个里面也有一部分重复代码
int kidslect(int (*board)[15],int player,struct stateinfo* step0,int a,int b,int *minscore,int *minkidindex,int *skidindex,int opmust[6],int *forbivalue,int (*forbiab)[2]){
    int layer=(*step0).layer;
    int sboardvalue=board[a][b];
    board[a][b]=(player==0)?9:10;
    int mscore,opscore0,opscore1;

    //如果有禁手（之前的禁手都落上了白子），先移除判断
    if (player==0&&forbivalue[0]!=-1&&(a!=forbiab[0][0]||b!=forbiab[0][1])) {
        forbifill(forbivalue, forbiab,board,0);
        if (forbi(a,b,board)==0) {
            mscore=statescore(player,board);   
            opscore0=statescore(!player,board);
            board[a][b]=(player==0)?10:9;
            opscore1=statescore(!player,board);
            board[a][b]=(player==0)?9:10;

            //己方必胜局：成5返回1，其他情况记录优先级后返回2（返回2时外层会继续遍历，持续更新优先级）
            if(mscore%10==7||((opscore1%10!=7)&&(((mscore%10==1||mscore%10==2||mscore%10==3)&&opscore0%10!=5&&(opscore0%10==0||(opscore0%10!=0&&opscore0%10>3)))||(mscore%10==4&&opscore0<2000&&(opscore0%10==0||(opscore0%10!=0&&opscore0%10>4)))))){                 //must
                if (mscore%10==7) {
                    int c=0;int d=0;
                    for (;c<layer;c++) {           
                        for (d=0;d<2;d++) {
                            kids[kidindex].ab[c][d]=(*step0).ab[c][d];
                        }
                    }
                    kidinfobuild(layer,a,b,1000000);
                    (*step0).kid[0]=&kids[kidindex++];
                    (*step0).must=1;
                    (*step0).score=1000000;
                    board[a][b]=sboardvalue;
                    forbifill(forbivalue, forbiab,board,2);
                    return 1;
                }
                else if (mscore%10<opmust[11]%10) {
                    opmust[9]=a;
                    opmust[10]=b;
                    opmust[11]=mscore;
                    board[a][b]=sboardvalue;
                    forbifill(forbivalue, forbiab,board,2);
                    return 2;
                }
            }

            //对方必胜局：不需禁手也是必胜局
            if (opscore1%10!=0&&opscore1%10!=5) opmust[7]=1;
            //破禁手
            else if (forbi(forbiab[0][0],forbiab[0][1],board)==0) {
                if (mscore>opmust[6]) {  //都能破禁手时，取最高分
                    opmust[4]=a;opmust[5]=b;
                    opmust[6]=mscore;
                }
                board[a][b]=sboardvalue;
                forbifill(forbivalue, forbiab,board,2);
                return 2;
            }
        }
        else {
            board[a][b]=sboardvalue;
            forbifill(forbivalue, forbiab,board,2);
            return 0;
        }
        forbifill(forbivalue, forbiab,board,2);
    }

    //没有禁手或无法破除的情况下
    mscore=statescore(player,board);
    opscore0=statescore(!player,board);
    board[a][b]=(player==0)?10:9;
    opscore1=statescore(!player,board);     
    board[a][b]=sboardvalue;
    //己方必胜局
    if(mscore%10==7||((opscore1%10!=7)&&(((mscore%10==1||mscore%10==2||mscore%10==3)&&opscore0%10!=5&&(opscore0%10==0||(opscore0%10!=0&&opscore0%10>3)))||(mscore%10==4&&opscore0<2000&&(opscore0%10==0||(opscore0%10!=0&&opscore0%10>4)))))){                 //must
        if (mscore%10==7) {
            int c=0;int d=0;
            for (;c<layer;c++) {            
                for (d=0;d<2;d++) {
                    kids[kidindex].ab[c][d]=(*step0).ab[c][d];
                }
            }
            kidinfobuild(layer,a,b,1000000);
            
            (*step0).kid[0]=&kids[kidindex++];
            (*step0).must=1;
            (*step0).score=1000000;
            return 1;
        }
        else if (mscore%10<opmust[11]%10) {
            opmust[9]=a;
            opmust[10]=b;
            opmust[11]=mscore;
        }
        return 2;
    }
    //对方必胜局
    if (opscore1%10!=0&&opscore1%10!=5&&(opscore1%10==7||(opscore1%10<=opmust[2]%10))) {
        if (opscore1%10==7) {
            int c=0;int d=0;
            for (;c<layer;c++) {
                for (d=0;d<2;d++) {
                    kids[kidindex].ab[c][d]=(*step0).ab[c][d];
                }
            }
            kidinfobuild(layer,a,b,mscore);
            
            (*step0).kid[0]=&kids[kidindex++];
            (*step0).must=1;
            (*step0).score=mscore;
            return -1;
        }
        else if ((opscore1%10==opmust[2]%10&&mscore<=opmust[3]&&opscore1<=opmust[2])) ;
        else{
            opmust[0]=a;
            opmust[1]=b;
            opmust[2]=(opscore1%10==7)?0:opscore1;
            //opmust[3]=opscore1;
            opmust[3]=mscore;
        }
        return 2;
    }
    //形成（新）禁手
    if ((layer==0||layer==2)&&player==0) {
        board[a][b]=(player==0)?9:10;   
        int tforbivalue[10];
        int tforbiab[10][2];
        if (forbifill(tforbivalue,tforbiab,board,1)==1) {
            board[a][b]=sboardvalue;
            forbifill(tforbivalue,tforbiab,board,0);
            return 0;
        }
        board[a][b]=sboardvalue;
    }
    //以上特殊情况都没有：判断分数（与当前最小分数对比），决定是否放入该层的LINE个最高分中
    if(*skidindex<LINE||mscore>*minscore){
        if (*skidindex<LINE) {
            int c=0;int d=0;
            for (;c<layer;c++) {           
                for (d=0;d<2;d++) {
                    kids[kidindex].ab[c][d]=(*step0).ab[c][d];
                }
            }
            kidinfobuild(layer,a,b,mscore);
            (*step0).kid[*skidindex]=&kids[kidindex++];
            (*step0).kidscore[*skidindex]=mscore;
            (*skidindex)++;
            
        }
        else{                          //update minscore and minindex
            (*(*step0).kid[*minkidindex]).ab[layer][0]=a;
            (*(*step0).kid[*minkidindex]).ab[layer][1]=b;
            (*(*step0).kid[*minkidindex]).score=mscore;
            (*step0).kidscore[*minkidindex]=mscore;
        }

        //更新当前最小分数
        *minscore=(*step0).kidscore[0];
        *minkidindex=0;
        for (int i=1; i<*skidindex; i++) {
            if ((*step0).kidscore[i]<*minscore) {
                *minscore=(*step0).kidscore[i];
                *minkidindex=i;
            }
        }
        return 3;
    }
    else return 0;
}

//工具函数：将禁手处都填为白子
int forbifill(int forbivalue[],int (*forbiab)[2],int (*board) [15],int yrn){
    int m=0;
    if (yrn==1) {
        int i=0;
        forbivalue[0]=-1;
        for (int a=0,b=0;a<15;a++) {
            for (b=0;b<15;b++) {
                if (board[a][b]<9&&forbi(a,b,board)!=0) {
                    m=1;
                    forbiab[i][0]=a;
                    forbiab[i][1]=b;
                    forbivalue[i]=board[a][b];
                    board[a][b]=10;
                    i++;
                    forbivalue[i]=-1;
                }
            }
        }
    }
    else if(yrn==0){
        for (int i=0;forbivalue[i]!=-1;i++) {
            board[forbiab[i][0]][forbiab[i][1]]=forbivalue[i];
        }
    }
    else{
        for (int i=0;forbivalue[i]!=-1;i++) {
            board[forbiab[i][0]][forbiab[i][1]]=10;
        }
    }
    return m;
}


//nodevalue函数负责向上传值，返回4步后最高分对应的第一步
int nodevalue(int (*board)[15],struct stateinfo *step0,int oriplayer){
    int must=(*step0).must;
    int layer=(*step0).layer;
    int maxkid=0;
    
    if (layer==3) {
        if (must==1) {                
            maxkid=0;
            if ((*step0).score==1000000) {
                (*step0).score=-1000000;
                return 0;
            }
        }
        else maxkid=minmaxkid(1,step0);
        
        int boardvalue[4];
        layerfill((*step0).kid[maxkid],boardvalue,board,oriplayer,1);
        int mscore=statescore(oriplayer,board);
        layerfill((*step0).kid[maxkid],boardvalue,board,oriplayer,0);
        
        //4步后己方分数上传
        if (mscore%10!=0&&mscore%10!=5) {
            (*step0).score=1000000; 
        }
        else (*step0).score=mscore;
        return maxkid;
    }
    else if (layer<3) {
        if (must==1) {
            maxkid=0;
            if((*step0).score==1000000){
                if (layer==1||layer==3) {
                    (*step0).score=-1000000;
                }
                return 0;
            }
            else nodevalue(board, (*step0).kid[0],oriplayer);   //递归往下走
        }
        else{
            for (int i=0;i<(*step0).kidsnum;i++) {
                nodevalue(board, (*step0).kid[i],oriplayer);
            }
            if (layer==1) {
                maxkid=minmaxkid(0,step0);
            }
            else maxkid=minmaxkid(1,step0);
            if(layer==0){
                struct stateinfo* kida=(*step0).kid[maxkid];
                int maxscore=(*kida).score;
                int maxkidinsex=maxkid;
                int maxs[LINE];int k=0;
                
                //并列最高分的第一步，放入数组maxs中
                for (int i=0; i<(*step0).kidsnum;i++) {
                    kida=(*step0).kid[i];
                    if ((*kida).score==maxscore) {
                        maxs[k++]=i;
                    }
                }
                if (k>1) {
                    kida=(*step0).kid[maxs[0]];
                    int a=(*kida).ab[0][0];
                    int b=(*kida).ab[0][1];
                    int sboardvalue=board[a][b];
                    board[a][b]=(oriplayer==0)?9:10;
                    maxscore=statescore(!oriplayer,board);  //maxscore此处用来记录对方最小分
                    board[a][b]=sboardvalue;
                    maxkidinsex=maxs[0];
                    //取数组中使对方分最小的下法
                    for (int i=1;i<k;i++) {
                        kida=(*step0).kid[maxs[i]];
                        a=(*kida).ab[0][0];
                        b=(*kida).ab[0][1];
                        sboardvalue=board[a][b];
                        board[a][b]=(oriplayer==0)?9:10;
                        int mopscore=statescore(!oriplayer,board);
                        board[a][b]=sboardvalue;
                        if (mopscore<maxscore) {
                            maxscore=mopscore;
                            maxkidinsex=maxs[i];
                        }
                        maxkid=maxkidinsex;
                    }
                }
            }
        }
        (*step0).score=(*(*step0).kid[maxkid]).score;
    }
    return maxkid;
}


//工具函数：遍历某node的kid，找一个最大或最小分，返回index
int minmaxkid(int max_ornot,struct stateinfo *step0){
    struct stateinfo* kida=(*step0).kid[0];
    int maxscore=(*kida).score;             //先找最大的，即对方最优
    int maxkidinsex=0;
    if (max_ornot==1) {
        for (int i=1; i<(*step0).kidsnum; i++) {
            kida=(*step0).kid[i];
            if ((*kida).score>maxscore) {
                maxscore=(*kida).score;
                maxkidinsex=i;
            }
        }
    }
    else{ 
        for (int i=1; i<(*step0).kidsnum; i++) {
            kida=(*step0).kid[i];
            if ((*kida).score<maxscore) {                    //此时max代表min
                maxscore=(*kida).score;
                maxkidinsex=i;
            }
        }
    }
    return maxkidinsex;
}



