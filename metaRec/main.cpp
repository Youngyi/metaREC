#include "Socialnet.h"
#include "MetaCpu.h"
#include <iostream>
#include<sstream>
#include "UCFRecommend.h"
#include "FriBasedRecommend.h"
using namespace std;
//���ԣ�����DEBUG
#define CHECKINDATA "d:\\data\\test\\testdata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriend.txt"
#define CHECKTESTDATA "d:\\data\\test\\testtestdata.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
/*
//ѵ����

#define TRAIN_CHECKINDATA "d:\\data\\traindata1.txt"
#define TRAIN_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TRAIN_CHECKTESTDATA "d:\\data\\traindata2.txt"

//���ɲ��Լ���
#define TEST_CHECKINDATA "d:\\data\\testdata1.txt"
#define TEST_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TEST_CHECKTESTDATA "d:\\data\\filter_testdata2.txt"
*/
#define INPUTADRESS "d:\\data\\input\\"
#define TRAIN_CHECKINDATA "traindata1.txt"
#define TRAIN_FRIENDDATA "friends_edges.txt"
#define TRAIN_CHECKTESTDATA "traindata2.txt"

//���ɲ��Լ���
#define TEST_CHECKINDATA "testdata1.txt"
#define TEST_FRIENDDATA "friends_edges.txt"
#define TEST_CHECKTESTDATA "filter_testdata2.txt"

#define OUTPUTADRESS "d:\\data\\output\\"
#define OUTPUTFILE "featuredata"
int isDebug=0;

void main(){
    int train_test;
    string train_testStr;
    string checkinFileName;
    string friendFileName;
    string caseFileName;
    //�û�
    cout<<"\
          ѵ����-1 \n\
          or ���Լ�-2 \n\
          or DEBUG-3 \n\
          or �����û���Эͬ���� -4 \n\
          or ���ں��ѵ�Эͬ���� -5 \n\
          "<<endl;
    cin>>train_test;
    cout<<"Gowalla-1 or foursuare -2"<<endl;
    int dateSource=1;
    cin>>dateSource;
    string dateSourceStr="";
    string pis="";
    if (dateSource==2)
    {
        pis+="foursquare_";
        dateSourceStr="foursquare_";
    }else{
        pis+="gowalla_";
        dateSourceStr="gowalla_";
    }
    //string fileType=".txt";
    bool isTrain=true;
    //ѡ���������ļ�
    if (train_test==1)
    {
        train_testStr="train";
        checkinFileName=INPUTADRESS+pis+TRAIN_CHECKINDATA;
        friendFileName=INPUTADRESS+pis+TRAIN_FRIENDDATA;
        caseFileName=INPUTADRESS+pis+TRAIN_CHECKTESTDATA;
    }else if (train_test==2)
    {
        train_testStr="test";
        checkinFileName=INPUTADRESS+pis+TEST_CHECKINDATA;
        friendFileName=INPUTADRESS+pis+TEST_FRIENDDATA;
        caseFileName=INPUTADRESS+pis+TEST_CHECKTESTDATA;
        isTrain=false;
    }else if (train_test==3)
    {
        isDebug=1;
        checkinFileName=INPUTADRESS+pis+CHECKINDATA;
        friendFileName=INPUTADRESS+pis+FRIENDDATA;
        caseFileName=INPUTADRESS+pis+CHECKTESTDATA;
        train_testStr="debug";
    }else if (train_test == 4 || train_test == 5)
    {
        //��ͳ���Ƽ��㷨����Эͬ������ʹ�õ����ݺͲ��Լ�ʹ�õ�������ͬ��
        train_testStr="UBFR or FriendBased";
        checkinFileName=INPUTADRESS+pis+TEST_CHECKINDATA;//CHECKINDATA;//
        friendFileName=INPUTADRESS+pis+TEST_FRIENDDATA;//FRIENDDATA;//
        caseFileName=INPUTADRESS+pis+TEST_CHECKTESTDATA;//CHECKTESTDATA;//
        isTrain=false;

    }
    int interval_hour=0;
    if (train_test != 4 && train_test != 5)
    {
        cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
        cin>>interval_hour;
    }
    

    
    cout<<"��������ֵ���㷽����1-normal��2-log��3-0/1"<<endl;
    int weightCpuType;
    string weightCpuTypeStr;
    cin>>weightCpuType;
    Socialnet * socialNet=NULL;
    //���������ļ�������ֵ���㷽�����Լ�LL��ʱ���������罻����
    if (weightCpuType==1)
    {
        socialNet=Socialnet::createSocialnet(checkinFileName,friendFileName,WEIGHTCPUTYPE_NORMAL,true,interval_hour);
        weightCpuTypeStr="normal";
    }else if (weightCpuType==2)
    {
        socialNet=Socialnet::createSocialnet(checkinFileName,friendFileName,WEIGHTCPUTYPE_LOG,true,interval_hour);
        weightCpuTypeStr="log";
    }else if (weightCpuType==3)
    {
        socialNet=Socialnet::createSocialnet(checkinFileName,friendFileName,WEIGHTCPUTYPE_BINARY,true,interval_hour);
        weightCpuTypeStr="binary";
    }

    //begin������
    if (train_test == 4){
        UCFRecommend * ucfr = new UCFRecommend(socialNet);
        ucfr->Recommend(caseFileName);
    }else if (train_test == 5){
        FriBasedRecommend * friendeBasedRecommend = new FriBasedRecommend(socialNet);
        friendeBasedRecommend->Recommend(caseFileName);
    }else{
        stringstream ss;
        string outputFileName;
        ss<<OUTPUTADRESS<<dateSourceStr<<OUTPUTFILE<<"_"<<weightCpuTypeStr<<"_"<<train_testStr<<"_"<<interval_hour<<"hours.txt";
        ss>>outputFileName;
        MetaCpu *metaCpu=new MetaCpu(socialNet,isTrain,true);
        metaCpu->metaCpu(caseFileName,outputFileName);
    }
    int cc;
    cin>>cc;
}