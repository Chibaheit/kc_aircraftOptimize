#include "core/predef.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "core/core.h"

using namespace std;

//int main(void)
int main(int argc,char *argv[])
{
    core_initialization();
    fstream in;
    stringstream ss;

    #ifdef DEBUG
        string str;
        in.open("query.txt",ios::in);
        while(in.good()){
            in>>str;
            ss<<str<<" ";
        }
    #else
        for(int i=1;i<argc;i++)
            ss<<argv[i]<<" ";
    #endif

    cout<<core_query(ss).str();

    #ifdef WINDOWS
        system("pause");
    #endif
    return 0;
}
//
//        ����main  ��ʱ����
//        ��
//        ����core
//           ����predef        Ԥ�賣��
//           ��
//           ����io            ��P_DAT��C_DAT���뺽ĸ&���ػ��趨
//           ��
//           ����calculator    ��������߼�
//           ����formulation   ��ʽ�������Ĺ�ʽ������formular_inline.h��
//           ����atkdp         ���ڻ��˺�������
//           ����belongingStructure  ����ڻ��˺�������������ݽṹ
//           ��
//           ����grid          ��ĸ������
//           ����Carrier
//           ����Plane
//           ��
//           ����gobal
