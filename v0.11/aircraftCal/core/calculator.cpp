#include "calculator.h"

#include <iostream>
#include <windows.h>

using namespace std;

/*
1.̰�İڷ�ս������������
2.01ö�ٷ�վ���
3.    ���ݷ�վ01ö��ս����/��������װ��(��֦��
4.���ؽ��
*/

int actual_avail[10],carrierNum;;

void cal_dataReady(string msg){//��query������Ҫ����
    stringstream st;
    st.str(msg);

    //read the query airDominance
    st>>airDomin;

    //read carrier
    st>>carrierNum;
    string tmpName;
    int tmpAvailGrid,tmpAvailBit;
    for(int i=0;i<carrierNum;i++){
        st>>tmpName>>tmpAvailBit;
        int tmpCarrierID=carriers_m[tmpName];
        int tmpCarrierGridNum=carriers[tmpCarrierID].getGridNum();
        int bit=1;
        //insert grid
        actual_avail[i]=0;
        for(int j=0;j<tmpCarrierGridNum;j++){
            if(tmpAvailBit&bit){
                gridVec.push_back(Grid(i,j,carriers[tmpCarrierID].getGridSize(j),carriers[tmpCarrierID].getAtk()));
                actual_avail[i]++;
            }
            bit<<=1;
        }
    }

    //read plane
    int planeTypeNum;
    st>>planeTypeNum;
    for(int i=0;i<planeTypeNum;i++){
        st>>tmpName;
        int tmpPlaneID=planes_m[tmpName];
        int tmpNum;
        st>>tmpNum;
        for(int i=0;i<tmpNum;i++)
            planeVec.push_back(planes[tmpPlaneID]);
    }
}

//���涼�Ǹ�sort�õ�������
inline bool cal_cmp_grid(const Grid &a,const Grid &b){
    return a.gridSize>b.gridSize;
}

inline bool cal_cmp_plane(Plane a,Plane b){
    return a.getAirDominance()>b.getAirDominance();
}

inline bool cal_cmp_plane_damage(Plane a,Plane b){
    return formulaDamage(a)>formulaDamage(b);
}

inline bool cal_cmp_plane_damageOP(Plane a,Plane b){
    return formulaDamageOP(1,a)>formulaDamageOP(1,b);
}

//���涼�Ǹ��ֻ�����ʽ
inline double formulaDamage(Plane &item){//�ڻ��˺�
    double res=item.getBombAtk()*2+item.getTorpedoAtk()*1.5;
    return res;
}

inline double formulaDamageOP(int gridSize,Plane &item){//��Ļ�˺�
    double res=0;
    int cate=item.getCategoryID();
    if(cate==PLANE_TORPEDO_ATTACKER)
        res=(item.getTorpedoAtk()*sqrt(gridSize)+25)*1.15;
    else if(cate==PLANE_BOMBER)
        res=item.getBombAtk()*sqrt(gridSize)+25;
    return res;
}

inline double formulaFighter(int gridSize,int airDominance){//�ƿ�����ʽ
    return sqrt(gridSize)*airDominance;
}

//���濪ʼ����������
int fsign[32],asign[32],ResA;//��¼���ѵ�״̬

//����ĳһ��״̬���ƿ�ֵ
inline double calDomi(int gridS){
    double res=0;
    int pi=0;
    for(int i=0;i<gridS;i++)
        if(fsign[i]==1){
            res+=formulaFighter(gridVec[i].gridSize,planeVecF[pi].getAirDominance());
            pi++;
        }
    return res;
}

//����ĳһ��״̬�Ŀ�Ļ����
inline double calAtk(int gridS){
    double res=0;
    int pi=0;
    for(int i=0;i<gridS;i++)
        if(asign[i]&1){
            res+=formulaDamageOP(gridVec[i].gridSize,planeVecA[pi]);
            pi++;
        }
    return res;
}

inline bool checkA(int curlv,int remainA,int gridS){//���ݿ�Ļ������֦
    if(remainA<0)return false;//not enough
    for(int i=0;i<remainA;i++)//�������ŵĹ�����װ��
        asign[curlv+i]=1;

    bool answer=(calAtk(gridS)>ResA);//�жϿ�Ļ������

    for(int i=0;i<remainA;i++)//�ָ�
        asign[curlv+i]=0;

    return answer;
}

inline void copystatus(char *answer,int gridS){//��������״̬���ַ���
    for(int i=0;i<gridS;i++){
        answer[i]=fsign[i]+'0'+asign[i]*2;
    }
    answer[gridS]='\0';
}

int most_avail_fighter[10],current_fighter[10];//ĳ�����õ�ս������Ŀ������ս������Ŀ���ⷣվ
inline bool is_penalty_cause_fighter_unplaceable(int test_grid){//�ж��Ƿ�վ��
    if(most_avail_fighter[gridVec[test_grid].carrierPos]<=current_fighter[gridVec[test_grid].carrierPos])
        return true;
    else return false;
}

stack<int>tem_placed_fighter_stack;
inline bool checkF(int curlv,int remainF,int gridS){//�����ƿռ�֦����ʵд���ܱ������������Խ�һ���Ż�(�ü��仯?)����ʵ��û�����Ҫ
    if(remainF<0)return false;//not enough

    int pos=-1;
    for(int i=0;i<remainF;i++){//�������ŵ�ս����װ��
        pos++;
        if(pos>=gridS)break;
        if(is_penalty_cause_fighter_unplaceable(curlv+pos))continue;
        fsign[curlv+pos]=1;
        tem_placed_fighter_stack.push(curlv+pos);
        current_fighter[gridVec[curlv+pos].carrierPos]++;
    }

    bool answer=(calDomi(gridS)>=airDomin);//�ж��ƿ�ֵ

    while(!tem_placed_fighter_stack.empty()){
        fsign[tem_placed_fighter_stack.top()]=0;
        current_fighter[gridVec[tem_placed_fighter_stack.top()].carrierPos]--;
        tem_placed_fighter_stack.pop();
    }
    return answer;
}

int thatair;//��������˺�ʱ���ƿ���ֵ
char answer[32];//��������˺�ʱ��01״̬
void baoliSearch(int curlv,int remainF,int remainA,int gridS){
    if(curlv==gridS||(remainF==0&&remainA==0)){//�ݹ����
        if(calDomi(gridS)>=airDomin){
            ResA=max((int)calAtk(gridS),ResA);
            thatair=calDomi(gridS);
            copystatus(answer,gridS);
        }
        return;
    }
    if(remainF&&!is_penalty_cause_fighter_unplaceable(curlv)){//���Է���ս��������ⷣվ
        current_fighter[gridVec[curlv].carrierPos]++;
        fsign[curlv]=1;
        //if(checkA(curlv+1,remainA,gridS))//�ƺ����Ǻ���Ч��Ч�������֦�����ʱ�临�Ӷȼӳ�
        baoliSearch(curlv+1,remainF-1,remainA,gridS);
        fsign[curlv]=0;
        current_fighter[gridVec[curlv].carrierPos]--;
    }
    if(remainA){//���Է��ù�����
        asign[curlv]=1;
        if(checkF(curlv+1,remainF,gridS))//��֦�����Լ�1e3��������
            baoliSearch(curlv+1,remainF,remainA-1,gridS);
        asign[curlv]=0;
    }
}

//��վö�ٲ���
inline void set_avail(int mask){//��վ01ö��_��ö����תΪ01���
    int pos=0;
    for(int i=0;i<4;i++)
        most_avail_fighter[i]=max(actual_avail[i]-1,0);
    while(mask){
        if(mask&1)
            most_avail_fighter[pos]=actual_avail[pos];
        pos++;
        mask>>=1;
    }
}

inline int getone(int tar){//��ȡ1�ĸ���
    int res=0;
    while(tar){
        tar&=(tar-1);
        res++;
    }
    return res;
}

bool avail_array_cmp(int a,int b){//��վ01ö��˳��
    return getone(a)<getone(b);
}

//�������
bool cal_run(){
    double curAir=0.0;
    //fighter greedy
    sort(gridVec.begin(),gridVec.end(),cal_cmp_grid);
    sort(planeVec.begin(),planeVec.end(),cal_cmp_plane);

    int usedfighter=0;
    int gsz=gridVec.size(),psz=planeVec.size();
    for(int i=0;i<gsz;i++){
        if(curAir>=airDomin||i>psz)break;
        if(planeVec[i].getAirDominance()!=0){
            planeVec[i].category=PLANE_FIGHTER;
            curAir+=formulaFighter(gridVec[i].gridSize,planeVec[i].getAirDominance());
            planeVecF.push_back(planeVec[i]);
        }
        usedfighter=i+1;
    }
    if(curAir<airDomin){
        cout<<"�ƿ��޷����㣬��ǰ������ֵ: "<<curAir<<endl;
        return false;
    }
    //bomber greedy
    sort(planeVecA.begin(),planeVecA.end(),cal_cmp_plane_damage);
    for(int i=0;i<psz&&planeVecA.size()<gsz-planeVecF.size();i++)
        if(planeVec[i].getCategoryID()==PLANE_BOMBER||planeVec[i].getCategoryID()==PLANE_TORPEDO_ATTACKER)
            planeVecA.push_back(planeVec[i]);

    //search data prepare
    //��վö�٣�д�úܱ���
    int avail_array[64],carriermask=1<<carrierNum;
    for(int i=0;i<carriermask;i++)
        avail_array[i]=i;
    sort(avail_array,avail_array+carriermask,avail_array_cmp);

    sort(planeVecA.begin(),planeVecA.end(),cal_cmp_plane_damageOP);
    memset(fsign,0,sizeof(fsign));
    memset(asign,0,sizeof(asign));
    ResA=-1;

    //search
    SYSTEMTIME t1;//��¼����ʱ��
    GetLocalTime(&t1);
    int i=0;
    for(int i=0;i<carriermask;i++){//ö�ٷ�վ
        if(ResA!=-1&&(getone(avail_array[i])>getone(avail_array[i-1])))break;//��վ��Ŀ����ʱ�������Ƿ�����˳�
        set_avail(avail_array[i]);//��ȡ��վ01���
        baoliSearch(0,planeVecF.size(),planeVecA.size(),gsz);
    }
    SYSTEMTIME t2;
    GetLocalTime(&t2);
    cout<<"AD: "<<thatair<<" Atk: "<<ResA<<" "<<answer<<" "<<(t2.wSecond-t1.wSecond)*1000+t2.wMilliseconds-t1.wMilliseconds<<"ms"<<endl;

    return true;
}


string answer_c[6][4];//����װ����
string &cal_get_res(){//����Ļ�ȡ
    int f=0,a=0;
    for(int i=0;i<6;i++)
        for(int j=0;j<4;j++)
            answer_c[i][j]=" - ";

    for(int i=0;answer[i]!='\0';i++){
        Grid curg=gridVec[i];
        if(answer[i]=='1'){
            answer_c[curg.carrierPos][curg.gridPos]=planeVecF[f].getName();
            f++;
        }
        else if(answer[i]=='2'){
            answer_c[curg.carrierPos][curg.gridPos]=planeVecA[a].getName();
            a++;
        }
    }

    stringstream stmp;
    for(int i=0;i<6;i++){
        for(int j=0;j<4;j++)
            stmp<<answer_c[i][j]<<" ";
        stmp<<endl;
    }
    string *ret=new string();
    *ret=stmp.str();
    return *ret;
}

ostream &operator<<(ostream &os,Grid &item){
    os<<item.carrierPos<<" "<<item.gridPos<<" "<<item.gridSize<<" "<<item.carrierAtk;
    return os;
}
