# -*- coding: cp936 -*-
import time
'''
���˵��û������ǩ������5����֮�ڵ�����ǩ����

'''
#import strptime
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime

f=open("d:\\1totalCheckins.txt",'r')
out=file("d:\\1out.txt","w")

#��һ�У���ʱ��ֵ�洢��lastt��,��ʼ��
lastline=f.readline()
arr=lastline.split('\t')
print arr
lastt=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
lastflag=True
i=0
#�����ÿ����ǰһ���жϣ����user��ͬ��ʱ���������֮�ڣ������������д�뵽out��txt��
#��Ĭ��Ϊ��ͬ�û�֮������ĺ������ǩ��ʱ�䲻�����������֮�ڣ�
while True:
    line=f.readline()
    arr=line.split('\t')
    i=i+1
    if i%1000==0:
        print i,":",arr
    #���ʱ��ֵ
    intt=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
    
    if lastt-intt>300:
        if lastflag:
            out.write(lastline)
        lastflag=True
    else:
        lastflag=False
    lastt=intt;
    lastline=line;
    if("xx" in arr[4]):
        print "end:",arr[4],"len:",len(arr[4] and "xx")
        break
out.flush();
out.close();
