# -*- coding: cp936 -*-
###����ǩ����ʱ���Ϊ��ͬ��ǩ�����ϣ�ͨ���Ƚϣ���һ�����ѵ�����Ͳ��Լ�

import time
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime


f=open("d:\\data\\filter2.txt",'r')
trainf1=open("d:\\data\\traindata1.txt",'w+')
trainf2=open("d:\\data\\traindata2.txt",'w+')
testf=open("d:\\data\\testdata2.txt",'w+')
testc=0
trainc1=0
trainc2=0
#���д�������
while True:
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    userid=int(arr[0])
    locidstr=arr[4]
    locid=int(locidstr[:-2])

    #����ÿ��ǩ����ʱ��ֲ�
    if tm.tm_year==2010 and tm.tm_mon>=10:
        testf.write(newline)
        testc=testc+1
    else:
        if tm.tm_year==2010 and tm.tm_mon>=9:
            trainf2.write(newline)
            trainc2=trainc2+1
        else:
            trainf1.write(newline)
            trainc1=trainc1+1

print "Train1Count:",trainc1,"     Train2Count:",trainc2,"   TestCount:",testc
