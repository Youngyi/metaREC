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
#"d:\\data\\fileter_processedChekins.txt"
#J:\\checkin\\filter_allCheckinNewYork.txt
f=open("E:\\checkin\\filter_foursquare_allCheckin.txt",'r')
#trainf=open("d:\\trainfilter2.txt",'w+')
#testf=open("d:\\testfilter2.txt",'w+')

locr={0:{0:0}}
user_loc={0:{0:0}}
checkintime=[0]*21
x=0
locr_max=0#位置边的最大权值
trainc=0
testc=0
locckins={0:0}
userckins={0:0}

#逐行处理数据

#第一行初始化,为位置关系等比较
lastline=f.readline()
arr=lastline.split('\t')
lasttime=time.mktime(strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ"))
lastuserid=arr[0]
lastlocid=arr[4]


timeDic={0:0}
i=0
while True:
    newline=f.readline()
    if newline=='':
        break;
    arr=newline.split('\t')
    tm=strptime(arr[1],"20%y-%m-%dT%H:%M:%SZ")
    newtime=time.mktime(tm)
    userid=int(arr[0])
    locidstr=arr[4]
    locid=int(locidstr)


    #统计每个签到的时间分布
    yearmon = int(str(tm.tm_year)+str(tm.tm_mon))
    if yearmon in timeDic:
        timeDic[yearmon] = timeDic[yearmon] + 1
    else:
        timeDic[yearmon] =1
            



    

    '''
    if tm.tm_year==2010 and tm.tm_mon>=10:
        testf.write(newline)
        testc=testc+1
    else:
        trainf.write(newline)
        trainc=trainc+1
    '''



    #统计每个用户的签到次数
    
    if userid in userckins:
        userckins[userid]=userckins[userid]+1
    else:
        userckins[userid]=1
    
   #每个位置的访问次数统计
    if locid in locckins:
        locckins[locid]=locckins[locid]+1
    else:
        locckins[locid]=1


    #存储位置-位置关系权值
    if arr[0]==lastuserid and lasttime-newtime<4*60*60 and lastlocid!=locid:
        if locid in locr:
            if lastlocid in locr[locid]:
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

    #存储用户对位置的访问矩阵
    if userid in user_loc:
        if(locid in user_loc[userid]):
            user_loc[userid][locid]=user_loc[userid][locid]+1
        else:
            user_loc[userid][locid]=1
    else:
        user_loc[userid]={locid:1}


#统计及输出

#print 'trainset:',trainc
#print 'testSet:',testc

for time_yearmon in timeDic:
    print str(time_yearmon)+":"+str(timeDic[time_yearmon])

##统计数据集矩阵密度

allsize=len(userckins)*len(locckins)
checkinsize=0
for uuid in user_loc:
    checkinsize=checkinsize+len(user_loc[uuid])
print 'raw_density: ',float(checkinsize)/allsize,' =checkinsize: ',checkinsize,' *allsize: ',allsize


#位置关系统计
print '位置关系统计',locr_max
locrcountlist=[0]*1000
for flid in locr:
    for slid in locr[flid]:
        locrcount=locr[flid][slid]
        if(locrcount>=1000): locrcount=999
        locrcountlist[locrcount]=locrcountlist[locrcount]+1
for i in range(0,1000):
    locr_table.write(i,0,i)
    locr_table.write(i,1,locrcountlist[i])


#统计在某一区间的签到数量的位置数量，R为range，N为取样点数量
print '统计在某一区间的签到数量的位置数量'
print '位置的总数量：',len(locckins)
R=1
N=5000
locckincoutlist=[0]*N
for lid in locckins:
    dex=locckins[lid]
    if dex>=N:dex=N-1
    locckincoutlist[dex]=locckincoutlist[dex]+1
for i in range(0,N):
    tmpstr=i
    loc_checkin_table.write(i,0,tmpstr)
    loc_checkin_table.write(i,1,locckincoutlist[i])

    
#统计签到位置数量
print '统计在某一区间的签到数量的用户数量'
print '用户的总数量：',len(userckins)
N=5000
userckincountlist=[0]*N
for uid in userckins:
    dex=userckins[uid]
    if dex>=N:dex=N-1
    userckincountlist[dex]=userckincountlist[dex]+1
for i in range(0,N):
    tmpstr=i
    user_checkin_table.write(i,0,tmpstr)
    user_checkin_table.write(i,1,userckincountlist[i])

 #'d:\\data\\dataAnalyse_ll.xls'
xlsfile.save('E:\\checkin\\foursquare_alcheckin_dataAnalyse_ll.xls')
print 'end!'

