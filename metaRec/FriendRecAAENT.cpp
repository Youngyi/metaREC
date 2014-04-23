#include "FriendRecAAENT.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>
#include <math.h>

FriendRecAAENT::FriendRecAAENT(Socialnet* socNet):BasedRecommend(socNet)
{

}


FriendRecAAENT::~FriendRecAAENT(void)
{
}
void FriendRecAAENT::calRecResult(map<int,float>* recResult,int user1id){
    Item * user1 = this->socialNet->getItemPtrById(user1id,ITEMTYPE_USER);
    for (EdgeMap::const_iterator user1CheckLocIter = user1->getToLocE()->begin(); user1CheckLocIter != user1->getToLocE()->end(); ++user1CheckLocIter){
        Item * user1CheckLoc = socialNet->getItemPtrById(user1CheckLocIter->first,ITEMTYPE_LOCATION);

        for (EdgeMap::const_iterator user2Iter = user1CheckLoc->getToUserE()->begin();user2Iter != user1CheckLoc->getToUserE()->end(); ++user2Iter){
            Item * user2 = socialNet->getItemPtrById(user2Iter->first, ITEMTYPE_USER);
            
            if (user2->getId() == user1id || user1->getToUserE()->find(user2->getId()) != user1->getToUserE()->end())
            {
                //���user2��user1��ͬ�����������Ѿ������ѣ�����
                continue;
            }
            if (recResult->find(user2->getId()) != recResult->end())
            {
                //�Ѿ��������
                continue;
            }
            //�����û�user1���������������е����ѵ�����(user2)֮������ƶ�
            if (this->aaentFeature.find(user2->getId()) != aaentFeature.end())
            {
                //���������user2��user1�ģ���user1��user2����ͬ
                recResult->insert(map<int,float>::value_type(user2->getId(),aaentFeature[user2->getId()]->at(user1id)));
            }else{
                //û�м����
                float resultf = 0.0f;
                for (EdgeMap::const_iterator user1CheckinLocIterForCal = user1->getToLocE()->begin(); user1CheckinLocIterForCal != user1->getToLocE()->end(); ++user1CheckinLocIterForCal){
                    if (user2->getToLocE()->find(user1CheckinLocIterForCal->first) != user2->getToLocE()->end())
                    {
                        Item * coCheckinLoc = socialNet->getItemPtrById(user1CheckinLocIterForCal->first,ITEMTYPE_LOCATION);
                        resultf += 1.0f/log((double)coCheckinLoc->getLocEntropy());
                    }
                }
                recResult->insert(map<int,float>::value_type(user2->getId(),resultf));
            }
        }
    }
    aaentFeature.insert(map<int, map<int,float>*>::value_type(user1id,recResult));
}

void FriendRecAAENT::Recommend(string friendData2FileName){
    cout<<friendData2FileName<<endl;
    ifstream testfile(friendData2FileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> * recResult = NULL;//����û�i���Ƽ���������е�һ��int��ʾitem��id��float��ʾԤ�⵽������ֵ
    map<int,float> sortedRec1;//Top-k���Ƽ����TOPN1-5
    map<int,float> sortedRec2;//Top-k���Ƽ����TOPN2-10

    int rightRec[2]={0};//�Ƽ���ȷ������
    int allRec[2]={0};//�Ƽ������е�Ԫ������
    int rightCase[2]={0};//Ӧ�ñ��Ƽ�������

    int rightRecOfUser[2] = {0};
    int rightCaseOfUser=0;


    while(true){
        i++;
        //��һ��ǩ����¼������
        if(testfile.eof()){
            cout<<"���������ս����"<<endl;
            break;
        }
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=2){
            break;
        }
        int user1id=atoi(result[0].data());
        int user2id=atoi(result[1].data());
        Item * user1=socialNet->getItemPtrById(user1id,ITEMTYPE_USER);
        Item * user2 = socialNet->getItemPtrById(user2id, ITEMTYPE_LOCATION);
        if(user1==NULL||user2==NULL){//����û�����λ�ò�����
            continue;
        }

        
        if(user1id!=lastUserId){//�����һ���µ��û���Ϊ���Ƽ�TOP-K�ĵص�
            if (rightCaseOfUser>=TOPN2 )//|| rightRecOfUser[1]!=0)
            {
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                allRec[0]+=TOPN1;

                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                allRec[1]+=TOPN2;
            }
            if(allRec[0]!=0){
                //cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<"friendrec_aan"<<friendData2FileName<<endl;
                cout<<" ��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"�ٻ���rightRec[0]/rightCase[0]"<<rightRec[0]<<"/"<<rightCase[0]<<" :"<<(float)rightRec[0]/rightCase[0]<<endl;
                // cout<<"top-10:"<<endl;
                // printMap(sortedRec2);
                cout<<" ��ȷ��rightRec[1]/allRec[1]��"<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCas[1]e"<<rightRec[1]<<"/"<<rightCase[1]<<" :"<<(float)rightRec[1]/rightCase[1]<<endl;
                //cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
            }
            //�������һЩ����

            sortedRec1.clear();
            sortedRec2.clear();
            for (int i=0;i<2;i++)
            {
                rightRecOfUser[i]=0;
            }
            rightCaseOfUser=0;


            //�����û���δ���۵�ITEM���������ۣ��Դ���Ϊ�Ƽ����
            recResult = new map<int,float>();
            calRecResult(recResult,user1id);
            //cout<<"�Ƽ��������λ��ID���Ƽ��ȣ�"<<user1id<<endl;
            //printMap(recResult);

            //ѡ��top-k���Ƽ����
            sortRecResult(sortedRec1,*recResult,TOPN1);
            sortRecResult(sortedRec2,*recResult,TOPN2);
            cout<<"�������Ƽ��������λ��ID���Ƽ��ȣ�"<<user1id<<endl;
            //printMap(sortedRec1);

            lastUserId=user1id;
        }
        //���㾫ȷ�ʺ��ٻ���
        //��һ��ͳ���Ƽ���ȷ������
        if(sortedRec1.find(user2id)!=sortedRec1.end()){
            if(sortedRec1[user2id]!=-1){
                rightRecOfUser[0]++;
                sortedRec1[user2id]=-1;
            }
        }
        if(sortedRec2.find(user2id)!=sortedRec2.end()){
            if(sortedRec2[user2id]!=-1){
                rightRecOfUser[1]++;
                sortedRec2[user2id]=-1;
            }
        }
        rightCaseOfUser++;
    }
    //���㾫ȷ�ʺ��ٻ���
    cout<<"FriendRecAAENT"<<friendData2FileName<<endl;
    cout<<"��ɣ�"<<endl;
    cout<<"top-5:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[0]/rightCase[0]<<endl;
    cout<<"top-10:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[1]/rightCase[1]<<endl;
}
