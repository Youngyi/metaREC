#include "item.h"


Item::Item(int itemid)
{
    this->id=itemid;
}


Item::~Item(void)
{
}

User::User(int userid)
    :Item(userid)
{
}
Location::Location(int locationid)
    :Item(locationid){}


User::~User(void)
{
}

void User::addToLocV(int locid,int time){
    Location* locPtr=::getLocationPtrbyId(locid);
    EdgeMap::iterator iter;
    iter=tolocv.find(locid);
    if(iter!=tolocv.end())
    {//�Ѿ�����һ���ߣ�����Ȩֵ��ʱ��set����
        iter->second->addEdge(time);
    }else{
        Edge * newedge=new Edge();
        newedge->addEdge(time);
        tolocv.insert(EdgeMap::value_type(locid,newedge));
    }
}
void Location::addToUserV(int userid){
    User * userPtr=::getUserPtrbyId(userid);
    EdgeMap::iterator iter=touserv.find(userid);
    if(iter==touserv.end())
    {
        Edge * oldedge=userPtr->tolocv[this->id];//������û��������洴����edge����ָ��
        touserv.insert(EdgeMap::value_type(userid,oldedge));
    }
}