#include "Edge.h"
#include "base.h"
#include<iostream>
using namespace std;
Edge::Edge(int weightCPUType,int time)
{
    this->type=type;
    this->weightCPUType=weightCPUType;
    count=1;
}


Edge::~Edge(void)
{
}
void Edge::addEdge(int time)
{
   // this->time.push_back(time);   
    this->count++;
}
void Edge::addEdge()
{
    this->count++;
}
    void print();

float Edge::getWeight(){//����Ȩֵ���㷽���ͱߵĴ��������㲢���رߵ�Ȩֵ
    if(this->weightCPUType==WEIGHTCPUTYPE_LOG){
        return log((double)this->count);
    }else if(this->weightCPUType==WEIGHTCPUTYPE_NORMAL){
        return (float)this->count;
    }else if(weightCPUType==WEIGHTCPUTYPE_BINARY){
        return 1;
    }else if(weightCPUType==WEIGHTCPUTYPE_CLASSIFY){
        //TODO::�ֽ׶μ���Ȩֵ��
    }else{
        return -999999;//ERROR
    }
    return count;
}
void Edge::print(){
    cout<<"weight:"<<this->getWeight()<<" "<<"count:"<<count;
    /*
    if(time.size()!=0){
         cout<<"time: ";
         for(int i=0;i<time.size();i++)
             cout<<time[i]<<",";
    }*/
   
    
}