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

//        main  ��ʱ����
//        |
//        core
//           |-predef
//           |
//           |-io          ��P_DAT��C_DAT���뺽ĸ&���ػ��趨
//           |-calculator      �����߼�
//           |     |-formulation       �õ��Ĺ�ʽ
//           |     |-grid              ��ĸ������
//           |-Carrier     ��ĸ��
//           |-Plane       ���ػ���
//           |-gobal       ���㹦�ܵ�ȫ�ֱ���
