#pragma once
#include "base.h"
#include <string>
#include "Socialnet.h"
#include <set>


class MetaCpuForFriendRec
{
public:
    //��ȡ��һ�׶ε����ݣ�����Ԫ·������ֵ���������������������䱣�����ļ��С�
    void metaCpu(string testDateFileName,string outputFileName,int toItemType,bool useWindowTime=false);
    MetaCpuForFriendRec(Socialnet *socialNet,bool outputNegEqualPos,bool outputFileWrite);
private:
    Socialnet *socialNet;

    //outputNegEqualPos: 1��ʾ��������ͬ�ĸ���-train����0��ʾ���еĸ���-test��
    //outputFileWrite:1��ʾ���д�ļ���0��ʾ������ԣ�ֻΪ��ͳ������
    const bool outputNegEqualPos;
    const bool outputFileWrite;

    int featureCount;




    //�������routeԪ·����idΪfromId��ITEM������item������Ԫ·�����Ե����ITEM��������ֵMAP
    FeatureMap calFeature(int fromId,//��ʼitem��ID�����ص�������Ϊ���item������itemֱ�ӵ�����ֵ
        const int *route,//Ԫ·������
        int length,
        int  windowTime = -1
        );//�������Ԫ·���ĳ���


    //��ȡ��һ�׶ε����ݣ�����Ԫ·������ֵ���������������������䱣�����ļ��С�
    int addNegativeCase(int fromid,//��ʵitem ID
        FeatureMap * metaFea,//��������ŵص�
        ofstream &outFile,//ѵ�����Ͳ��Լ�������ļ�
        int count//�ҵ�����Ϊcount�ĸ���,��count=-1�������еĸ���������
        );
    int addAPositiveCase(//��������һ�������������Ƿ�����
        Item * user,
        Item * toItem,
        int itemType,
        FeatureMap * metaFea,
        set<int> *uToUserSet,
        int * metaPathInCount,//��¼������������һ��Ԫ·��
        string & postiveStr////�洢��Ҫд���ļ��������ַ����������ɲ��Լ��׶Σ������������20���Ͳ�д�룬Ҳ�����ɸ�����
        );

    void printFeatureMap(const FeatureMap *fm) const;
};

