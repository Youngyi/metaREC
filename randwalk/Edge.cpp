#include "Edge.h"
#include<iostream>
#include<string>
#include <math.h>
using namespace std;
extern int weightCPUtype;//λ��-λ��Ȩֵ�ļ��㷽��
void Edge::addEdge(int time)
{
   // this->time.push_back(time);
    
    this->count++;
}
void Edge::addEdge()
{
    this->count++;
}

Edge::Edge(void)
{
    count=0;
}


float Edge::getWeight(){
    if(weightCPUtype==CPU_LOG){
        return log((double)this->count);
    }
    return count;
}
int Edge::getCount(){
    return count;
}


void Edge::addCount(){
    count++;
}
void Edge::print(){
    cout<<"weight:"<<this->getWeight()<<" ";
    /*
    if(time.size()!=0){
         cout<<"time: ";
         for(int i=0;i<time.size();i++)
             cout<<time[i]<<",";
    }*/
   
    
}
