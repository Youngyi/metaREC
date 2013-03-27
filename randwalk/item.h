#pragma once
#include"inital.h"
#include"Edge.h"
class Item
{
protected:
    int id;
    EdgeMap tolocv;
    EdgeMap touserv;
public:
    Item(int itemid);
    ~Item(void);
};

class User:public Item
{
    friend class Location;
public:
    User(int userid);
    ~User(void);
    void addToLocV(int locid,int time);
    void addToUserV();
};
//λ��-�û����ϵ�Ȩֵ��ʱ�伯�ϴ����user������
class Location:Item{
public:
    Location(int locationid);
    void addToLocV(int locid);
    //����ߣ�����Ҫ���������Ϣ����һ���û�-��λ�ã��ڶ���λ��-���û���
    //��һ�θ���Ȩֵ��ʱ�����Ϣ����edge���󣬵ڶ���ֱ��ָ���һ�δ����Ķ��󼴿�
    void addToUserV(int userid);
};
