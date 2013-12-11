#pragma once
#include "base.h"
#include <string>
#include "Socialnet.h"
#include <set>


class MetaCpu
{
public:
    //��ȡ��һ�׶ε����ݣ�����Ԫ·������ֵ���������������������䱣�����ļ��С�
    void metaCpu(string testDateFileName,string outputFileName);
    MetaCpu(Socialnet *socialNet,bool outputNegEqualPos,bool outputFileWrite);
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
        int length);//�������Ԫ·���ĳ���


    //��ȡ��һ�׶ε����ݣ�����Ԫ·������ֵ���������������������䱣�����ļ��С�
    int addNegativeCase(int fromid,//��ʵitem ID
        FeatureMap * metaFea,//��������ŵص�
        ofstream &outFile,//ѵ�����Ͳ��Լ�������ļ�
        int count//�ҵ�����Ϊcount�ĸ���,��count=-1�������еĸ���������
        );
    int addAPositiveCase(//��������һ�������������Ƿ�����
        Item * user,
        Item * location,
        FeatureMap * metaFea,
        set<int> *uLocSet,
        ofstream &outFile,
        int * metaPathInCount//��¼������������һ��Ԫ·��
        );

    void printFeatureMap(const FeatureMap *fm) const;
};

