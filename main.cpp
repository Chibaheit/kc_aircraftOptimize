#include <iostream>
#include <fstream>
#include <sstream>
#include "core/core.h"

using namespace std;

int main(void)
{
    core_initialization();

////temporary
    fstream in;
    stringstream ss;
    string tmp;
    in.open("query.txt",ios::in);
    while(in.good()){
        in>>tmp;
        ss<<tmp<<" ";
    }
    tmp=ss.str();
    cout<<core_query(tmp);

    system("pause");
////
    return 0;
}

//        main  ��ʱ����
//        |
//        core  ���㹦�ܱ���
//           |-io          ��P_DAT��C_DAT���뺽ĸ&���ػ��趨
//           |-calculator  ���������
//           |-Carrier     ��ĸ��
//           |-Plane       ���ػ���
//           |-gobal       ���㹦�ܵ�ȫ�ֱ���
