#include <vector>
#include <map>
#include "Carrier.h"
#include "Plane.h"
#include "calculator.h"

int planes_tot;
vector<Plane>planes;
map<string,int>planes_m;//�ɻ���ӳ��

int carriers_tot;
vector<Carrier>carriers;
map<string,int>carriers_m;//����ӳ��

map<string,int>plane_cate_m;//������ӳ��
map<string,int>carrier_cate_m;

//�����ѯ
vector<Grid>gridVec;//�ػ���
vector<Plane>planeVec;//�ɻ�
vector<Plane>planeVecA;//attacker/bomber
int airDomin;//�ƿ�
