#include "GeograRecommend.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>
#include <math.h>
#define ISDEBUG 0
//����λ������Ƽ���ʹ�õ�ϵ������˵Ӧ����ͨ���������ȷ������������ - TODO
#define A_PARAM_G 0.1
#define B_PARAM_G -100
//�ۺ�����Ȩ��ϵ��
#define A_PARAM_USG 0.1
#define B_PARAM_USG 0.2
#define C_PARAM_USG 1-A_PARAM_USG-B_PARAM_USG


GeograRecommend::GeograRecommend(Socialnet* socNet):BasedRecommend(socNet)
{
}

void GeograRecommend::CalRecResultForGEO(map<int,float>&recResult,int uid){
    Item * user = socialNet->getItemPtrById(uid,ITEMTYPE_USER);
    EdgeMap * userLocList = user->getToLocE();

    ItemMap* locList = socialNet->getLoclist();
    for(ItemMap::const_iterator locIter=locList->begin();locIter!=locList->end();++locIter){

    }
}
void GeograRecommend::Recommend(string checkinDataFileName, bool isUSG){
    ifstream testfile(checkinDataFileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> f2fWeightMap;//�û�i���û�k�����ƶȣ�SIk,i������iΪ��ǰ��userid��<int-k��float-f2fWeight��SIk,i>
    map<int,float> u2uWeightMap;//�û�i���û�k�����ƶȣ�Wi,k������iΪ��ǰ��userid��int-k��float-u2uWeight��Wi,k
    
    map<int,float> recResultU;//�����û���Эͬ��������û�i���Ƽ����ʣ����е�һ��int��ʾλ��j��id��float��ʾԤ�⵽������ֵ
    map<int,float> recResultS;//���ں��ѹ�ϵ����û�i���Ƽ����ʣ�
    map<int,float> recResultG;//���ڵ���λ������û�i���Ƽ�����

    map<int,float> sortedRec1;//Top-k���Ƽ����TOPN1-5
    map<int,float> sortedRec2;//Top-k���Ƽ����TOPN2-10
    map<int,float> sortedRec3;//Top-k���Ƽ����TOPN3-20

    set<int> locIdSet;//���Լ��е�ǰ�û��ѷ��ʹ����û��������������Լ����û���η���ĳһλ��
    int rightRec[3]={0};//�Ƽ���ȷ������
    int allRec[3]={0};//�Ƽ������е�Ԫ������
    int rightCase=0;//Ӧ�ñ��Ƽ�������

    while(true){
        i++;
        //��һ��ǩ����¼������
        if(testfile.eof()) break;
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);

        if(result.size()!=5){
            break;
        }

        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
       
        Item * user=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
        Item * loc = socialNet->getItemPtrById(locid,ITEMTYPE_LOCATION);

        //����ǩ����¼���

        if(user==NULL||loc==NULL){//����û�����λ�ò�����
            continue;
        }

        if(userid!=lastUserId){//�����һ���µ��û���Ϊ���Ƽ�TOP-K�ĵص�
            //�Ȱ���һ��user����Ϣ��ӡ����
            if(allRec[0]!=0){
                if(ISDEBUG){
                    cout<<"top-5:"<<endl;
                    printMap(sortedRec1);
                }
                cout<<" FriendBased��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"�ٻ���rightRec[0]/rightCase"<<rightRec<<"/"<<rightCase<<" :"<<(float)rightRec[0]/rightCase<<endl;
                if(ISDEBUG){
                    cout<<"top-10:"<<endl;
                    printMap(sortedRec2);
                }
                cout<<" ��ȷ��rightRec[1]/allRec[1]��"<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase<<" :"<<(float)rightRec[1]/rightCase<<endl;
                if(ISDEBUG){
                    cout<<"top-20:"<<endl;
                    printMap(sortedRec3);
                }
                cout<<" ��ȷ��rightRec[2]/allRec[2]��"<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase<<" :"<<(float)rightRec[2]/rightCase<<endl;
            }
            //��������µ�user���Ƽ���Ϣ
            //�������һЩ����
            f2fWeightMap.clear();
            recResultU.clear();
            recResultS.clear();
            recResultG.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();

            //�����û���δ���۵�ITEM���������ۣ��Դ���Ϊ�Ƽ����
            if (isUSG)
            {
                calculateSIik(f2fWeightMap,userid);
                calculateWik(u2uWeightMap, userid);
                calRecResult(recResultU,u2uWeightMap,userid);
                calRecResult(recResultS,f2fWeightMap,userid);
            }
            
            
            
            //ѡ��top-k���Ƽ����

            sortRecResult(sortedRec1,recResult,TOPN1);
            sortRecResult(sortedRec2,recResult,TOPN2);
            sortRecResult(sortedRec3,recResult,TOPN3);
            //cout<<"�������Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            allRec[0]+=TOPN1;
            allRec[1]+=TOPN2;
            allRec[2]+=TOPN3;
            lastUserId=userid;
        }
        //���㾫ȷ�ʺ��ٻ���
        //��һ��ͳ���Ƽ���ȷ������
        if(sortedRec1.find(locid)!=sortedRec1.end()){

            if(sortedRec1[locid]!=-1){
                rightRec[0]++;
                sortedRec1[locid]=-1;
            }
        }
        if(sortedRec2.find(locid)!=sortedRec2.end()){
            if(sortedRec2[locid]!=-1){
                rightRec[1]++;
                sortedRec2[locid]=-1;
            }
        }
        if(sortedRec3.find(locid)!=sortedRec3.end()){
            if(sortedRec3[locid]!=-1){
                rightRec[2]++;
                sortedRec3[locid]=-1;
            }
        }

        if(locIdSet.find(locid)==locIdSet.end()){
            rightCase++;
            locIdSet.insert(locid);
        }
    }
    //���㾫ȷ�ʺ��ٻ���
    cout<<"FriendBased top-5:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[0]/rightCase<<endl;
    cout<<"top-10:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[1]/rightCase<<endl;
    cout<<"top-20:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[2]/rightCase<<endl;
}

GeograRecommend::~GeograRecommend(void)
{
}
