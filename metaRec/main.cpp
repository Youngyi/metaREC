#include "Socialnet.h"
#include "MetaCpu.h"
#include <iostream>
#include<sstream>
#include "UCFRecommend.h"
#include "FriBasedRecommend.h"
#include "GeograRecommend.h"
#include "MetaCpuForFriendRec.h"
using namespace std;
//���ԣ�����DEBUG
#define CHECKINDATA "d:\\data\\test\\testCheckindata.txt"
#define FRIENDDATA "d:\\data\\test\\testfriendDate.txt"
#define FRIENDDATA2 "d:\\data\\test\\testfriendDate2.txt"
#define CHECKTESTDATA "d:\\data\\test\\testCheckindata2.txt"
#define TRAINDATA "d:\\data\\test\\trainfeaturedata.txt"
/*
//ѵ����

#define TRAIN_CHECKINDATA "d:\\data\\traindata1.txt"
#define TRAIN_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TRAIN_CHECKTESTDATA "d:\\data\\traindata2.txt"


#define TEST_CHECKINDATA "d:\\data\\testdata1.txt"
#define TEST_FRIENDDATA "d:\\data\\Gowalla_edges.txt"
#define TEST_CHECKTESTDATA "d:\\data\\filter_testdata2.txt"
*/
//#define INPUTADRESS "d:\\data\\input\\"

#define INPUTADRESS "e:\\checkin\\"

//���ɲ��Լ���
#define TRAIN_CHECKINDATA "traindata1.txt"
#define TRAIN_FRIENDDATA "friends_edges.txt"
#define TRAIN_CHECKTESTDATA "traindata2.txt"

//���ɲ��Լ���
#define TEST_CHECKINDATA "testdata1.txt"
#define TEST_FRIENDDATA "friends_edges.txt"
#define TEST_CHECKTESTDATA "testdata2.txt"



#define TEST_CHECKTESTDATANAME "testdata2"

#define OUTPUTADRESS "e:\\checkin\\"
#define OUTPUTFILE "featuredata"






//���ں����Ƽ�
#define FRIENDREC_CHECKINDATE "processedChekins.TXT"

//���ں����Ƽ���ѵ����
#define FRIENDREC_TRAINFRIENDDATE "friends_testdata1.TXT"
#define FRIENDREC_TRAINFRIENDDATE_2 "friends_testdata2.txt"

//���Լ�
#define FRIENDREC_TESTFRIENDDATE "friends_testdata1.TXT"
#define FRIENDREC_TESTFRIENDDATE_2 "friends_testdata2.txt"

#define FRIENDREC_OUTPUTFILE "friendrec_featuredata"


int isDebug=0;

void readSocialNetFile(){

}
void main(){
    int train_test;
    string train_testStr;
    string checkinFileName;
    string friendFileName;
    string caseFileName;
    //�û�
    cout<<"\
          λ���Ƽ���\n \
          or ѵ����-1 \n\
          or ���Լ�-2 \n\
          or DEBUG-3 \n\
          or �����û���Эͬ���� -4 \n\
          or ���ں��ѵ�Эͬ���� -5 \n\
          or λ����ص��Ƽ� -6 \n\
          or �ۺ��Ƽ��㷨 -7 \n\
          or ͳ��λ�þ����ǩ������ - 8\
          \n \
          �����Ƽ���\n \
          or ѵ���� -9 \n \
          or ���Լ�- 10 \n \
          or DEBUG-11 \n\
          "<<endl;
    cin>>train_test;
    cout<<"foursquare_NewYork_-1 or foursquare_all_ -2 gowalla_ -3"<<endl;
    int dateSource=1;
    cin>>dateSource;
    string dateSourceStr="";
    string pis="";
    if (dateSource==1)
    {
        pis+="foursquare_NewYork_";
        dateSourceStr="foursquare_NewYork_";
    }else if (dateSource == 2)
    {
        pis+="foursquare_all_";
        dateSourceStr="foursquare_all_";
    }else{
        pis+="gowalla_";
        dateSourceStr="gowalla_";
    }
    //string fileType=".txt";
    bool isTrain=true;
    int interval_hour=0;
    //ѡ���������ļ�

    switch (train_test)
    {
    case 1:
        {
            train_testStr="train";
            checkinFileName=INPUTADRESS+pis+TRAIN_CHECKINDATA;
            friendFileName=INPUTADRESS+pis+TRAIN_FRIENDDATA;
            caseFileName=INPUTADRESS+pis+TRAIN_CHECKTESTDATA;
            cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
            cin>>interval_hour;
            break;
        }
    case 2:
        {
            train_testStr="test";
            checkinFileName=INPUTADRESS+pis+TEST_CHECKINDATA;
            friendFileName=INPUTADRESS+pis+TEST_FRIENDDATA;
            caseFileName=INPUTADRESS+pis+TEST_CHECKTESTDATA;
            cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
            cin>>interval_hour;
            isTrain=false;
            break;
        }
    case 3:
        {
            isDebug=1;
            checkinFileName=CHECKINDATA;
            friendFileName=FRIENDDATA;
            caseFileName=CHECKTESTDATA;
            cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
            cin>>interval_hour;
            train_testStr="debug";
            break;
        }
    case 4:
    case 6:
    case 7:
        {
            train_testStr="Based";
            checkinFileName=INPUTADRESS+pis+TEST_CHECKINDATA;//CHECKINDATA;//
            friendFileName=INPUTADRESS+pis+TEST_FRIENDDATA;//FRIENDDATA;//
            int fileIndex=-1;
            cout<<"�������ļ����룺1-11"<<endl;
            cin>>fileIndex;
            stringstream ss;
            string s;
            ss << fileIndex;
            ss >> s;
            caseFileName=INPUTADRESS+pis+TEST_CHECKTESTDATANAME+"_"+s+".txt";//CHECKTESTDATA;//
            isTrain=false;
            break;
        }
    case  5:
    case 8:
        {
            train_testStr="Based";
            checkinFileName=INPUTADRESS+pis+TEST_CHECKINDATA;//CHECKINDATA;//
            friendFileName=INPUTADRESS+pis+TEST_FRIENDDATA;//FRIENDDATA;//
            caseFileName=INPUTADRESS+pis+TEST_CHECKTESTDATA;//CHECKTESTDATA;//
            isTrain=false;
            break;
        }
    case 9:
        {
            train_testStr="train";
            checkinFileName=INPUTADRESS+pis+FRIENDREC_CHECKINDATE;

            friendFileName=INPUTADRESS+pis+FRIENDREC_TRAINFRIENDDATE;
            caseFileName=INPUTADRESS+pis+FRIENDREC_TRAINFRIENDDATE_2;
            cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
            cin>>interval_hour;
            break;
        }
    case 10:
        {
            train_testStr="test";
            checkinFileName=INPUTADRESS+pis+FRIENDREC_CHECKINDATE;

            friendFileName=INPUTADRESS+pis+FRIENDREC_TESTFRIENDDATE;
            caseFileName=INPUTADRESS+pis+FRIENDREC_TESTFRIENDDATE_2;
            cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
            cin>>interval_hour;
            isTrain=false;
            break;
        }
    case 11:
        {
            isDebug=1;
            checkinFileName=CHECKINDATA;
            friendFileName=FRIENDDATA;
            caseFileName=FRIENDDATA2;
            cout<<"������LL�ߵ�ʱ������Сʱ����"<<endl;
            cin>>interval_hour;
            train_testStr="debug";
            break;
        }
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

    switch(train_test)
    {
    case 1:
    case 2:
    case 3:
        {
            stringstream ss;
            string outputFileName;
            ss<<OUTPUTADRESS<<dateSourceStr<<OUTPUTFILE<<"_"<<weightCpuTypeStr<<"_"<<train_testStr<<"_"<<interval_hour<<"hours.txt";
            ss>>outputFileName;
            MetaCpu *metaCpu=new MetaCpu(socialNet,isTrain,true);
            metaCpu->metaCpu(caseFileName,outputFileName);
            break;
        }
    case 9:
    case 10:
    case 11:
        {
            stringstream ss;
            string outputFileName;
            ss<<OUTPUTADRESS<<"friendREC_"<<dateSourceStr<<OUTPUTFILE<<"_"<<weightCpuTypeStr<<"_"<<train_testStr<<"_"<<interval_hour<<"hours.txt";
            ss>>outputFileName;
            MetaCpuForFriendRec *metaCpu=new MetaCpuForFriendRec(socialNet,isTrain,true);
            metaCpu->metaCpu(caseFileName,outputFileName,ITEMTYPE_USER);
            break;
        }
    case 4:
        {
            UCFRecommend * ucfr = new UCFRecommend(socialNet);
            ucfr->Recommend(caseFileName);
            break;
         }
    case 5:
        {
            FriBasedRecommend * friendeBasedRecommend = new FriBasedRecommend(socialNet);
            friendeBasedRecommend->Recommend(caseFileName);
            break;
        }
    case 6:
        {
            GeograRecommend * geoRec = new GeograRecommend(socialNet);
            geoRec->Recommend(caseFileName,false);
            break;
        }
    case 7:
        {
            GeograRecommend * geoRec = new GeograRecommend(socialNet);
            geoRec->Recommend(caseFileName,true);
            break;
        }
    case 8:
        {
            GeograRecommend * geoRec = new GeograRecommend(socialNet);
            geoRec->statistics("d:\\data\\output\\output_geoprobility.txt");
            break;
        }

    }
    int cc;
    cin>>cc;
}

/*
//#define INPUTADRESS "d:\\data\\input\\"
#define INPUTADRESS "d:\\checkin\\"
//���ɲ��Լ���
#define TRAIN_CHECKINDATA "traindata1.txt"
#define TRAIN_FRIENDDATA "friends_edges.txt"
#define TRAIN_CHECKTESTDATA "traindata2.txt"

//���ɲ��Լ���
#define TEST_CHECKINDATA "testdata1.txt"
#define TEST_FRIENDDATA "friends_edges.txt"
#define TEST_CHECKTESTDATA "testdata2.txt"

#define TEST_CHECKTESTDATANAME "testdata2"

#define OUTPUTADRESS "d:\\checkin\\"
#define OUTPUTFILE "featuredata"



//���ں����Ƽ�
#define FRIENDREC_CHECKINDATE "processedChekins.TXT"

//���ں����Ƽ���ѵ����
#define FRIENDREC_TRAINFRIENDDATE "friends_testdata1.TXT"
#define FRIENDREC_TRAINFRIENDDATE_2 "friends_testdata2.txt"
//���Լ�
#define FRIENDREC_TESTFRIENDDATE "friends_testdata1.TXT"
#define FRIENDREC_TESTFRIENDDATE_2 "friends_testdata2.txt"

#define FRIENDREC_OUTPUTFILE "friendrec_featuredata"

*/