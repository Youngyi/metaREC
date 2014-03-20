#include "BasedRecommend.h"

#include <vector>
#include <math.h>
#include <set>
#include<iostream>
#include<iterator>
#include "base.h"
#include "Item.h"
#include "Edge.h"
void BasedRecommend::printMap(const map<int,float> &mapt ){
    for(map<int,float>::const_iterator iter=mapt.begin();iter!=mapt.end();++iter){
        cout<<iter->first<<"     "<<iter->second<<endl;
    }
}
BasedRecommend::BasedRecommend(Socialnet * socNet)
{
    this->socialNet = socNet;
}
void BasedRecommend::calculateWik(map<int,float> & wikMap,int userid){
    Item * u=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
    //����ÿһ���û��������û������ƶ�Wi,k
    //��ʱ�����������һ��int��ʾk���ڶ���int��ʾ��Ӧ��ֵ
    map<int,int> cijckjMap;
    int cij2=0;//���ڵ�ǰ���û����й̶���cij2
    //Wik=cijckj/��sqrt��cij2��*sqrt��ckj2����
    EdgeMap *locEdgeList=u->getToLocE();
    //�����û�u��˵���ҵ��û������з��ʹ���λ�ã�Ȼ���ҵ����ʹ���Щλ�õ��û�
    for(EdgeMap::const_iterator liter=locEdgeList->begin();liter!=locEdgeList->end();++liter){
        int lid=liter->first;
        float cij=liter->second->getWeight();
        cij2+=cij*cij;
        Item * loc=socialNet->getItemPtrById(lid,ITEMTYPE_LOCATION);
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
        Item *toUser=socialNet->getItemPtrById(toUId,ITEMTYPE_USER);
        EdgeMap * locList=toUser->getToLocE();
        for(EdgeMap::const_iterator edgIter=locList->begin();edgIter!=locList->end();++edgIter){
            ckj2+=edgIter->second->getWeight()*edgIter->second->getWeight();
        }
        wikMap[toUId]=cijckjMap[toUId]/(sqrt((float)cij2)*sqrt((float)ckj2));
    }
}
void BasedRecommend::calculateSIik(map<int,float>& f2fWeightMap,int userid){
    const float para=0.5;//�Զ���Ȩֵ
    Item * u=socialNet->getItemPtrById(userid,ITEMTYPE_USER);
    //����ÿһ���û������ĺ��ѵĺ��Ѷ�SIk,i
    EdgeMap * frdsEdgeList=u->getToUserE();
    for(EdgeMap::const_iterator fliter=frdsEdgeList->begin();fliter!=frdsEdgeList->end();++fliter){
        int coFrdCount=0;
        int allFrdCount=0;
        int coVisitedCount=0;
        int allVisitedCount=0;
        int fid=fliter->first;
        Item * frdUser = socialNet->getItemPtrById(fid,ITEMTYPE_USER);

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
void BasedRecommend::calRecResult(map<int,float>&recResult,const map<int,float> & u2uWeightMap,int uid){
    int ss=0;
    Item * user=socialNet->getItemPtrById(uid, ITEMTYPE_USER);
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
        Item * toUser=socialNet->getItemPtrById(toUserId,ITEMTYPE_USER);//::getUserPtrbyId(toUserId,NOALLOWNEW);
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
                Item *tmpUser=socialNet->getItemPtrById(tmpUid,ITEMTYPE_USER);
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
void BasedRecommend::sortRecResult(map<int,float> &sortedRec,const map<int,float> &recResult,int N){
    int leastUserId=-1;
    float leastRanked=INT_MAX;
    int sortedCount=0;
    for(map<int,float>::const_iterator recIter=recResult.begin();recIter!=recResult.end();++recIter){
        if(sortedCount<N){
            sortedRec.insert(map<int,float>::value_type(recIter->first,recIter->second));
            sortedCount++;
            if(sortedCount==N){
                //N�����ˣ��ʹ洢��С��Ԫ��
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
BasedRecommend::~BasedRecommend(void)
{
}
