#pragma once
#include"inital.h"
class Edge
{
private:
    int count;//����ñߵķ��ʴ���

    void addCount();
    //std::vector<int> time;
public:
    Edge(void);
    void addEdge(int time);
    void addEdge();
    void print();

    float getWeight();//�ߵ�Ȩֵ
    int getCount();//����ñߵķ��ʴ���
};
