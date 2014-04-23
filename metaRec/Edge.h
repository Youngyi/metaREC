#pragma once
//#include <list>
#include "base.h"
class Edge
{
public:
    Edge(int weightCPUType,int time=-1);
    void addEdge(int time);
    void addEdge();
    void print();
    float getWeight();//����Ȩֵ���㷽���ͱߵĴ��������㲢���رߵ�Ȩֵ
    float getWeight(int windowTime);
    ~Edge(void);
private:
    int weightCPUType;//Ȩ�ؼ��㷽��
    int count;//�ߵĴ�����
    int type;//�ߵ�����

    //list<int> checkinTimes;
    int vecWindowTime[WINDOWTIME_COUNT] ;
};

