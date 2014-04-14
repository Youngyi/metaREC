#pragma once
#include "BasedRecommend.h"
class FriendRecAAN:
    public BasedRecommend
{
private:
    map<int,map<int,float> *> aanFeature;//�洢�û�֮���aan�������ֱ������user1_ID,user2_ID,���ƶ�
    void calRecResult(map<int,float>*recResult,int user1id);
public:
    void Recommend(string checkinDataFileName);
    FriendRecAAN(Socialnet* socNet);
    ~FriendRecAAN(void);
};

