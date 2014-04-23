#include "UCFRecommend.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>
#define TOPN_1 10
#define TOPN_2 20
#define TOPN_3 40
UCFRecommend::UCFRecommend(Socialnet* socNet):BasedRecommend(socNet)
{
}


void UCFRecommend::Recommend(string fileName){
    cout<<fileName<<endl;
    ifstream testfile(fileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> u2uWeightMap;//�û�i���û�k�����ƶȣ�Wi,k������iΪ��ǰ��userid��int-k��float-u2uWeight��Wi,k
    map<int,float> recResult;//����û�i���Ƽ���������е�һ��int��ʾitem��id��float��ʾԤ�⵽������ֵ
    map<int,float> sortedRec1;//Top-k���Ƽ����TOPN1-5
    map<int,float> sortedRec2;//Top-k���Ƽ����TOPN2-10
    map<int,float> sortedRec3;//Top-k���Ƽ����TOPN3-20

    set<int> locIdSet;//���Լ��е�ǰ�û��ѷ��ʹ����û��������������Լ����û���η���ĳһλ��
    int rightRec[3]={0};//�Ƽ���ȷ������
    int allRec[3]={0};//�Ƽ������е�Ԫ������
    int rightCase[3]={0};//Ӧ�ñ��Ƽ�������

    int rightRecOfUser[3] = {0};
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
        if(result.size()!=5){
            break;
        }
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        Item * user=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
        Item * loc = socialNet->getItemPtrById(locid, ITEMTYPE_LOCATION);
        //����ǩ����¼���

        if(user==NULL||loc==NULL){//����û�����λ�ò�����
            continue;
        }
        if(userid!=lastUserId){//�����һ���µ��û���Ϊ���Ƽ�TOP-K�ĵص�

            if (rightCaseOfUser>=5)
            {
                rightCaseOfUser++;
                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                rightRec[2]+=rightRecOfUser[2];
                rightCase[2] += rightCaseOfUser;
                allRec[0]+=TOPN1;
                allRec[1]+=TOPN2;
                allRec[2]+=TOPN3;
            }
            if(allRec[0]!=0){
                //cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<"UCFR"<<fileName<<endl;
                cout<<" ��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"�ٻ���rightRec[0]/rightCase[0]"<<rightRec[0]<<"/"<<rightCase[0]<<" :"<<(float)rightRec[0]/rightCase[0]<<endl;
               // cout<<"top-10:"<<endl;
               // printMap(sortedRec2);
                cout<<" ��ȷ��rightRec[1]/allRec[1]��"<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase[1]"<<rightRec[1]<<"/"<<rightCase[1]<<" :"<<(float)rightRec[1]/rightCase[1]<<endl;
                //cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
                cout<<" ��ȷ��rightRec[2]/allRec[2]��"<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase[2]"<<rightRec[2]<<"/"<<rightCase[2]<<" :"<<(float)rightRec[2]/rightCase[2]<<endl;
            }
            //�������һЩ����
            u2uWeightMap.clear();
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
            for (int i=0;i<3;i++)
            {
                rightRecOfUser[i]=0;
            }
            if(locIdSet.find(locid)==locIdSet.end()) rightCaseOfUser++;

            //����ÿһ���û��������û������ƶ�Wi,k,����������u2uWeightMap
            calculateWik(u2uWeightMap,userid);
           // cout<<fileName<<endl;
            cout<<"�û���"<<userid<<endl;

            cout<<"�û�-�û�Ȩֵ�����û���Ȩֵ��"<<userid<<endl;
            // printMap(u2uWeightMap);

            //�����û���δ���۵�ITEM���������ۣ��Դ���Ϊ�Ƽ����
            calRecResult(recResult,u2uWeightMap,userid);
            cout<<"�Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            //printMap(recResult);

            //ѡ��top-k���Ƽ����
            sortRecResult(sortedRec1,recResult,TOPN_1);
            sortRecResult(sortedRec2,recResult,TOPN_2);
            sortRecResult(sortedRec3,recResult,TOPN_3);
            cout<<"�������Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            //printMap(sortedRec1);
            lastUserId=userid;
        }
        rightCaseOfUser++;
        //���㾫ȷ�ʺ��ٻ���
        //��һ��ͳ���Ƽ���ȷ������
        if(sortedRec1.find(locid)!=sortedRec1.end()){
            if(sortedRec1[locid]!=-1){
                rightRecOfUser[0]++;
                sortedRec1[locid]=-1;
            }
        }
        if(sortedRec2.find(locid)!=sortedRec2.end()){
            if(sortedRec2[locid]!=-1){
                rightRecOfUser[1]++;
                sortedRec2[locid]=-1;
            }
        }
        if(sortedRec3.find(locid)!=sortedRec3.end()){
            if(sortedRec3[locid]!=-1){
                rightRecOfUser[2]++;
                sortedRec3[locid]=-1;
            }
        }
        
        if(locIdSet.find(locid)==locIdSet.end()){
            rightCaseOfUser++;
            locIdSet.insert(locid);
        }
    }
    //���㾫ȷ�ʺ��ٻ���
    cout<<"UCFR"<<fileName<<endl;
    cout<<"��ɣ�"<<endl;
    cout<<"top-5:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[0]/rightCase[0]<<endl;
    cout<<"top-10:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[1]/rightCase[1]<<endl;
    cout<<"top-20:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[2]/rightCase[2]<<endl;
}

UCFRecommend::~UCFRecommend(void)
{
}
