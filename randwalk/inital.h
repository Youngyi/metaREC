#pragma once
#include<vector>
#include<map>
#include<string>
#define TYPEUSER 0
#define TYPELOCATION 1
#define NOALLOWNEW 1

//CHECKINDATA��FRIENDDATA��ԭ���ݣ�����ǩ�����ݺͺ��ѹ�ϵ����
//��ʹ�û���Ԫ·�����Ƽ��㷨ʱ
//CHECKTESTDATA��ʾ��һ��ʱ��ε�ǩ����¼���Դ˻���������߸���
//TRAINDATA���ΪFEATUREDATA
//FEATUREDATA��ʾ����ļ��������ǲ��Լ��ļ�����ѵ�����ļ����ļ���ʽ���������߸�����

//����ѵ�������õ��ļ���
#define TRAIN_CHECKINDATA "d:\\data\\traindata1.txt"
#define TRAIN_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TRAIN_CHECKTESTDATA "d:\\data\\traindata2.txt"
#define TRAIN_FEATUREDATA "d:\\data\\trainfeaturedata.txt"

//���ɲ��Լ���
#define TEST_CHECKINDATA "d:\\data\\testdata1.txt"
#define TEST_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TEST_CHECKTESTDATA "d:\\data\\filter_testdata2.txt"
#define TEST_FEATUREDATA "d:\\data\\testfeaturedata.txt"

//����ѵ�������õ��ļ���������LL�ߣ�
#define TRAIN_FEATUREDATANOLLEDGE "d:\\data\\trainfeaturedata_nolledge.txt"
//����ѵ�������õ��ļ���������LL�ߣ�
#define TEST_FEATUREDATANOLLEDGE "d:\\data\\testfeaturedata_nolledge.txt"

#define ISDEBUG 0

#define CPU_COUNT 0//0-ֱ�ӽ������ֵ��ΪȨֵ
#define CPU_LOG 1//1-log
//outputNegMod: 1��ʾ��������ͬ�ĸ���-train����0��ʾ���еĸ���-test��"
#define NEGMOD_ALL_FORTEST 0
#define NEGMOD_ALL_FORTRAIN 1



/*
2010��9��ǰ������Ϊtraindata1��
2010��9�µ�����Ϊtraindata2��
2010��9��ǰ��9�µ�����Ϊtestdata1��
2010��10�µ�����Ϊtestdata2

"d:\\data\\filter_testdata2.txt":   ���ļ��Ƕ��ڲ��Լ��жԱ��ļ��н����˴���������׶�ǩ����������20���û�ɾ��

*/


/*
������
#define CHECKINDATA "d:\\data\\useful\\testdata1.txt"
#define FRIENDDATA "d:\\data\\useful\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\useful\\filter"
#define TRAINDATA "d:\\data\\useful\\testfeaturedata.txt"

����
#define CHECKINDATA "d:\\data\\test\\testdata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriend.txt"
#define CHECKTESTDATA "d:\\data\\test\\testtestdata.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
�����û���Эͬ���ˣ����ں��ѹ�ϵ��Эͬ����
#define CHECKINDATA "d:\\data\\testdata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\filter_testdata2.txt"
#define TRAINDATA "d:\\data\\testfeaturedata.txt"

#define CHECKINDATA "d:\\data\\traindata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\traindata2.txt"
#define TRAINDATA "d:\\data\\trainfeaturedata.txt"

#define CHECKINDATA "d:\\data\\test\\traindata1.txt"
#define FRIENDDATA "d:\\data\\test\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\test\\t.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
/* 
#define CHECKINDATA "d:\\data\\test\\testdata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriend.txt"
#define CHECKTESTDATA "d:\\data\\test\\testtestdata.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"

#define CHECKINDATA "d:\\data\\testdata1.txt"
#define FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define CHECKTESTDATA "d:\\data\\testdata2.txt"
#define TRAINDATA "d:\\data\\testfeaturedata.txt"
*/

using namespace std;
class Edge;
class Item;
class User;
class Location;

typedef map<int ,Edge * > EdgeMap;
typedef map<int,User*> UserMap;
typedef map<int,Location*> LocMap;
typedef map<int,float> FeatureMap;


//ȫ�ֱ��������ڴ洢���е��û������е�λ�õĽڵ�ָ��


User* getUserPtrbyId(int id,int type=0);
Location* getLocationPtrbyId(int id,int type=0);



