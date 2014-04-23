#include "FriBasedRecommend.h"
#include "Item.h"
#include "Edge.h"
#include<sstream>
#include<iostream>
#include<fstream>
#include <set>

FriBasedRecommend::FriBasedRecommend(Socialnet* socNet):BasedRecommend(socNet)
{
}

void FriBasedRecommend::Recommend(string checkinDataFileName){
    ifstream testfile(checkinDataFileName);

    char buffer[100];
    int i=0;
    int lastUserId=-1;
    map<int,float> f2fWeightMap;//�û�i���û�k�����ƶȣ�SIk,i������iΪ��ǰ��userid��<int-k��float-f2fWeight��SIk,i>
    map<int,float> recResult;//����û�i���Ƽ���������е�һ��int��ʾλ��j��id��float��ʾԤ�⵽������ֵ
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
            
            
            /*
            if (rightCaseOfUser>=TOPN1)
            {
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                allRec[0]+=TOPN1;
            }
            if (rightCaseOfUser>=TOPN2)
            {
                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                allRec[1]+=TOPN2;
            }
            */
            if (rightCaseOfUser>=TOPN3)
            {
                rightRec[0]+=rightRecOfUser[0];
                rightCase[0] += rightCaseOfUser;
                allRec[0]+=TOPN1;

                rightRec[1]+=rightRecOfUser[1];
                rightCase[1] += rightCaseOfUser;
                allRec[1]+=TOPN2;

                rightRec[2]+=rightRecOfUser[2];
                rightCase[2] += rightCaseOfUser;
                allRec[2]+=TOPN3;
            }

            if(allRec[0]!=0){
                if(ISDEBUG){
                    cout<<"top-5:"<<endl;
                    printMap(sortedRec1);
                }

                cout<<" FriendBased��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"�ٻ���rightRec[0]/rightCase"<<rightRec[0]<<"/"<<rightCase[0]<<" :"<<(float)rightRec[0]/rightCase[0]<<endl;
                if(ISDEBUG){
                    cout<<"top-10:"<<endl;
                    printMap(sortedRec2);
                }
                cout<<" ��ȷ��rightRec[1]/allRec[1]��"<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase[1]<<" :"<<(float)rightRec[1]/rightCase[1]<<endl;
                if(ISDEBUG){
                    cout<<"top-20:"<<endl;
                    printMap(sortedRec3);
                }
                cout<<" ��ȷ��rightRec[2]/allRec[2]��"<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase[2]<<" :"<<(float)rightRec[2]/rightCase[2]<<endl;
            }
            //��������µ�user���Ƽ���Ϣ
            //�������һЩ����
            f2fWeightMap.clear();
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
            for (int i=0;i<3;i++)
            {
                rightRecOfUser[i]=0;
            }
            rightCaseOfUser=0;


            //����ÿһ���û��������û������ƶ�Wi,k,����������u2uWeightMap
            calculateSIik(f2fWeightMap,userid);
            if(ISDEBUG){
                cout<<"�û���"<<userid<<endl;
                cout<<"�û�-�û�Ȩֵ�����û���Ȩֵ��"<<userid<<endl;
                printMap(f2fWeightMap);
            }
            //�����û���δ���۵�ITEM���������ۣ��Դ���Ϊ�Ƽ����
            calRecResult(recResult,f2fWeightMap,userid);//������������������ʹ�õ���UCF���Ƽ���������ȷ���ɲ������á�
            if(ISDEBUG){
                cout<<"�Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
                printMap(recResult);
            }

            //ѡ��top-k���Ƽ����

            sortRecResult(sortedRec1,recResult,TOPN1);
            sortRecResult(sortedRec2,recResult,TOPN2);
            sortRecResult(sortedRec3,recResult,TOPN3);
            if (recResult.size()>20 &&(sortedRec1.size() != TOPN1||sortedRec2.size() != TOPN2||sortedRec3.size() != TOPN3))
            {
                cout<<"sortedRec.size() != TOPN"<<endl;
                int xx;
                cin>>xx;
            }
            //cout<<"�������Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            lastUserId=userid;
        }
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
    cout<<"FriendBased top-5:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[0]/rightCase[0]<<endl;
    cout<<"top-10:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[1]/rightCase[2]<<endl;
    cout<<"top-20:"<<endl;
    cout<<" ��ȷ�ʣ�"<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"�ٻ���"<<(float)rightRec[2]/rightCase[3]<<endl;
}


FriBasedRecommend::~FriBasedRecommend(void)
{
}
