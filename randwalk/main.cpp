#include"Edge.h"
#include"item.h"
#include<cstdio>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include <vector>
#include <map>
#include <set>
#include<ctime>
#include<iterator>
#include<cmath>
#include<math.h>

#define TIME_ITERVAL 24*60*60
#define OUTPUT_NEG_EQUAL 1//1��ʾ��������ͬ�ĸ���-train����0��ʾ���еĸ���-test��
#define MOD_FILE_WRITE 1//1��ʾ���д�ļ���0��ʾ������ԣ�ֻΪ��ͳ�����ݡ�


#define TOPN1 5
#define TOPN2 10
#define TOPN3 20
static  UserMap userlist;
static LocMap locationlist;

using std::cout;

int outputNegMod1;
int outputFileWriteMod1;

string checkinDataFileName;//ǩ�����ݣ����ڹ����罻���粢����Ԫ·������
string friendDataFileName;//���ѹ�ϵ���ݣ����ڹ����罻���粢����Ԫ·������
string testCheckinDataFileName;//ǩ�����ݣ����ڱȽϲ����������͸�����
string featureDataFileName;//��������������ݣ����ݸ�ʽΪ���û�ID��λ��ID������ֵ��������������label

int hasLLedge=1;//�Ƿ���L-L��
int weightCPUtype=0;//λ��-λ��Ȩֵ�ļ��㷽��



User* getUserPtrbyId(int id,int allownewtype){
    UserMap::iterator iter;
    iter =userlist.find(id);
    if(iter!=userlist.end()){
        return iter->second;
    }
    if(allownewtype==NOALLOWNEW) return NULL;
    User *newuser=new User(id);
    userlist.insert(UserMap::value_type(id,newuser));
    return newuser;
}
Location* getLocationPtrbyId(int id,int allownewtype)
{
    LocMap::iterator iter;
    iter =locationlist.find(id);
    if(iter!=locationlist.end()){
        return iter->second;
    }
    if(allownewtype==NOALLOWNEW) return NULL;
    Location *newloc=new Location(id);
    locationlist.insert(LocMap::value_type(id,newloc));
    return newloc;
}
Item * getItemPtrbyId(int id,int type,int allownewtype){
    if(type==TYPEUSER){
        return ::getUserPtrbyId(id,allownewtype);
    }else{
        return ::getLocationPtrbyId(id,allownewtype);
    }
}
Item * getItemPtrbyId(int id,char type,int allownewtype){
    if(type=='U'){
        return ::getUserPtrbyId(id,allownewtype);
    }else if(type='L'){
        return ::getLocationPtrbyId(id,allownewtype);
    }else{
        return NULL;
    }
}
bool isNeighbor(int fromid,int fromtype,int toid,int totype){
    Item * item;
    if(fromtype==TYPEUSER){
        item=::getUserPtrbyId(fromid,NOALLOWNEW);
    }else{
        item=::getLocationPtrbyId(fromid,NOALLOWNEW);
    }
    EdgeMap* emp=item->getToItemE(totype);
    EdgeMap::iterator iter=emp->find(toid);
    if(iter!=emp->end()){
        return true;
    }else{
       return false;
    }
}
bool isNeighbor(int fromid,char fromtype,int toid,char totype){
    int frominttype=TYPEUSER;
    if(fromtype=='L')
        frominttype=TYPELOCATION;
    int tointtype=TYPEUSER;
    if(totype=='L')
        tointtype=TYPELOCATION;
    return isNeighbor(fromid,frominttype,toid,tointtype);
}
void addFriend(int userFId,int userSId){
    User *fuser=::getUserPtrbyId(userFId,NOALLOWNEW);
    User *suser=::getUserPtrbyId(userSId,NOALLOWNEW);
    //cout<<"before add friends-------------------------"<<endl;
    if(fuser!=NULL&&suser!=NULL){
        //fuser->print("User");
        //suser->print("User");
        fuser->addToUserV(userSId);
        suser->addToUserV(userFId);
       // fuser->print("User");
        //suser->print("User");
    }
    else
    {
       // cout<<"NO USER"<<endl;
    }
    
}
vector<std::string> split(string str)
{
    char tmp[2];
    tmp[0]=9;
    tmp[1]='\0';
    string pattern(tmp);
     string::size_type pos;
     vector<string> result;
     str+=pattern;//��չ�ַ����Է������
     int size=str.size();
 
     for(int i=0; i<size; i++)
     {
         pos=str.find(pattern,i);
         if(pos<size)
         {
             string s=str.substr(i,pos-i);
             result.push_back(s);
             i=pos+pattern.size()-1;
         }
     }
     return result;
 }
void printMap(const map<int,float> &mapt ){
    for(map<int,float>::const_iterator iter=mapt.begin();iter!=mapt.end();++iter){
        cout<<iter->first<<"     "<<iter->second<<endl;
    }
}
//�������routeԪ·����idΪfromId��ITEM����һ��item������ֵMAP
FeatureMap calFeature(int fromId,const char *route,int length){
    FeatureMap result;
    Item *item1=::getItemPtrbyId(fromId,route[0],NOALLOWNEW);
    if(item1==NULL){
        cout<<"error:NO this Item!"<<endl;
        return result;
    }
    EdgeMap *edgelist1=item1->getToItemE(route[1]);
    int allweight1=item1->getAllWeight(route[1]);
    for(EdgeMap::const_iterator iter1=edgelist1->begin();iter1!=edgelist1->end();++iter1){
        int id2=iter1->first;
        float weight=1;
        if(iter1->second!=NULL){
            if(route[0]=='U'&&route[1]=='U'){
                weight=iter1->second->getCount();
            }else{
                weight=iter1->second->getWeight();
            }
        }
        float feat1=weight/allweight1;
        //cout<<feat1;
        Item * item2=::getItemPtrbyId(id2,route[1],NOALLOWNEW);
        EdgeMap *edgelist2=item2->getToItemE(route[2]);
        int allweight2=item2->getAllWeight(route[2]);

        for(EdgeMap::const_iterator iter2=edgelist2->begin();iter2!=edgelist2->end();++iter2){
            int id3=iter2->first;
            float weight=1;
            if(iter2->second!=NULL){
                if(route[1]=='U'&&route[2]=='U'){
                    weight=iter2->second->getCount();
                }else{
                    weight=iter2->second->getWeight();
                }
            }
            float feat2=weight/allweight2;
            if(length==2){
                if(!::isNeighbor(fromId,route[0],id3,route[2])){
                    //����ڵ��ԭ�ڵ�û�����ӣ���������֮���Ԫ·������ֵ
                    float feature=feat1*feat2;
                    FeatureMap::iterator fiter=result.find(id3);
                    if(fiter==result.end()){
                            result.insert(FeatureMap::value_type(id3,feature));
                    }
                    else{
                        fiter->second=feature+fiter->second;
                    }
                }
            }else{
                Item * item3=::getItemPtrbyId(id3,route[2],NOALLOWNEW);
                EdgeMap *edgelist3=item3->getToItemE(route[3]);
                int allweight3=item3->getAllWeight(route[3]);
                for(EdgeMap::const_iterator iter3=edgelist3->begin();iter3!=edgelist3->end();++iter3){
                    int id4=iter3->first;
                    float weight=1;
                    if(iter3->second!=NULL){
                         if(route[2]=='U'&&route[3]=='U'){
                             weight=iter3->second->getCount();
                          }else{
                            weight=iter3->second->getWeight();
                          }
                    }
                    float feat3=weight/allweight3;
                    if(!::isNeighbor(fromId,route[0],id4,route[3])){
                        float feature=feat1*feat2*feat3;
                        FeatureMap::iterator fiter=result.find(id4);
                        if(fiter==result.end()){
                            result.insert(FeatureMap::value_type(id4,feature));
                        }
                        else{
                            fiter->second=feature+fiter->second;
                        }
                    }
                }
            }
         }
    }
    return result;
}
/*
float getMetaPathFeatures(char *route,int fromid,vector<int> toid,int length){
    FeatureMap m=::calFeature(fromid,route,length);
    for(int i=0;i<toid.size();i++){
        FeatureMap::iterator iter=m.find(toid[i]);

    }
    
    if(iter!=m.end()){
        return iter->second;
    }
    return -1;
}*/
void readCheckinData(int testCount=-1){
    //����ǩ������
    ifstream checkinfile(checkinDataFileName);
    char buffer[100];
    int i=0;
    int lastUserId=0,lastLocId=0,lastTimeSec=0;//�����жϱȽϻ��λ��-λ�ñ�
    //�����û���ǩ����Ϣ�����û���λ�ýڵ���Ϣ�Լ��û�-λ�ú�λ��-λ�õı���Ϣ
    cout<<"��ȡǩ���ļ�"<<endl;
    int xxbug=0;
    while(xxbug<10000){
      //  xxbug++;
        i++;
        if(i%100000==0) cout<<i<<",";
        if(checkinfile.eof()){
            break;
        }
        if(testCount!=-1&&i>testCount){
            break;
        }
        checkinfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        //һ���ַ����ָ��ת��
        struct tm t; 
        t.tm_year=atoi(result[1].substr(0,4).data())-1900;
        t.tm_mon=atoi(result[1].substr(5,2).data()-1);
        t.tm_mday=atoi(result[1].substr(8,2).data());
        t.tm_hour=atoi(result[1].substr(11,2).data());
        t.tm_min=atoi(result[1].substr(13,2).data());
        t.tm_sec=atoi(result[1].substr(15,2).data());
        t.tm_isdst=0;
        time_t tsec;
        tsec=mktime(&t);
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());

        //cout<<"some ID________________________________________________"<<endl;
       // cout<<"userid:"<<userid<<endl;
        //cout<<"LocId:"<<locid<<endl;
        //cout<<"Time:"<<result[1]<<"time second:"<<tsec<<endl;

        //��������ҵ��û���λ�ýڵ㡣
        User * user=getUserPtrbyId(userid);
        Location * loc=::getLocationPtrbyId(locid);
        //cout<<"before dealing________________________________________________"<<endl;
        //user->print("user");
        //loc->print("location");

        //�����в����û�-λ�ñ�����
        user->addToLocV(locid,tsec);
        loc->addToUserV(userid);
        //����λ��-λ�ñ�����
        if(lastUserId==userid&&(lastTimeSec-tsec)<TIME_ITERVAL&&lastLocId!=locid&&lastLocId!=0){
            loc->addToLocV(lastLocId);
        }
        lastUserId=userid;
        lastTimeSec=tsec;
        lastLocId=locid;

        //cout<<"after dealing________________________________________________"<<endl;
        //user->print("user");
        //loc->print("location");
       // cout<<endl<<endl<<endl;
    }/*
    if(LTOLWEIGHTCPU){
        //ʹ���µı�Ȩֵ���㷽��
        for(LocMap::const_iterator iter=locationlist.begin();iter!=locationlist.end();++iter){
            Location * loc=
        }

    }*/
    cout<<endl;
   // cout<<"\ncount:"<<userlist.size()<<","<<locationlist.size()<<endl;
    //cout<<endl<<endl<<endl;cout<<endl<<endl<<endl;
}
void readFriendData(int testCount=-1){
    //���û���ϵ����
    ifstream friendfile(friendDataFileName);
    char buffer[100];
    int i=0;
    cout<<"��ȡ���ѹ�ϵ�ļ�"<<endl;
    int xxbug=0;
    while(xxbug<10000){
       // xxbug++;
        i++;
        if(i%100000==0) cout<<i<<",";
        if(friendfile.eof()){
            break;
        }
        if(testCount!=-1&&i>testCount){
            break;
        }
        friendfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=2){
            break;
        }
        int fuid=atoi(result[0].data());
        int suid=atoi(result[1].data());
        addFriend(fuid,suid);
    }
    cout<<endl;
}
//�ҵ�����Ϊcount�ĸ���,��count=-1�������еĸ���������
int addNegativeCase(int fromid,FeatureMap * metaFea,ofstream &outFile,int count,int outputNegMod,int outputFileWriteMod,int featureCount){
    if(count==0) return 0;
    if(outputNegMod!=OUTPUT_NEG_EQUAL){
        count=-1;
    }
    int negcount=0;//ѡ��ĸ�������
    int trueNegCount=0;//ԭʼ�ĸ�������
    bool flag=true;//�Ƿ�Ҫ������Ӹ���
    //ofstream outFile=outFilePtr;
    for(int i=0;i<featureCount;i++){
        float feat[5];
        if(i>0){
            for(int j=0;j<i;j++){
                feat[j]=0;
            }
        }
        FeatureMap *meF=&metaFea[i];
        for(FeatureMap::iterator iter=meF->begin();iter!=meF->end();++iter){
            int toid=iter->first;
            feat[i]=iter->second;
            if(i!=featureCount-1){
                for(int j=i+1;j<featureCount;j++){
                    FeatureMap* meF2=&metaFea[j];
                    FeatureMap::iterator iter2=meF2->find(toid);
                    if(iter2!=meF2->end()){
                        feat[j]=iter2->second;
                        meF2->erase(iter2);
                    }else{
                        feat[j]=0;
                    }
                }
            }
            trueNegCount++;
            if(count==-1||(flag&&(rand()%100)==1)){
                if(outputFileWriteMod==MOD_FILE_WRITE){
                    outFile<<fromid<<","<<toid<<",";
                    for(int x=0;x<featureCount;x++){
                        outFile<<feat[x]<<",";
                    }
                    outFile<<"0"<<endl;
                }
                negcount++;
                if(negcount==count){
                    flag=false;
                }

            }
        }
    }
    return trueNegCount;
}
//��ȡ��һ�׶ε����ݣ�����Ԫ·������ֵ���������������������䱣�����ļ��С�
//outputNegMod: 1��ʾ��������ͬ�ĸ���-train����0��ʾ���еĸ���-test��
//outputFileWriteMod:1��ʾ���д�ļ���0��ʾ������ԣ�ֻΪ��ͳ������
void readTestData(int outputNegMod,int outputFileWriteMod){
    int featureCount=5;
    if(!hasLLedge){
        featureCount=3;
    }
    ifstream testfile(testCheckinDataFileName);
    ofstream outfile(featureDataFileName,ios::out);
    int userCount=0;
    char buffer[100];
    int lastuserid=-1;
    //���ÿһ��Դ�ڵ㣬ͨ��Ԫ·������һ��Ŀ�Ľڵ㣬Դ�ڵ��Ŀ�Ľڵ�ֱ�ӿ��Լ���Ԫ·������ֵ��
    //��i��Ԫ·�������ļ���Ľ������metaFeature[i]�У���ʽΪ<Ŀ�Ľڵ�id������ֵ>
    FeatureMap metaFeature[5];
    char route0[4]="UUL";
    char route1[5]="UUUL";
    char route2[5]="ULUL";
    char route3[4]="ULL";
    char route4[5]="ULLL";

    int allCount=0;//���е�ǩ����¼����
    int metaPathInCount[5];//����Ԫ·�������ǩ������
    for(int i=0;i<featureCount;i++) metaPathInCount[i]=0;
    int reCount=0;//��train�׶��ظ���ǩ������
    int testReCount=0;//��test�׶��Ѿ�������������ǩ������
    int noCount=0;//������Ԫ·���ɵ�����λ�õ�ǩ������
    int nullCount=0;//�û�����λ�ýڵ㲻����ԭʼ���ݼ���ǩ������

    int positiveCount=0;//��������
    int negativeCount=0;//�������ݼ���������
    int trueNegCount=0;//��ʵ��ȫ����������

    int i=0;
    std::cout<<"��ȡtest�ļ�"<<endl;
    int lastPosCount=0;//��һ��userid������

    //�û��������λ�ü���:
    set<int> uLocSet;
    int xbug=0;
    while(xbug<100){
        //xbug++;
        //��ȡ�ļ��е��µ�һ�У������д������û�id��λ��id
        i++;
        //if(i==1000) break;//just for test
        if(i%10000==0) cout<<i<<",";
        if(testfile.eof()){
            break;
        }
        allCount++;
        testfile.getline(buffer,100);
        string str(buffer);
        vector<std::string> result=split(str);
        if(result.size()!=5){
            break;
        }
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        User * u=::getUserPtrbyId(userid,NOALLOWNEW);
        Location * l=::getLocationPtrbyId(locid,NOALLOWNEW);

        if(u==NULL||l==NULL){//����û�����λ�ò�����
            nullCount++;
            continue;
        }


        //��������û�id��һ���µ��û����ȶԾɵ��û����д�����Ӹ�������Ȼ���ڼ����µ��û���Ԫ·���ܵ����λ��
        if(userid!=lastuserid){
            userCount++;
            uLocSet.clear();//����û����ʵ�λ�ü���
            //����һ��User�е�ʣ��ǩ����Ϊ����
            if(lastuserid!=-1){
                negativeCount+=lastPosCount;//��Ӻ�����������ͬ�ĸ���
                if(lastPosCount>=20||outputNegMod==OUTPUT_NEG_EQUAL){
                    //���������������20�����������������ͬ�ĸ������Ž��з���
                    trueNegCount+=::addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount,outputNegMod, outputFileWriteMod,featureCount);
                }
            }

            //����һ���µ�userid���ҵ�����û�ͨ��Ԫ·�����������λ�ã���������û�-λ�õ�Ԫ·������ֵ
            if(hasLLedge){
                metaFeature[0]=::calFeature(userid,route0,2);
                metaFeature[1]=::calFeature(userid,route1,3);
                metaFeature[2]=::calFeature(userid,route2,3);
                metaFeature[3]=::calFeature(userid,route3,2);
                metaFeature[4]=::calFeature(userid,route4,3);
            }else{
                metaFeature[0]=::calFeature(userid,route0,2);
                metaFeature[1]=::calFeature(userid,route1,3);
                metaFeature[2]=::calFeature(userid,route2,3);
            }
            lastuserid=userid;
            lastPosCount=0;
        }
        ostream_iterator<int> output(std::cout,"");
        //std::os
        //cout<<"uLocSet:";
       
        //std::copy(uLocSet.begin(),uLocSet.end(),output);
        //cout<<endl;
        EdgeMap *emp=u->getToLocE();
        if(emp->find(locid)!=emp->end()){//����û�train�׶��Ѿ����ʹ���λ��
            reCount++;
            continue;
        }
        set<int>::const_iterator resIter=uLocSet.find(locid);
        if(resIter!=uLocSet.end()){//�û���test�׶��Ѿ����ʹ�
        //�������û���test�׶εڶ��η���ĳһλ��ʱ����������������
            testReCount++;
            continue;
        }
        int flag=0;
        FeatureMap::iterator miter;
        float tmpFeature[5];
        for(int i=0;i<featureCount;i++){//���������map����ֱ���lid
            miter=metaFeature[i].find(locid);
            if(miter!=metaFeature[i].end()){//�ҵ�һ����˵��ͨ����Ԫ·�����Ե����λ�ã����Ԫ·��������Ϊ�û�-λ�öԵ�һ������
                tmpFeature[i]=miter->second;
                metaPathInCount[i]++;
                flag=1;
                metaFeature[i].erase(miter);//������λ�ô�����MAP����ɾ�����Է�ֹ����ѡ��ʱѡ������
            }else{//��Ԫ·���޷����û������λ��
                 tmpFeature[i]=0;
            }
        }
        if(flag==0) noCount++;//�������κ�Ԫ·�����Է��ʵõ���λ��
        else{//���һ������
            uLocSet.insert(locid);
            if(outputFileWriteMod==MOD_FILE_WRITE){
                outfile<<userid<<","<<locid;
                for(int i=0;i<featureCount;i++)
                    outfile<<","<<tmpFeature[i];
                outfile<<",1"<<endl;
            }
            positiveCount++;
            lastPosCount++;
        }
    }
    cout<<endl;
    //������һ��Userid�ĸ�������
    negativeCount+=lastPosCount;
    if(lastPosCount>=20||outputNegMod==OUTPUT_NEG_EQUAL)
        trueNegCount+=::addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount,outputNegMod, outputFileWriteMod,featureCount);

    //������ֲ���
    cout<<"�������ݼ��г��ֵ����е��û�����userCount��"<<userCount<<endl;
    cout<<"���е�ǩ����¼����AllCount:"<<allCount<<endl;
    for(int i=0;i<featureCount;i++)
        cout<<"����Ԫ·�������ǩ������metaPathInCount["<<i<<"]:"<<metaPathInCount[i]<<endl;

    cout<<"�ظ���ǩ������ReCount:"<<reCount<<endl;
    cout<<"��test�׶��ظ���ǩ������TestReCount:"<<testReCount<<endl;
    cout<<"������Ԫ·���ɵ�����λ�õ�ǩ������noCount:"<<noCount<<endl;
    cout<<"�û�����λ�ýڵ㲻����ԭʼ���ݼ���ǩ������nullCount"<<nullCount<<endl;
    cout<<"��������positiveCount��"<<positiveCount<<endl;
    cout<<"ѡ��������������negativeCount��"<<negativeCount<<endl;
    cout<<"��������trueNeCount��"<<trueNegCount<<endl;
    outfile.close();
}
void calculateWik(map<int,float> & wikMap,int userid){
    User * u=::getUserPtrbyId(userid,NOALLOWNEW);
    //����ÿһ���û��������û������ƶ�Wi,k
    //��ʱ�����������һ��int��ʾk���ڶ���int��ʾ��Ӧ��ֵ
    map<int,int> cijckjMap;
    int cij2=0;//���ڵ�ǰ���û����й̶���cij2
    //Wik=cijckj/��sqrt��cij2��*sqrt��ckj2����
    EdgeMap *locEdgeList=u->getToLocE();
    //�����û�u��˵���ҵ��û������з��ʹ���λ�ã�Ȼ���ҵ����ʹ���Щλ�õ��û�
    for(EdgeMap::const_iterator liter=locEdgeList->begin();liter!=locEdgeList->end();++liter){
        int lid=liter->first;
        int cij=liter->second->getWeight();
        cij2+=cij*cij;
        Location * loc=::getLocationPtrbyId(lid,NOALLOWNEW);
        EdgeMap * userEdgeList=loc->getToUserE();
        //����cijckj��һ������
        for(EdgeMap::const_iterator uiter=userEdgeList->begin();uiter!=userEdgeList->end();++uiter){
            int uid=uiter->first;
            if(uid==userid) continue;
            int ckj=uiter->second->getWeight();
            int cijckj=cij*ckj;
            if(wikMap.find(uid)==wikMap.end()){
                wikMap.insert(map<int,float>::value_type(uid,0));
                cijckjMap.insert(map<int,int>::value_type(uid,cijckj));
            }else{
                cijckjMap[uid]+=cijckj;
            }
        }
    }
    //�����û�k��ckj2��
    for(map<int,float>::const_iterator witer=wikMap.begin();witer!=wikMap.end();++witer){
        int toUId=witer->first;
        int ckj2=0;
        User *toUser=::getUserPtrbyId(toUId,NOALLOWNEW);
        EdgeMap * locList=toUser->getToLocE();
        for(EdgeMap::const_iterator edgIter=locList->begin();edgIter!=locList->end();++edgIter){
            ckj2+=edgIter->second->getWeight()*edgIter->second->getWeight();
        }
        wikMap[toUId]=cijckjMap[toUId]/(sqrt((float)cij2)*sqrt((float)ckj2));
    }
}

void calRecResult(map<int,float>&recResult,const map<int,float> & u2uWeightMap,int uid){
    int ss=0;
    User * user=::getUserPtrbyId(uid,NOALLOWNEW);
    int count=0;
    /*
    for(map<int,float>::const_iterator witer=u2uWeightMap.begin();witer!=u2uWeightMap.end();witer++){
    //ͳ��Ϊ���û�Ҫ�����λ�õ�����
        int toUserId=witer->first;
        if(toUserId==uid) continue;
        User * toUser=::getUserPtrbyId(toUserId,NOALLOWNEW);
        EdgeMap *locedgelist=toUser->getToLocE();
        count+=locedgelist->size();
       
    }*/
    //cout<<u2uWeightMap.size()<<"allsize:"<<count<<endl;
    //��ʽ��ʼ���㣡��
    for(map<int,float>::const_iterator witer=u2uWeightMap.begin();witer!=u2uWeightMap.end();witer++){
        //����ÿһ�����û�i��������ֵ���û����ʹ���λ��L1������Щλ���Ƽ����û�i
        //�����û�uid�������û��ķ���λ�ü���~
        int toUserId=witer->first;
        if(toUserId==uid) continue;
        User * toUser=::getUserPtrbyId(toUserId,NOALLOWNEW);
        EdgeMap *locedgelist=toUser->getToLocE();
        for(EdgeMap::const_iterator editer=locedgelist->begin();editer!=locedgelist->end();++editer){
        //�����û�i��λ��toLId����������
            ss++;
            int toLId=editer->first;
            if(user->getToLocE()->find(toLId)!=user->getToLocE()->end()) continue;//����û�i�������ʹ���λ�ã����Ƽ���λ��
            if(recResult.find(toLId)!=recResult.end()) {
               // cout<<"��Location�Ѿ��������"<<endl;
                continue;//��Location�Ѿ��������
            }
            //������������
            //Location * toLoc=::getLocationPtrbyId(toLId,NOALLOWNEW);
            float wikckjA=0;
            float wikA=0;

            for(map<int,float>::const_iterator u2uWiter=u2uWeightMap.begin();u2uWiter!=u2uWeightMap.end();u2uWiter++){
            //�ҵ����к��û�i���Ƶ��û���Ȼ��ʹ�ù�ʽ����
                int tmpUid=u2uWiter->first;
                User* tmpUser=::getUserPtrbyId(tmpUid,NOALLOWNEW);
                if(tmpUser->getToLocE()->find(toLId)!=tmpUser->getToLocE()->end()){
                    //����û�tmpUid���ʹ���λ��toLId��wikckj�ӽ���������wikckjԪ��Ϊ0��
                    wikckjA+=u2uWiter->second*(tmpUser->getToLocE()->find(toLId)->second->getWeight());
                }
                wikA+=u2uWiter->second;
            }
            if(wikA!=0)
                recResult.insert(map<int,float>::value_type(toLId,wikckjA/wikA));
            else
                cout<<"err:wik-ALL is 0"<<endl;
            
            /*if(ss%20000==0){
                cout<<"recResultSize:"<<recResult.size();
            }*/
        }
    }
}
void sortRecResult(map<int,float> &sortedRec,const map<int,float> &recResult,int N=20){
    int leastUserId=-1;
    float leastRanked=INT_MAX;
    int sortedCount=0;
    for(map<int,float>::const_iterator recIter=recResult.begin();recIter!=recResult.end();++recIter){
        if(sortedCount<N){
            sortedRec.insert(map<int,float>::value_type(recIter->first,recIter->second));
            sortedCount++;
            if(sortedCount==N){
                //K�����ˣ��ʹ洢��С��Ԫ��
                for(map<int,float>::const_iterator sorIter=sortedRec.begin();sorIter!=sortedRec.end();++sorIter){
                    if(sorIter->second<leastRanked){
                        leastRanked=sorIter->second;
                        leastUserId=sorIter->first;
                    }
                }
            }
        }else{
           //20�����ˣ��Ƚϵ�ǰԪ�غ����������СԪ�أ����������ɾ����СԪ�أ�����ǰԪ�ز��룬Ȼ��������СԪ��
            if(recIter->second>leastRanked){
                sortedRec.erase(leastUserId);
                sortedRec.insert(map<int,float>::value_type(recIter->first,recIter->second));
                //������СԪ��
                leastRanked=INT_MAX;
                for(map<int,float>::const_iterator sorIter=sortedRec.begin();sorIter!=sortedRec.end();++sorIter){
                    if(sorIter->second<leastRanked){
                        leastRanked=sorIter->second;
                        leastUserId=sorIter->first;
                    }
                }
            }
        }
    }
}

//�����û�ID��������û�����������֮������Ѷȣ����ѵ����̶ܳȣ�
void calculateSIik(map<int,float>& f2fWeightMap,int userid){
    const float para=0.5;//�Զ���Ȩֵ

    User * u=::getUserPtrbyId(userid,NOALLOWNEW);
    //����ÿһ���û������ĺ��ѵĺ��Ѷ�SIk,i
    EdgeMap * frdsEdgeList=u->getToUserE();
    for(EdgeMap::const_iterator fliter=frdsEdgeList->begin();fliter!=frdsEdgeList->end();++fliter){
        int coFrdCount=0;
        int allFrdCount=0;
        int coVisitedCount=0;
        int allVisitedCount=0;
        int fid=fliter->first;
        User * frdUser=::getUserPtrbyId(fid);

        //ͳ�����û���ͬ�����Ѻ�ȫ������������
        EdgeMap * frdsOfFrdEdgeList=frdUser->getToUserE();
        for(EdgeMap::const_iterator FOfFLiter=frdsOfFrdEdgeList->begin();FOfFLiter!=frdsOfFrdEdgeList->end();++FOfFLiter){
            if(frdsEdgeList->find(FOfFLiter->first)!=frdsEdgeList->end()){
                coFrdCount++;
            }
        }
        allFrdCount=frdsEdgeList->size()+frdsOfFrdEdgeList->size()-coFrdCount;

        //ͳ�����û���ͬ���ʵ�λ�ú�ȫ����λ������
        EdgeMap * locEdgeList=u->getToLocE();//���û����ʹ���λ��
        EdgeMap * locOfFrdEdgeList=frdUser->getToLocE();
        for(EdgeMap::const_iterator LOfFLiter=locOfFrdEdgeList->begin();LOfFLiter!=locOfFrdEdgeList->end();++LOfFLiter){
            if(locEdgeList->find(LOfFLiter->first)!=locEdgeList->end()){
                coVisitedCount++;
            }
        }
        allVisitedCount=locEdgeList->size()+locOfFrdEdgeList->size()-coVisitedCount;

        //������������map��
        float frty=para*coFrdCount/allFrdCount+(1-para)*coVisitedCount/allVisitedCount;
        f2fWeightMap.insert(map<int,float>::value_type(fid,frty));
    }
}
void FriendBasedRecommend(){
    /*
   int index=0;
    cout<<"������1-12������ѡ���Ӧ�������ļ�"<<endl;
    cin>>index;
    string fileName;
    stringstream ss;
    ss<<CHECKTESTDATA<<index<<".txt";
    ss>>fileName;
    cout<<fileName<<endl;
    ifstream testfile(fileName);*/
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
        User * user=::getUserPtrbyId(userid,NOALLOWNEW);
        Location * loc=::getLocationPtrbyId(locid,NOALLOWNEW);
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
                cout<<" ��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
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
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
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
void UCFRecommend(){
    int index=0;
    cout<<"������1-12������ѡ���Ӧ�������ļ�"<<endl;
    cin>>index;
    string fileName;
    stringstream ss;
    ss<<checkinDataFileName<<index<<".txt";
    ss>>fileName;
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
        User * user=::getUserPtrbyId(userid,NOALLOWNEW);
        Location * loc=::getLocationPtrbyId(locid,NOALLOWNEW);
        //����ǩ����¼���

        if(user==NULL||loc==NULL){//����û�����λ�ò�����
            continue;
        }
        if(userid!=lastUserId){//�����һ���µ��û���Ϊ���Ƽ�TOP-K�ĵص�
            if(allRec[0]!=0){
                cout<<"top-5:"<<endl;
                //printMap(sortedRec1);
                cout<<" ��ȷ��rightRec[0]/allRec[0]��"<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"�ٻ���rightRec[0]/rightCase"<<rightRec<<"/"<<rightCase<<" :"<<(float)rightRec[0]/rightCase<<endl;
                cout<<"top-10:"<<endl;
                //printMap(sortedRec2);
                cout<<" ��ȷ��rightRec[1]/allRec[1]��"<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[1]<<"/"<<rightCase<<" :"<<(float)rightRec[1]/rightCase<<endl;
                cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
                cout<<" ��ȷ��rightRec[2]/allRec[2]��"<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"�ٻ���rightRec[1]/rightCase"<<rightRec[2]<<"/"<<rightCase<<" :"<<(float)rightRec[2]/rightCase<<endl;
            }
            //�������һЩ����
            u2uWeightMap.clear();
            recResult.clear();
            sortedRec1.clear();
            sortedRec2.clear();
            sortedRec3.clear();
            locIdSet.clear();
           //����ÿһ���û��������û������ƶ�Wi,k,����������u2uWeightMap
            calculateWik(u2uWeightMap,userid);
            cout<<fileName<<endl;
            cout<<"�û���"<<userid<<endl;

            //cout<<"�û�-�û�Ȩֵ�����û���Ȩֵ��"<<userid<<endl;
           // printMap(u2uWeightMap);
            
            //�����û���δ���۵�ITEM���������ۣ��Դ���Ϊ�Ƽ����
            calRecResult(recResult,u2uWeightMap,userid);
            //cout<<"�Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            //printMap(recResult);

            //ѡ��top-k���Ƽ����
            sortRecResult(sortedRec1,recResult,TOPN1);
            sortRecResult(sortedRec2,recResult,TOPN2);
            sortRecResult(sortedRec3,recResult,TOPN3);
            //cout<<"�������Ƽ��������λ��ID���Ƽ��ȣ�"<<userid<<endl;
            //printMap(sortedRec);
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
void main()
{
    int outputNegMod;
    cout<<"11-ѵ���������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ��������"<<endl;
    cout<<"12-���Լ������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ��������"<<endl;
    cout<<"21-ѵ���������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊlog(����)����"<<endl;
    cout<<"22-���Լ������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊlog(����)����"<<endl;
    cout<<"31-ѵ���������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ0��1����"<<endl;
    cout<<"32-���Լ������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ0��1����"<<endl;
    cout<<"41-ѵ���������ļ������������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ��������"<<endl;
    cout<<"42-���Լ������ļ������������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ��������"<<endl;
    int inparam;
    cin>>inparam;
    if(inparam==11){
        checkinDataFileName=TRAIN_CHECKINDATA;//ǩ�����ݣ����ڹ����罻���粢����Ԫ·������
        friendDataFileName=TRAIN_FRIENDDATA;//���ѹ�ϵ���ݣ����ڹ����罻���粢����Ԫ·������
        testCheckinDataFileName=TRAIN_CHECKTESTDATA;//ǩ�����ݣ����ڱȽϲ����������͸�����
        featureDataFileName=TRAIN_FEATUREDATA;//��������������ݣ����ݸ�ʽΪ���û�ID��λ��ID������ֵ��������������label
        hasLLedge=true;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTRAIN;
    }else if(inparam==12){
        //12-���Լ������ļ����������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ��������
        checkinDataFileName=TEST_CHECKINDATA;
        friendDataFileName=TEST_FRIENDDATA;
        testCheckinDataFileName=TEST_CHECKTESTDATA;
        featureDataFileName=TEST_FEATUREDATA;
        hasLLedge=false;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTEST;
    }else if(inparam==41){
        //ѵ���������ļ������������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ������
        checkinDataFileName=TRAIN_CHECKINDATA;
        friendDataFileName=TRAIN_FRIENDDATA;
        testCheckinDataFileName=TRAIN_CHECKTESTDATA;
        featureDataFileName=TRAIN_FEATUREDATANOLLEDGE;
        hasLLedge=false;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTRAIN;
    }else if(inparam==42){
        //ѵ���������ļ������������L-L�ߣ��ߵ�Ȩֱֵ��Ϊ������
        checkinDataFileName=TEST_CHECKINDATA;
        friendDataFileName=TEST_FRIENDDATA;
        testCheckinDataFileName=TEST_CHECKTESTDATA;
        featureDataFileName=TEST_FEATUREDATANOLLEDGE;
        hasLLedge=false;
        weightCPUtype=CPU_COUNT;
        outputNegMod=NEGMOD_ALL_FORTEST;
    }else{
        cout<<"ERROR INPUT!"<<endl;

    }
    cout<<"Begin:"<<endl;
    cout<<"CHECKINDATA"<<checkinDataFileName<<endl;
    cout<<"FRIENDDATA"<<friendDataFileName<<endl;
    cout<<"CHECKTESTDATA"<<testCheckinDataFileName<<endl;
    cout<<"FEATUREDATA"<<featureDataFileName<<endl;
    cout<<"hasLLedge"<<hasLLedge<<endl;
    readCheckinData();
    readFriendData();

    cout<<"��ѡ���Ƽ�������"<<endl;
    cout<<"1-�����û���Эͬ����"<<endl;
    cout<<"2-���ں��ѵ�Эͬ����"<<endl;
    cout<<"3-������·Ԥ���λ���Ƽ���������Ϊѵ��������Լ�"<<endl;

    int method=0;
    cin>>method;
    if(method==1){
        //�����û���Эͬ���˵�λ���Ƽ�
        UCFRecommend();
    }else if(method==2){
        //���ں��ѵ�λ���Ƽ�
        FriendBasedRecommend();
    }else if(method==3){
        //������·Ԥ���λ���Ƽ�
        
        int outputFileWriteMod;
        cout<<"outputFileWriteMod:1��ʾ���д�ļ���0��ʾ������ԣ�ֻΪ��ͳ�����ݡ�"<<endl;
        cin>>outputFileWriteMod;

        ::readTestData(outputNegMod,outputFileWriteMod);
        cout<<"���ݼ��е������û�������"<<::userlist.size()<<endl;
        cout<<"���ݼ������е�λ��������"<<::locationlist.size()<<endl;
    }
    int xx;
    cin>>xx;
}
