import time
from tkinter import *
from tkinter.messagebox import *
import socket
import threading
import os
import tkinter.messagebox
import random   #通过随机确定谁先开始
Map=[]
root = Tk()
root.title("网络五子棋--服务器端")
print (os.path.abspath('.'))#获得当前工作目录
imgs= [PhotoImage(file='bmp\\BlackStone.gif'), PhotoImage(file='bmp\\WhiteStone.gif')]
turn=0       #轮到那方走棋 ，0黑方1是白方
Myturn=-1    #第一次确定自己的角色
STO=0  #确定先手重置游戏
WIN_LOSE=0 #是否已经有人获胜
qi=["黑棋","白棋"]
qi_to=["先手","后手"]
ID=0
def callexit(event):#退出
    pos="exit|"
    sendMessage(pos)
    os._exit(0)
def startover(event):#重置游戏
    pos="startover|"
    sendMessage(pos)
    begin()
    global STO
    STO=1
    #先不进行清空的情况，等待用户第二次进行返回时进行重置!
def begin():#随机确定谁先放置棋子
     pos="begin|"
     global Myturn
     Myturn=random.randint(0,1)#确定自己的下棋次序，以及提醒用户端的下棋次序
     sendMessage(pos+"{}".format(Myturn))
def callpos(event):#走棋
    global turn,ID
    global Myturn
    if Myturn==-1:  #第一次确定自己的角色（白方还是黑方）
        Myturn=turn
    else:
        if(Myturn!=turn):
           showinfo(title="提示",message="还没轮到自己走棋")
           return 
    #print ("clicked at", event.x, event.y,turn)
    x=(event.x)//40  #换算棋盘坐标
    y=(event.y)//40
    print ("clicked at", x, y,turn)   
    if map[x][y]!=" ":
       showinfo(title="提示",message="已有棋子")
    else:
        img1= imgs[turn]
        ID+=1
        c1=cv.create_image((x*40+20,y*40+20),image=img1,tag='tag{}'.format(ID))
        cv.pack()
        map[x][y]=str(turn)

        Map.append((x,y,ID))#将点存储起来,方便回溯

        pos=str(x)+","+str(y)
        sendMessage("move|"+pos)
        print("服务器走的位置",pos)
        label1["text"]="服务器走的位置"+pos+"下一位为用户端"
        
        #输出输赢信息
        if win_lose( )==True:
            if turn==0 :
                showinfo(title="提示",message="黑方赢了")
                sendMessage("over|黑方赢了")

            else:
                showinfo(title="提示",message="白方赢了")
                sendMessage("over|白方赢了")

        #换下一方走棋
        if turn==0 :
            turn=1
        else:
            turn=0



def drawOtherChess(x,y):#画对方棋子
        global turn,ID
        img1= imgs[turn]
        ID += 1
        c2=cv.create_image((x*40+20,y*40+20),image=img1,tag='tag{}'.format(ID))
        cv.pack()
        map[x][y]=str(turn)

        Map.append((x,y,ID))#将点存储起来,方便回溯

        #换下一方走棋
        if turn==0 :
            turn=1
        else:
            turn=0
            
def drawQiPan( ):#画棋盘
    for i in range(0,15):
        cv.create_line(20,20+40*i,580,20+40*i,width=2)
    for i in range(0,15):
        cv.create_line(20+40*i,20,20+40*i,580,width=2)
    cv.pack()
    
def win_lose( ):#输赢判断
    #扫描整个棋盘，判断是否连成五颗
    a = str(turn)
    #print ("a=",a)
    for i in range(0,11):#0--10
        # 判断X= Y轴上是否形成五子连珠
        for j in range(0,11):#0--10
            if map[i][j] == a and map[i + 1][j + 1] == a and map[i + 2][j + 2] == a and map[i + 3][j + 3] == a and map[i + 4][j + 4] == a :
                print("X=  Y轴上形成五子连珠")
                global WIN_LOSE
                WIN_LOSE = 1

                global Start
                Start = 0
                return True


    for i in range(4,15):# 4 To 14
        # 判断X= -Y轴上是否形成五子连珠
        for j in range(0,11):#0--10
            if map[i][j] == a and map[i - 1][j + 1] == a and map[i - 2][j + 2] == a and map[i - 3][j + 3] == a and map[i - 4][j + 4] == a :
                print("X= -Y轴上形成五子连珠")
                WIN_LOSE = 1
                Start = 0
                return True

    for i in range(0,15):#0--14
        # 判断Y轴上是否形成五子连珠
        for j in range(4,15):# 4 To 14
            if map[i][j] == a and map[i][j - 1] == a and map[i][j - 2] == a and map[i][j - 3] == a and map[i][j - 4] == a :
                print("Y轴上形成五子连珠")

                WIN_LOSE=1
                Start = 0
                return True

    for i in range(0,11):#0--10
        # 判断X轴上是否形成五子连珠
        for j in range(0,15):#0--14
            if map[i][j] == a and map[i + 1][j] == a and map[i + 2][j] == a and map[i + 3][j] == a and map[i + 4][j] == a :
                print("X轴上形成五子连珠")
                WIN_LOSE = 1
                Start = 0
                return True

    return False
    #End Function
def print_map( ):#输出map地图
    for j in range(0,15):#0--14 
       for i in range(0,15):#0--14
           print (map[i][j],end=' ')
       print ('w')

#发送消息
def sendMessage(pos):
    global s
    global addr
    s.sendto(pos.encode(),addr)

#接收消息 
def receiveMessage():
    global s, STO,Myturn
    while True:
        #接收客户端发送的消息
        global addr
        data, addr = s.recvfrom(1024)
        data=data.decode('utf-8','ignore')
        a=data.split("|")                          #分割数据
        if not data:
            print('client has exited!')
            break
        elif a[0] == 'join':                      #连接服务器请求
            print('client 连接服务器!')
            begin()
            global Start
            Start = 1
            if Start == 1:
                cv.bind("<Button-1>", callpos)
                cv.pack()
            label1["text"]='client 连接服务器成功'
            global Myturn
            label1["text"] = "服务端为{},{}".format(qi[Myturn],qi_to[Myturn])   #输出服务端棋子颜色
        elif a[0] == 'exit':                      #对方退出信息
            print('client 对方退出!')
            label1["text"]='client 对方退出，游戏结束'
        elif a[0] == 'over':        #对方赢信息
            print('对方赢信息!')
            label1["text"]=data.split("|")[0] 
            showinfo(title="提示",message=data.split("|")[1] )
            time.sleep(5)
            showinfo(title="提示", message="可以进行重置游戏来重新开始一把游戏")
        elif a[0] == 'move':        #客户端走的位置信息
            print('received:',data,'from',addr)
            p=a[1].split(",")
            x=int(p[0])
            y=int(p[1])
            print(p[0],p[1])
            label1["text"]="客户端走的位置"+p[0]+","+p[1]+"下一位为服务端"
            drawOtherChess(x,y)        #画对方棋子
        elif a[0]=="startover": #由用户端请求来重新进行比赛
            print("对方请求重新开始游戏")
            global map
            if map==[]:
                showinfo(title="提示",message="现在已是新棋盘")
            result=tkinter.messagebox.askokcancel(title="重置游戏",message="对方请求重新开始游戏~")
            if result==True:
                for i in range(1,ID+1,1):
                    cv.delete("tag{}".format(i))
                global WIN_LOSE
                map = [[" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "] for y in range(15)]
                WIN_LOSE = 0
                if STO ==0:
                    sendMessage("startover|")   #重置游戏
                else :
                    STO=0
    s.close()

#启动线程接收客户端的消息
def startNewThread( ):
        #启动一个新线程来接收客户器端的消息
        #thread.start_new_thread(function,args[,kwargs])函数原型，
        #其中function参数是将要调用的线程函数，args是传递给线程函数的参数，它必须是个元组类型，而kwargs是可选的参数
        #receiveMessage函数不需要参数，就传一个空元组
        thread=threading.Thread(target=receiveMessage,args=())


        thread.setDaemon(True)
        thread.start()

#创建UDP SOCKET
def callip(self):

    #选择是否在本计算机上同时开启客户端与服务端
    # label1=Label(root,text="服务器端ip地址选择(输入数字)(默认为使用127.0.0.1地址):1.等待其他用户来连接自身服务器端")
    # label1.pack()
    # v=StringVar()
    # entryServerIP = Entry(root, textvariable =v )#Entry组件
    # v.set("1") #设置StringVar变量的值，Entry组件文本自动更新
    # entryServerIP.pack()
    # select= entryServerIP.get()
    # button1 = Button(root, text="退出游戏")
    # button1.bind("<Button-1>", callexit)
    # button1.pack()
    while True:
        select= entryServerIP.get()
        if select=='1':
            #直接获取ip地址
            global localhost
            localhost = socket.gethostbyname(socket.gethostname())
            label1.config(text="当前ip地址为:{}".format(localhost))

            # label1 = Label(root, text="ip地址为:{}".format(localhost))
            # label1.pack()
            # showinfo(title="提示",message="ip地址为:{}".format(localhost))
            break
        else:
            localhost='127.0.0.1'
            label1.config(text="当前ip地址为:{}".format(localhost))
            # showinfo(title="提示", message="ip地址为:{}".format(localhost))

            break

    
#map = [[" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],]

#map =  [[" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "]] * 15错误
Start=0
map =  [[" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "]for y in range(15)]
cv = Canvas(root, bg = 'steelblue', width = 610, height = 610)
drawQiPan( )

label1=Label(root,text="服务器端ip地址选择(输入数字)(默认为使用127.0.0.1地址):1.其他主机客户端连接的ip地址")
label1.pack()
v=StringVar()
entryServerIP = Entry(root, textvariable =v )#Entry组件
v.set("0") #设置StringVar变量的值，Entry组件文本自动更新
entryServerIP.pack()



button1=Button(root,text="获取ip地址")
button1.bind("<Button-1>", callip)
button1.pack()

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
s.bind(('localhost',8000))
addr=('localhost',8000)
startNewThread()       ## 启动线程接收客户端的消息receiveMessage();

label2=Label(root,text="服务器端....")
label2.pack()
f=Frame(root)
button1=Button(f,text="重置游戏")
button1.bind("<Button-1>", startover)
button1.pack(side='left')
button2=Button(f,text="退出游戏")
button2.bind("<Button-1>", callexit)
button2.pack(side='left')
f.pack()



root.mainloop()

# !/usr/bin/env python
# coding=utf-8
