#pragma once
#include <list>
class Edge
{
public:
    Edge(int weightCPUType,int time=0);
    void addEdge(int time);
    void addEdge();
    void print();
    float getWeight();//����Ȩֵ���㷽���ͱߵĴ��������㲢���رߵ�Ȩֵ
    ~Edge(void);
private:
    int weightCPUType;//Ȩ�ؼ��㷽��
    int count;//�ߵĴ�����
    int type;//�ߵ�����

    list<int> checkinTimes;
};

