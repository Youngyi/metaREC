#pragma once
#include <map>
#include "Socialnet.h"
#define TOPN1 5
#define TOPN2 10
#define TOPN3 20
class BasedRecommend
{
protected:
    map<int, map<int,float>> allf2fWeight;

    Socialnet * socialNet;
    void calRecResult(map<int,float>&recResult,const map<int,float> & u2uWeightMap,int uid);
    void sortRecResult(map<int,float> &sortedRec,const map<int,float> &recResult,int N=20);
    void printMap(const map<int,float> &mapt );
     //�����û�ID��������û�����������֮������Ѷȣ����ѵ����̶ܳȣ�
    void calculateSIik(map<int,float>& f2fWeightMap,int userid);
    //�����û�����ʷ��¼�������û�֮������ƶ�
    void calculateWik(map<int,float> & wikMap,int userid);
public:
    BasedRecommend(Socialnet * socNet);
    ~BasedRecommend(void);
     
};

