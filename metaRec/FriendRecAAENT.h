#pragma once
#include "basedrecommend.h"
class FriendRecAAENT :
    public BasedRecommend
{
private:
    map<int,map<int,float> *> aaentFeature;//�洢�û�֮���aan�������ֱ������user1_ID,user2_ID,���ƶ�
    void calRecResult(map<int,float>*recResult,int user1id);
public:
    void Recommend(string checkinDataFileName);
    FriendRecAAENT(Socialnet* socNet);
    ~FriendRecAAENT(void);
};

