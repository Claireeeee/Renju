#include "defs.h"

//禁手检查： 以A、B为中心，检查四个方向的棋型，三三、四四或长连时返回1
int forbi(int A,int B,int (*board)[15])  
{
//如果A、B周围两个子范围内无黑子，认为A，B处不会形成禁手
    int c,d0,d,mm;
    c=(A-2>=0)?A-2:0;
    d0=(B-2>=0)?B-2:0;
    mm=0;
    for (;c<=A+2&&c<15;c++) {
        for (d=d0;d<=B+2&&d<15;d++) {
            if (board[c][d]==9||board[c][d]==11) {
                mm=1;
            }
        }
    }
    if (mm==0) return 0;
//确定搜索范围：A、B为中心，五子为边长的矩形，abegin,aend,bbegin,bend为行列端点
    int m=board[A][B];
    board[A][B]=9;
    int abegin,aend,bbegin,bend;
    int m1,m2,m3,m4,m5,m6,m7;
    int live3=0;
    int dol4=0;
    for (abegin=A;abegin>=0&&abegin>A-5;abegin--) {        
    }
    if (abegin<0) abegin+=1;
    for (aend=A; aend<=14&&aend<A+5;aend++) {
    }
    if (aend>14) aend-=1;
    for (bbegin=B;bbegin>=0&&bbegin>B-5;bbegin--) {
    }
    if (bbegin<0) bbegin+=1;
    
    for (bend=B; bend<=14&&bend<B+5;bend++) {
    }
    if (bend>14) bend-=1;
//范围内搜索棋型
    int a=A;
    int b=B;
    int i=0;
    for (b=bbegin; b+4<=bend; b++)          //遍历行
    {
        m1=board[a][b];
        m2=board[a][b+1];
        m3=board[a][b+2];
        m4=board[a][b+3];
        m5=board[a][b+4];
        if (b-1>=0)
            m6=board[a][b-1];
        else m6=10;
        if (b+5<=14) {
            m7=board[a][b+5];
        }
        else m7=10;
        int k;
        i=m1m5(m1,m2,m3,m4,m5,m6,m7,&live3,&dol4,&b,&k);

        if (i==1||live3>1||dol4>1) {        //出现三三、四四或长连时返回1
            board[A][B]=m;
            return 1;
        }
    }
    for (a=abegin,b=B; a+4<=aend; a++)        //遍历列
    {
        m1=board[a][b];
        m2=board[a+1][b];
        m3=board[a+2][b];
        m4=board[a+3][b];
        m5=board[a+4][b];
        if (a-1>=0)
            m6=board[a-1][b];
        else m6=10;
        if (a+5<=14) {
            m7=board[a+5][b];
        }
        else m7=10;
        int k=0;
        i=m1m5(m1,m2,m3,m4,m5,m6,m7,&live3,&dol4,&a,&k);
        
        if (i==1||live3>1||dol4>1) {
            board[A][B]=m;
            return 1;
        }
    }

    int lfbegin,lfend,rtbegin,rtend;
    lfbegin=(A-abegin<=bend-B)?A-abegin:bend-B;
    lfend=(aend-A<=B-bbegin)?aend-A:bend-B;
    rtbegin=(A-abegin<=B-bbegin)?A-abegin:B-bbegin;
    rtend=(aend-A<=bend-B)?aend-A:bend-B;
    
    for (a=A-lfbegin,b=B+lfbegin; b>=B-lfend+4&&a+4<=A+lfend; b--,a++)          //遍历y=x对角线
    {
        m1=board[a][b];
        m2=board[a+1][b-1];
        m3=board[a+2][b-2];
        m4=board[a+3][b-3];
        m5=board[a+4][b-4];
        if (b+1<=14&&a-1>=0)
            m6=board[a-1][b+1];
        else m6=10;
        if (b-5>=0&&a+5<=14) {
            m7=board[a+5][b-5];
        }
        else m7=10;
        int k=0;
        i=m1m5(m1,m2,m3,m4,m5,m6,m7,&live3,&dol4,&a,&k);
        b=b-k;
        
        if (i==1||live3>1||dol4>1) {
            board[A][B]=m;
            return 1;
        }
    }
    
    for (a=A-rtbegin,b=B-rtbegin; a+4<=A+rtend&&b+4<=B+rtend; a++,b++)        //遍历y=-x对角线
    {
        m1=board[a][b];
        m2=board[a+1][b+1];
        m3=board[a+2][b+2];
        m4=board[a+3][b+3];
        m5=board[a+4][b+4];
        if (a-1>=0&&b-1>=0)
            m6=board[a-1][b-1];
        else m6=10;
        if (a+5<=14&&b+5<=14) {
            m7=board[a+5][b+5];
        }
        else m7=10;
        i=m1m5(m1,m2,m3,m4,m5,m6,m7,&live3,&dol4,&a,&b);
        
        if (i==1||live3>1||dol4>1) {
            board[A][B]=m;
            return 1;
        }
    }
    board[A][B]=m;
    return 0;
}


//函数m1m5: 检查m1到m7棋型的工具函数，更新活3四4的数量
//return:0 无长连，1：长连
int m1m5(int m1,int m2,int m3,int m4,int m5,int m6,int m7,int *live3,int *dol4,int *b,int *a)
{
    //长连
    if ((m1==9||m1==11)&&(m2==9||m2==11)&&(m3==9||m3==11)&&(m4==9||m4==11)&&(m5==9||m5==11)) {//00000
        if (m6==9||m6==11||m7==9||m7==11) {
            //printf("long \n");
            return 1;
        }
        else return 0;
    }
    //4
    if((m1<9)&&(m2==9||m2==11)&&(m3==9||m3==11)&&(m4==9||m4==11)&&(m5==9||m5==11)) //x0000
    {
        (*dol4)++;(*b)+=4;(*a)+=4;
    }
    if ((m1==9||m1==11)&&(m2==9||m2==11)&&(m3==9||m3==11)&&(m4==9||m4==11)&&(m5<9))
    {//0000x
        (*dol4)++;(*b)+=4;(*a)+=4;
    }
    //死4
    if ((m1==9||m1==11)&&(m2==9||m2==11)&&(m3<9)&&(m4==9||m4==11)&&(m5==9||m5==11)) {//00x00
        (*dol4)++;(*b)+=4;(*a)+=4;
    }
    if ((m1==9||m1==11)&&(m2==9||m2==11)&&(m3==9||m3==11)&&(m4<9)&&(m5==9||m5==11)) {//000x0
        (*dol4)++;(*b)+=4;(*a)+=4;
    }
    if ((m1==9||m1==11)&&(m4==9||m4==11)&&(m2<9)&&(m3==9||m3==11)&&(m5==9||m5==11)) {//0x000
        (*dol4)++;(*b)+=4;(*a)+=4;
    }
    //活3
    if ((m3<9)&&(m2==9||m2==11)&&(m1==9||m1==11)&&(m5<9)&&(m4==9||m4==11))
    {//00x0x
        if(m6<9)       //x00x0x
        {
            (*live3)++;(*b)+=4;(*a)+=4;
        }
    }
    if ((m5<9)&&(m2==9||m2==11)&&(m3==9||m3==11)&&(m4==9||m4==11)&&(m1<9))
    {//x000x
        if (m6<9||m7<9) {
            (*live3)++;(*b)+=4;}(*a)+=4;
    }
    
    if ((m5<9)&&(m2==9||m2==11)&&(m3==9||m3==11)&&(m1==9||m1==11)&&(m4<9))
    {//000xx
        if (m6<9) {
            (*live3)++;(*b)+=4;}(*a)+=4;
    }
    if ((m5<9)&&(m3==9||m3==11)&&(m2<9)&&(m4==9||m4==11)&&(m1==9||m1==11))
    {   //0x00x
        if(m6<9)       //x0x00x
        {
            (*live3)++;(*b)+=4;(*a)+=4;
        }
    }
    
    return 0;
}



