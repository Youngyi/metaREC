#pragma once
#include"inital.h"
#include"Edge.h"
class Item
{
protected:
    int id;
    EdgeMap tolocv;//locationId,*Edge
    EdgeMap touserv;//userId,*Edge
    int tolocAllWei;//ָ��λ�ýڵ�����еıߵ�Ȩ�غ�
    int touserAllWei;
public:
    Item(int itemid);
    void print(string type);
    EdgeMap* getToLocE();
    EdgeMap* getToUserE();
    EdgeMap * getToItemE(int type);
    EdgeMap * getToItemE(char type);
    int getAllWeight(char type);
    ~Item(void);
};

class User:public Item
{
    friend class Location;
public:
    User(int userid);
    ~User(void);
    void addToLocV(int locid,int time);
    void addToUserV(int userid);
};
//λ��-�û����ϵ�Ȩֵ��ʱ�伯�ϴ����user������
class Location:public Item{
public:
    Location(int locationid);
    void addToLocV(int locid);
    //����ߣ�����Ҫ���������Ϣ����һ���û�-��λ�ã��ڶ���λ��-���û���
    //��һ�θ���Ȩֵ��ʱ�����Ϣ����edge���󣬵ڶ���ֱ��ָ���һ�δ����Ķ��󼴿�
    void addToUserV(int userid);

};
