# -*- coding: cp936 -*-
import time
import xlwt
try:
    strptime = time.strptime
except AttributeError:
    from strptime import strptime

    
xlsfile=xlwt.Workbook()
loc_checkin_table=xlsfile.add_sheet('loc_checkin_count')
user_checkin_table=xlsfile.add_sheet('user_checkin_count')
locr_table=xlsfile.add_sheet('locr_count')

f=open("d:\\filter2.txt",'r')
trainf=open("d:\\trainfilter2.txt",'w+')
testf=open("d:\\testfilter2.txt",'w+')



locr={0:{0:0}}
user_loc={0:{0:0}}
checkintime=[0]*21
x=0
locr_max=0#λ�ñߵ����Ȩֵ
trainc=0
testc=0
locckins={0:0}
userckins={0:0}

#���д�������

#��һ�г�ʼ��,Ϊλ�ù�ϵ�ȱȽ�
lastline=f.readline()
arr=lastline.split('\t')
lasttime=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
lastuserid=arr[0]
lastlocid=arr[4]

i=0
while True:
    #i=i+1
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    userid=int(arr[0])
    locidstr=arr[4]
    locid=int(locidstr[:-2])

    #ͳ��ÿ��ǩ����ʱ��ֲ�
    if tm.tm_year==2010 and tm.tm_mon>=10:
        testf.write(newline)
        testc=testc+1
    else:
        trainf.write(newline)
        trainc=trainc+1

print "TrainCount:",trainc,"   TestCount:",trainc
'''
    #ͳ��ÿ���û���ǩ������
    
    if userckins.has_key(userid):
        userckins[userid]=userckins[userid]+1
    else:
        userckins[userid]=1
    
   #ÿ��λ�õķ��ʴ���ͳ��
    if locckins.has_key(locid):
        locckins[locid]=locckins[locid]+1
    else:
        locckins[locid]=1


    #�洢λ��-λ�ù�ϵȨֵ
    if arr[0]==lastuserid and lasttime-newtime<24*60*60 and lastlocid!=locid:
        if locr.has_key(locid):
            if locr[locid].has_key(lastlocid):
                locr[locid][lastlocid]= locr[locid][lastlocid]+1
                if locr[locid][lastlocid]>locr_max:locr_max=locr[locid][lastlocid]
            else:
                locr[locid][lastlocid]=1
           ## print "haskey",locr[locid][lastlocid]
        else:
            locr[locid]={lastlocid:1}
           ## print "nokey",locr[locid][lastlocid]
          
    lastuserid=arr[0]
    lasttime=newtime
    lastlocid=locid

    #�洢�û���λ�õķ��ʾ���
    if user_loc.has_key(userid):
        if(user_loc[userid].has_key(locid)):
            user_loc[userid][locid]=user_loc[userid][locid]+1
        else:
            user_loc[userid][locid]=1
    else:
        user_loc[userid]={locid:1}


#ͳ�Ƽ����

print 'trainset:',trainc
print 'testSet:',testc



##ͳ�����ݼ������ܶ�

allsize=len(userckins)*len(locckins)
checkinsize=0
for uuid in user_loc.keys():
    checkinsize=checkinsize+len(user_loc[uuid])
print 'raw_density: ',float(checkinsize)/allsize,' =checkinsize: ',checkinsize,' *allsize: ',allsize


#λ�ù�ϵͳ��
print 'λ�ù�ϵͳ��',locr_max
locrcountlist=[0]*1000
for flid in locr.keys():
    for slid in locr[flid].keys():
        locrcount=locr[flid][slid]
        if(locrcount>=1000): locrcount=999
        locrcountlist[locrcount]=locrcountlist[locrcount]+1
for i in range(0,1000):
    locr_table.write(i,0,i)
    locr_table.write(i,1,locrcountlist[i])
xlsfile.save('d:\\data1.xls')

#ͳ����ĳһ�����ǩ��������λ��������RΪrange��NΪȡ��������
print 'ͳ����ĳһ�����ǩ��������λ������'
print 'λ�õ���������',len(locckins)
R=1
N=5000
locckincoutlist=[0]*N
for lid in locckins.keys():
    dex=locckins[lid]
    if dex>=N:dex=N-1
    locckincoutlist[dex]=locckincoutlist[dex]+1
for i in range(0,N):
    tmpstr=i
    loc_checkin_table.write(i,0,tmpstr)
    loc_checkin_table.write(i,1,locckincoutlist[i])

    
#ͳ��ǩ��λ������
print 'ͳ����ĳһ�����ǩ���������û�����'
print '�û�����������',len(userckins)
N=5000
userckincountlist=[0]*N
for uid in userckins.keys():
    dex=userckins[uid]
    if dex>=N:dex=N-1
    userckincountlist[dex]=userckincountlist[dex]+1
for i in range(0,N):
    tmpstr=i
    user_checkin_table.write(i,0,tmpstr)
    user_checkin_table.write(i,1,userckincountlist[i])
'''


print 'end!'

