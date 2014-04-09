# -*- coding: cp936 -*-
#useful
'''
将Foursquare的csv数据，转为gowalla的格式，便于一下统一分析
'''

import time
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime

def timeNum(year,month):
    return year*100+month

f=open("E:\\checkin\\allCheckin.csv",'r')

fout=open("E:\\checkin\\foursquare_allCheckin.txt",'w')

#将string类型的locId改为int型的
locStrIdDic={}
locIdNum=0
i=0
while True:
    i=i+1
    if i%10000 ==0:
        print i
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split(',')#arr=newline.split('\t')
    tm=strptime(arr[2],"%d/%m/20%y %H:%M:%S")#tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    userid=int(arr[1])
    locidstr=arr[3]
    locid=locIdNum
    if locidstr in locStrIdDic:
        locid=locStrIdDic[locidstr]
    else:
        locStrIdDic[locidstr]=locIdNum
        locIdNum+=1
            
    timenum=timeNum(tm.tm_year,tm.tm_mon)

    nline=str(userid)+'\t'+str(tm.tm_year)+'-'+str(tm.tm_mon)+'-'+str(tm.tm_mday)+'T'+str(tm.tm_hour)+':'\
    +str(tm.tm_min)+":"+str(tm.tm_sec)+'Z\t'+arr[5]+'\t'+arr[6]+'\t'+ str(locid)+'\n'

    fout.write(nline)



print timeCountStatc
