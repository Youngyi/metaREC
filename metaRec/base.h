#pragma once
#include<vector>
#include<map>
#include<string>

//Ȩֵ���㷽��
#define WEIGHTCPUTYPE_NORMAL 0//��ͨ
#define WEIGHTCPUTYPE_LOG 1//log
#define WEIGHTCPUTYPE_BINARY 2//0-1
#define WEIGHTCPUTYPE_CLASSIFY 3//�ּ���TODO

//item����
#define ITEMTYPE_USER 0
#define ITEMTYPE_LOCATION 1

//EDGE����
#define EDGETYPE_CHECKIN 0
#define EDGETYPE_FRIEND 1
#define EDGETYPE_LLRELATION 2

//����itemʱͬʱ�������
#define ALLOW_INSERT_IF_NULL true

#define NOT_CONCERN_LL_RELATION false



using namespace std;
class Edge;
class Item;
typedef map<int ,Edge * > EdgeMap;
typedef map<int,Item*> ItemMap;

typedef map<int,float> FeatureMap;



