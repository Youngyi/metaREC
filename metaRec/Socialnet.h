#pragma once
#include "base.h"
#include "Item.h"
class Socialnet
{
public:
    
    ~Socialnet(void);
    Item * getItemPtrById(int id,int itemType,bool isAllowNewType=false);
    bool isNeighbor(int fromId,int fromItemType,int toId,int toItemType);
    static Socialnet * createSocialnet(string chinkinFileName,string friendFileName,
        int weightCpuType=WEIGHTCPUTYPE_NORMAL,
        bool hasLLRelation=true
        ) ;
private:
    Socialnet(int,bool);
    void readCheckinData(string chinkinFileName);
    void readFriendData(string friendFileNmae);
    void addCheckin(int userId,int locId);
    void addFriend(int userId,int user2Id);
    void addLLRelation(int locId,int locId2);

private:
    ItemMap userList;
    ItemMap locList;

    int weightCpuType;//�ߵ�Ȩֵ���㷽��
    bool hasLLRelation;

    static int instanceCount;//����ģʽ
    static const int time_iterval=24*60*60;
};

