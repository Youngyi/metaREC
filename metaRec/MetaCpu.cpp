#include "MetaCpu.h"
#include<fstream>
#include <iostream>
#include<sstream>
#include<iterator>
#include "Edge.h"
extern int isDebug;
MetaCpu::MetaCpu(Socialnet *socialNet,bool outputNegEqualPos,bool outputFileWrite):outputFileWrite(outputFileWrite),outputNegEqualPos(outputNegEqualPos){
    this->socialNet=socialNet;
    featureCount=6;
    if (!socialNet->hasLLRelation) featureCount=3;
}

//��ȡ��һ�׶ε����ݣ�����Ԫ·������ֵ���������������������䱣�����ļ��С�
void MetaCpu::metaCpu(string testDateFileName,string outputFileName){
    FeatureMap metaFeature[6];
    int route0[3]={ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_LOCATION};//UUL
    int route1[4]={ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_LOCATION};//UUUL
    int route2[4]={ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_USER,ITEMTYPE_LOCATION};//"ULUL";
    int route3[3]={ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION};//"ULL";
    int route4[4]={ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION};//"ULLL";
    int route5[4]={ITEMTYPE_USER,ITEMTYPE_USER,ITEMTYPE_LOCATION,ITEMTYPE_LOCATION};//"UULL";

    ifstream testfile(testDateFileName);
    ofstream outfile(outputFileName,ios::out);
   

    //ͳ����Ϣ
    int userCount=0;
    int allCount=0;//���е�ǩ����¼����
    int metaPathInCount[6];//����Ԫ·�������ǩ������
    for(int i=0;i<featureCount;i++) metaPathInCount[i]=0;
    int reCount=0;//��train�׶��ظ���ǩ������
    int testReCount=0;//��test�׶��Ѿ�������������ǩ������
    int noCount=0;//������Ԫ·���ɵ�����λ�õ�ǩ������
    int nullCount=0;//�û�����λ�ýڵ㲻����ԭʼ���ݼ���ǩ������

    int positiveCount=0;//��������
    int negativeCount=0;//�������ݼ���������
    int trueNegCount=0;//��ʵ��ȫ����������


    cout<<"��ȡ�Ƚ��ļ�������������"<<testDateFileName<<endl;
    cout<<"���д���ļ���"<<outputFileName<<endl;
    char buffer[100];
    int lastuserid=-1;
    int lastPosCount=0;//��һ��userid������
    set<int> uLocSet;//�û��������λ�ü���
    string postiveStr="";//�洢��Ҫд���ļ��������ַ����������ɲ��Լ��׶Σ������������20���Ͳ�д�룬Ҳ�����ɸ�����
    while (true)
    {
        allCount++;
        if(allCount%10000==0) cout<<allCount<<",";
        if(testfile.eof()){
            break;
        }
        
        //�Լ�¼���г�������
        testfile.getline(buffer,100);
        string str(buffer);
        if (isDebug)
        {
            cout<<"�����¼��"<<endl;
            cout<<str<<endl;
        }
        vector<std::string> result=split(str);
        if(result.size()!=5){
            break;
        }
        int userid=atoi(result[0].data());
        int locid=atoi(result[4].data());
        Item * user=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
        Item * location=socialNet->getItemPtrById(locid,ITEMTYPE_LOCATION);
        if(user==NULL||location==NULL){//����û�����λ�ò�����
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
                if(lastPosCount>=20||outputNegEqualPos){
                    //���������������20�����������������ͬ�ĸ������Ž��з���
                    if(isDebug){
                        cout<<"����д���ļ���*****"<<endl;
                        cout<<postiveStr<<"*********************"<<endl;
                    }
                    outfile<<postiveStr;
                    trueNegCount+=addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount);

               }
            }

            //����һ���µ�userid���ҵ�����û�ͨ��Ԫ·�����������λ�ã���������û�-λ�õ�Ԫ·������ֵ
            metaFeature[0]=calFeature(userid,route0,2);
            metaFeature[1]=calFeature(userid,route1,3);
            metaFeature[2]=calFeature(userid,route2,3);
            metaFeature[3]=calFeature(userid,route3,2);
            metaFeature[4]=calFeature(userid,route4,3);
            metaFeature[5]=calFeature(userid,route5,3);
            if (isDebug) printFeatureMap(metaFeature);
            lastuserid=userid;
            lastPosCount=0;
             postiveStr="";
        }


        //���ݵ�ǰ��¼��user,location�����Լ���õĸ��û�������ֵ���ϣ��������һ������
        int reFlag=addAPositiveCase(user,location,metaFeature,&uLocSet,metaPathInCount,postiveStr);
        if (reFlag==-2)
        {
            if (isDebug) cout<<"��һ�׶Σ�����LBSN���磩�Ѿ����ʹ���λ��"<<endl;
            reCount++;
        }else if (reFlag==-1)
        {
            if (isDebug) cout<<"��һ�׶Σ��������������Ѿ����ʹ���λ��"<<endl;
            testReCount++;
        }else if (reFlag==-3)
        {
            if (isDebug) cout<<"��λ�ò������κ�Ԫ·��"<<endl;
            noCount++;
        }else if (reFlag==1)
        {
            lastPosCount++;
            positiveCount++;
        }
    }

    //������һ��Userid�ĸ�������
    negativeCount+=lastPosCount;
    if(lastPosCount>=20||outputNegEqualPos){
        //���������������20�����������������ͬ�ĸ������Ž��з���
        if(isDebug){
            cout<<"����д���ļ���*****"<<endl;
            cout<<postiveStr<<"*********************"<<endl;
        }
        outfile<<postiveStr;
        trueNegCount+=addNegativeCase(lastuserid,metaFeature,outfile,lastPosCount);

    }

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


int MetaCpu::addAPositiveCase(
    //���ݵ�ǰ��¼��user,location�����Լ���õĸ��û�������ֵ���ϣ��������һ������
    Item * user,
    Item * location,
    FeatureMap * metaFea,
    set<int> *uLocSet,
    int *metaPathInCount,
    string & postiveStr////�洢��Ҫд���ļ��������ַ����������ɲ��Լ��׶Σ������������20���Ͳ�д�룬Ҳ�����ɸ�����
    ){
        EdgeMap *userEdgeList=user->getToLocE();
        if(userEdgeList->find(location->getId())!=userEdgeList->end()){//����û��ڹ����罻����׶��Ѿ����ʹ���λ��
            return -2;
        }
        if(uLocSet->find(location->getId())!=uLocSet->end()){//�û����������ɽ׶��Ѿ����ʹ�//�������û����������ɽ׶εڶ��η���ĳһλ��ʱ����������������
            return -1;
        }
        int flag=0;
        FeatureMap::iterator miter;

        float tmpFeature[6];
        for(int i=0;i<featureCount;i++){//����������map����ֱ���lid
            miter=metaFea[i].find(location->getId());
            if(miter!=metaFea[i].end()){
                //�ҵ�һ����˵��ͨ����Ԫ·�����Ե����λ�ã����Ԫ·��������Ϊ�û�-λ�öԵ�һ������
                tmpFeature[i]=miter->second;
                metaPathInCount[i]++;
                flag=1;
                metaFea[i].erase(miter);//������λ�ô�����MAP����ɾ�����Է�ֹ����ѡ��ʱѡ������
            }else{//��Ԫ·���޷����û������λ��
                tmpFeature[i]=0;
            }
        }
        if(flag==0) return -3;//�������κ�Ԫ·�����Է��ʵõ���λ��
        else{//���һ������
            int locid=location->getId();
            uLocSet->insert(locid);
            if(outputFileWrite){
                stringstream ss;
                string tmp;
                ss<<user->getId()<<","<<locid;
                for(int i=0;i<featureCount;i++)
                    ss<<","<<tmpFeature[i];
                ss<<",1";
                ss>>tmp;
                postiveStr.append(tmp);
                postiveStr.append("\n");
                if (isDebug)
                {
                    cout<<"����������"<<endl;
                    cout<<user->getId()<<","<<locid;
                    for(int i=0;i<featureCount;i++)
                        cout<<","<<tmpFeature[i];
                    cout<<",1"<<endl;
                }
            }
           return 1;
        }
}

//�������routeԪ·����idΪfromId��ITEM������item������Ԫ·�����Ե����ITEM��������ֵMAP
FeatureMap MetaCpu::calFeature(int fromId,const int *route,int length){
    FeatureMap result;
    Item *item1=socialNet->getItemPtrById(fromId,route[0]);
    if(item1==NULL) return result;

    string kongbai="    ";
    if (isDebug)  {
        cout<<"��ʼ����һ��Item��һ��Ԫ·������***************"<<endl;
        cout<<"�����ʼ�ڵ�ΪID��item������Item������ֵ"<<endl<<"��ʼ�ڵ㣺"<<endl;item1->print(route[0]);
    }

    EdgeMap *edgelist1=item1->getToItemE(route[1]);
    float allweight1=item1->getAllWeight(route[1]);

    for(EdgeMap::const_iterator iter1=edgelist1->begin();iter1!=edgelist1->end();++iter1){
        int id2=iter1->first;
        float weight1;
        if(iter1->second!=NULL){
            weight1=iter1->second->getWeight();
            if (weight1==0) continue;
        }
        float feat1=weight1/allweight1;
        
        Item * item2=socialNet->getItemPtrById(id2,route[1]);
        if (item2==NULL) continue;

        if (isDebug)
        {
            cout<<kongbai<<"��ʼ�ڵ㵽�ڶ����ڵ㣺"<<weight1<<"/"<<allweight1<<"="<<feat1<<endl;
            cout<<kongbai<<"�ڶ����ڵ㣺"<<endl;
            item2->print(route[1],1);

        }

        EdgeMap *edgelist2=item2->getToItemE(route[2]);
        float allweight2=item2->getAllWeight(route[2]);
        for(EdgeMap::const_iterator iter2=edgelist2->begin();iter2!=edgelist2->end();++iter2){
            int id3=iter2->first;
            float weight2;
            if(iter2->second!=NULL){
                weight2=iter2->second->getWeight();
                if (weight2==0) continue;
            }
            float feat2=weight2/allweight2;
            if (isDebug) cout<<kongbai<<kongbai<<"�ڶ��ڵ㵽�������ڵ㣺"<<weight2<<"/"<<allweight2<<"="<<feat2<<endl;
            if(length==2){
                if(!socialNet->isNeighbor(fromId,route[0],id3,route[2])){
                    //����ڵ��ԭ�ڵ�û�����ӣ���������֮���Ԫ·������ֵ
                    float feature=feat1*feat2;
                    FeatureMap::iterator fiter=result.find(id3);
                    if(fiter==result.end()){
                        if (isDebug) cout<<kongbai<<kongbai<<"**����һ���µ�����ֵ:"<<id3<<"-"<<feature<<endl;
                        result.insert(FeatureMap::value_type(id3,feature));
                    }
                    else{
                        if (isDebug) cout<<kongbai<<kongbai<<"**��������ֵ����������·���ɴ:"<<id3<<"-"<<feature<<"+"<<fiter->second<<"="<<feature+fiter->second<<endl;
                        fiter->second=feature+fiter->second;
                    }
                }else{
                    if (isDebug) cout<<kongbai<<kongbai<<"**�����ڵ��Դ��ڱ�"<<endl;
                }
            }else{
                Item * item3=socialNet->getItemPtrById(id3,route[2]);
                if(item3==NULL) continue;
                if (item3->getId()==fromId&&item3->getType()==route[0])
                {
                    if (isDebug) {cout<<kongbai<<kongbai<<"�������ڵ�͵�һ���ڵ��ظ�������"<<endl;}
                    continue;
                }
                if (isDebug) {cout<<kongbai<<kongbai<<"�������ڵ㣺"<<endl;item3->print(route[1],2);}
                EdgeMap *edgelist3=item3->getToItemE(route[3]);
                float allweight3=item3->getAllWeight(route[3]);

                for(EdgeMap::const_iterator iter3=edgelist3->begin();iter3!=edgelist3->end();++iter3){
                    int id4=iter3->first;
                    float weight3=1;
                    if(iter3->second!=NULL){
                        weight3=iter3->second->getWeight();
                        if (weight3==0) continue;
                    }
                    float feat3=weight3/allweight3;
                    if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"�����ڵ㵽���ĸ��ڵ㣺"<<weight3<<"/"<<allweight3<<"="<<feat3<<endl;
                    if(!socialNet->isNeighbor(fromId,route[0],id4,route[3])){
                        float feature=feat1*feat2*feat3;
                        FeatureMap::iterator fiter=result.find(id4);
                        if(fiter==result.end()){
                            if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"**����һ���µ�����ֵ:"<<id4<<"-"<<feature<<endl;
                            result.insert(FeatureMap::value_type(id4,feature));
                        }
                        else{
                            if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"**��������ֵ����������·���ɴ:"<<id4<<"-"<<feature+fiter->second<<endl;
                            fiter->second=feature+fiter->second;
                        }
                    }else{
                        if (isDebug) cout<<kongbai<<kongbai<<kongbai<<"**�����ڵ��Դ��ڱ�"<<endl;
                    }
                }
            }
        }
    }
    if (isDebug)
    {
        cout<<"�����Item�ĸ�Ԫ·������ֵ����*************"<<endl;
    }
    return result;
}
//�ҵ�����Ϊcount�ĸ���,��count=-1�������еĸ���������
int MetaCpu::addNegativeCase(int fromid,
    FeatureMap * metaFea,
    ofstream &outFile,
    int count){
    /*
        ����������ѡ������metaFea����ʣ�µ�ȫ�����Ǹ���������ֵ��
        ���ԣ�������Ҫ���������ʱ��θ�����������metaFea�ҳ���Ҫ�ĸ�����
        �ҳ�������
        �ȴӵ�һ��Ԫ·���ɴ�Ľڵ㿪ʼ���ҵ�Ԫ·������ֵ����Ϊһ������
        Ȼ���ҵڶ���Ԫ·���ɴ�Ľڵ㣬��ʱ����һ��Ԫ·���ɴ�Ľڵ��ڵ�һ�δ���ʱ��ɾ�������������ҵ��Ľڵ��һԪ·������ֵΪ0.
        �Դ����ơ�
    */
    if(count==0) return 0;
    if(!outputNegEqualPos)  count=-1;
    


    int negcount=0;//ѡ��ĸ�������
    int trueNegCount=0;//ԭʼ�ĸ�������
    bool flag=true;//�Ƿ�Ҫ������Ӹ���

    if (isDebug) printFeatureMap(metaFea);
    for(int i=0;i<featureCount;i++){
        float feat[6];
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
                if(outputFileWrite){
                    outFile<<fromid<<","<<toid<<",";
                    for(int x=0;x<featureCount;x++){
                        outFile<<feat[x]<<",";
                    }
                    outFile<<"0"<<endl;
                    if (isDebug)
                    {
                        cout<<fromid<<","<<toid<<",";
                        for(int x=0;x<featureCount;x++){
                            cout<<feat[x]<<",";
                        }
                        cout<<"0"<<endl;
                    }
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

void MetaCpu::printFeatureMap(const FeatureMap *fm) const{

    string kongbai="    ";
    cout<<"****�������ֵ����Feature:"<<endl;
    for(int i=0;i<featureCount;i++){
        cout<<"Ԫ·��"<<i<<":"<<endl;
        for(FeatureMap::const_iterator iter=fm[i].begin();iter!=fm[i].end();++iter){
            cout<<kongbai<<iter->first<<":"<<iter->second<<endl;
        }
    }
    cout<<"****����ֵ�����������****"<<endl;
}