#pragma once
#include "base.h"
class Item
{
public:
    Item(int itemid);
    Item(int itemid,int itemType);
    void print(int type,int kongbaiCount=0);

    //��ñ߼��ϵĲ�ͬ�ӿ�
    EdgeMap* getToLocE() ;
    EdgeMap* getToUserE() ;
    EdgeMap * getToItemE(int type) ;




    void addToLocE(int id,int weightCPUType,int time=0);
    void addToUserE(int id,int weightCUPType,int time=0);


    //��ȡĳһ��ߵ�Ȩֵ֮��
    float getAllWeight(int type);
    int getId();
    int getType();


private:
    void addEdge(int id,int weightCPUType,int time,int type);
protected:
    int id;
    int toUserAllWei;//ͬ��
    int type;//ITEMTYPE_USER or ITEMTYPE_LOCATION
    EdgeMap toLocE;//locationId,*Edge
    EdgeMap toUserE;//userId,*Edge
    int toLocAllWei;//ָ��λ�ýڵ�����еıߵ�Ȩ�غ�,Ĭ��ֵΪ-1.-1ʱ���¼��㣬�������-1˵���Ѿ��������
};

















/*
class User:public Item
{
    friend class Location;
public:
    User(int userid);

    void addToUserE(int userid);
};
//λ��-�û����ϵ�Ȩֵ��ʱ�伯�ϴ����user������
class Location:public Item{
public:
    Location(int locationid);
    //����ߣ�����Ҫ���������Ϣ����һ���û�-��λ�ã��ڶ���λ��-���û���
    //��һ�θ���Ȩֵ��ʱ�����Ϣ����edge���󣬵ڶ���ֱ��ָ���һ�δ����Ķ��󼴿�
    void addToUserE(int userid);

};
*/
