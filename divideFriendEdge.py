# -*- coding: cp936 -*-
#useful
###����ǩ����ʱ���Ϊ��ͬ��ǩ�����ϣ�ͨ���Ƚϣ���һ�����ѵ�����Ͳ��Լ�
import random

f=open("e:\\checkin\\foursquare_all_friends_edges_out.txt",'r')
trainf1=open("e:\\checkin\\foursquare_all_friends_traindata1.txt",'w+')
trainf2=open("e:\\checkin\\foursquare_all_friends_traindata2.txt",'w+')
testf1=open("e:\\checkin\\foursquare_all_friends_testdata1.txt",'w+')
testf2=open("e:\\checkin\\foursquare_all_friends_testdata2.txt",'w+')

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
