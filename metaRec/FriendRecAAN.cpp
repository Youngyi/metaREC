#include "FriendRecAAN.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>

FriendRecAAN::FriendRecAAN(Socialnet* socNet):BasedRecommend(socNet)
{
}


FriendRecAAN::~FriendRecAAN(void)
{
}
void calRecResult(map<int,float>* recResult,int user1id){
    
}

void FriendRecAAN::Recommend(string checkinDataFileName){
    /*
    cout<<checkinDataFileName<<endl;
    ifstream testfile(checkinDataFileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> * recResult = NULL;//����û�i���Ƽ���������е�һ��int��ʾitem��id��float��ʾԤ�⵽������ֵ
    map<int,float> sortedRec1;//Top-k���Ƽ����TOPN1-5
    map<int,float> sortedRec2;//Top-k���Ƽ����TOPN2-10
    map<int,float> sortedRec3;//Top-k���Ƽ����TOPN3-20

    int rightRec[3]={0};//�Ƽ���ȷ������
    int allRec[3]={0};//�Ƽ������е�Ԫ������
    int rightCase=0;//Ӧ�ñ��Ƽ�������


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
            if(allRec[0]!=0){
                //cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<"friendrec_aan"<<checkinDataFileName<<endl;
                cout<<" ��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"�ٻ���rightRec[0]/rightCase"<<rightRec<<"/"<<rightCase<<" :"<<(float)rightRec[0]/rightCase<<endl;
                // cout<<"top-10:"<<endl;
                // printMap(sortedRec2);
                cout<<" ��ȷ��rightRec[1]/allRec[1]��"<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase<<" :"<<(float)rightRec[1]/rightCase<<endl;
                //cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
                cout<<" ��ȷ��rightRec[2]/allRec[2]��"<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase<<" :"<<(float)rightRec[2]/rightCase<<endl;
            }
            //�������һЩ����

            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();


            //�����û���δ���۵�ITEM���������ۣ��Դ���Ϊ�Ƽ����
            calRecResult(recResult,user1id);
            cout<<"�Ƽ��������λ��ID���Ƽ��ȣ�"<<user1id<<endl;
            //printMap(recResult);

            //ѡ��top-k���Ƽ����
            sortRecResult(sortedRec1,*recResult,TOPN1);
            sortRecResult(sortedRec2,*recResult,TOPN2);
            sortRecResult(sortedRec3,*recResult,TOPN3);
            cout<<"�������Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            //printMap(sortedRec1);
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
    cout<<"UCFR"<<fileName<<endl;
    cout<<"��ɣ�"<<endl;
    cout<<"top-5:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[0]/rightCase<<endl;
    cout<<"top-10:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[1]/rightCase<<endl;
    cout<<"top-20:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[2]/rightCase<<endl;
    }
    */
}