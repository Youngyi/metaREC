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
    map<int,float> u2uWeightMap;//用户i和用户k的相似度：Wi,k，其中i为当前的userid，int-k，float-u2uWeight：Wi,k
    map<int,float> recResult;//针对用户i的推荐结果，其中第一个int表示item的id，float表示预测到的评价值
    map<int,float> sortedRec1;//Top-k个推荐结果TOPN1-5
    map<int,float> sortedRec2;//Top-k个推荐结果TOPN2-10
    map<int,float> sortedRec3;//Top-k个推荐结果TOPN3-20

    set<int> locIdSet;//测试集中当前用户已访问过的用户，用于消除测试集中用户多次访问某一位置
    int rightRec[3]={0};//推荐正确的数量
    int allRec[3]={0};//推荐的所有的元素数量
    int rightCase[3]={0};//应该被推荐的数量

    int rightRecOfUser[3] = {0};
    int rightCaseOfUser=0;
    while(true){
        i++;
        //读一行签到记录并解析
        if(testfile.eof()){
            cout<<"结束，最终结果："<<endl;
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
        //解析签到记录完毕

        if(user==NULL||loc==NULL){//如果用户或者位置不存在
            continue;
        }
        if(userid!=lastUserId){//针对于一个新的用户，为其推荐TOP-K的地点

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
                cout<<" 精确率rightRec[0]/allRec[0]："<<rightRec[0]<<"/"<<allRec[0]<<" :"<<(float)rightRec[0]/allRec[0]<<endl;
                cout<<"召回率rightRec[0]/rightCase[0]"<<rightRec[0]<<"/"<<rightCase[0]<<" :"<<(float)rightRec[0]/rightCase[0]<<endl;
               // cout<<"top-10:"<<endl;
               // printMap(sortedRec2);
                cout<<" 精确率rightRec[1]/allRec[1]："<<rightRec[1]<<"/"<<allRec[1]<<" :"<<(float)rightRec[1]/allRec[1]<<endl;
                cout<<"召回率rightRec[1]/rightCase[1]"<<rightRec[1]<<"/"<<rightCase[1]<<" :"<<(float)rightRec[1]/rightCase[1]<<endl;
                //cout<<"top-20:"<<endl;
                //printMap(sortedRec3);
                cout<<" 精确率rightRec[2]/allRec[2]："<<rightRec[2]<<"/"<<allRec[2]<<" :"<<(float)rightRec[2]/allRec[2]<<endl;
                cout<<"召回率rightRec[1]/rightCase[2]"<<rightRec[2]<<"/"<<rightCase[2]<<" :"<<(float)rightRec[2]/rightCase[2]<<endl;
            }
            //首先清除一些东西
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

            //计算每一个用户和其他用户的相似度Wi,k,将结果存放在u2uWeightMap
            calculateWik(u2uWeightMap,userid);
           // cout<<fileName<<endl;
            cout<<"用户："<<userid<<endl;

            cout<<"用户-用户权值：（用户，权值）"<<userid<<endl;
            // printMap(u2uWeightMap);

            //计算用户对未评价的ITEM的隐含评价，以此作为推荐结果
            calRecResult(recResult,u2uWeightMap,userid);
            cout<<"推荐结果：（位置ID，推荐度）"<<userid<<endl;
            //printMap(recResult);

            //选择top-k个推荐结果
            sortRecResult(sortedRec1,recResult,TOPN_1);
            sortRecResult(sortedRec2,recResult,TOPN_2);
            sortRecResult(sortedRec3,recResult,TOPN_3);
            cout<<"排序厚的推荐结果：（位置ID，推荐度）"<<userid<<endl;
            //printMap(sortedRec1);
            lastUserId=userid;
        }
        rightCaseOfUser++;
        //计算精确率和召回率
        //第一步统计推荐正确的数量
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
    //计算精确率和召回率
    cout<<"UCFR"<<fileName<<endl;
    cout<<"完成："<<endl;
    cout<<"top-5:"<<endl;
    cout<<" 精确率："<<(float)rightRec[0]/allRec[0]<<endl;
    cout<<"召回率"<<(float)rightRec[0]/rightCase[0]<<endl;
    cout<<"top-10:"<<endl;
    cout<<" 精确率："<<(float)rightRec[1]/allRec[1]<<endl;
    cout<<"召回率"<<(float)rightRec[1]/rightCase[1]<<endl;
    cout<<"top-20:"<<endl;
    cout<<" 精确率："<<(float)rightRec[2]/allRec[2]<<endl;
    cout<<"召回率"<<(float)rightRec[2]/rightCase[2]<<endl;
}

UCFRecommend::~UCFRecommend(void)
{
}
