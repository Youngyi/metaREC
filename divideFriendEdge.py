# -*- coding: cp936 -*-
#useful
###����ǩ����ʱ���Ϊ��ͬ��ǩ�����ϣ�ͨ���Ƚϣ���һ�����ѵ�����Ͳ��Լ�
import random

f=open("d:\\data\\input\\gowalla_friends_edges.txt",'r')
trainf1=open("d:\\data\\input\\gowalla_friends_traindata11.txt",'w+')
trainf2=open("d:\\data\\input\\gowalla_friends_traindata12.txt",'w+')
testf1=open("d:\\data\\input\\gowalla_friends_testdata11.txt",'w+')
testf2=open("d:\\data\\input\\gowalla_friends_testdata12.txt",'w+')

testc1=0
testc2=0
trainc1=0
trainc2=0
#���д�������

while True:
    newline=f.readline()
    if newline=='':
        break;
    ran = random.randint(1,100)
    if ran <= 80:
        trainf1.write(newline)
        testf1.write(newline)
    else:
        if ran<=90:
            trainf2.write(newline)
            testf1.write(newline)
        else:
            testf2.write(newline)

f.close()
trainf1.close()
trainf2.close()
testf1.close()
testf2.close()
