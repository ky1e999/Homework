import time
from tkinter import *
from tkinter.messagebox import *
import socket
import threading
import os
import tkinter.messagebox
root = Tk()
root.title(" 网络五子棋--UDP 客户端")
chess=["黑棋","白棋"]
chess_cons=["先手","后手"]
print (os.path.abspath('.'))#获得当前工作目录

imgs= [PhotoImage(file='bmp\\BlackStone.gif'), PhotoImage(file='bmp\\WhiteStone.gif')]
turn=0
Myturn=-1
Start=0  #判断是否可以点击
STO=0  #确定先手重置游戏
Map=[]
WIN_LOSE=0
ID=0
def callexit(event):#退出
    pos="exit|"
    sendMessage(pos)
    os._exit(0)

def startover(event):#重置游戏

    pos="startover|"
    sendMessage(pos)
    global STO
    STO=1

def calljoin(event):#连接服务器
    global host,Myturn
    host = entryServerIP.get()
    Start=1
    pos='join|'          #"连接服务器"命令
    sendMessage(pos);#发送连接服务器请求

    if Start == 1:
        cv.bind("<Button-1>", callback)
        cv.pack()
    startNewThread()     # 启动线程接收服务器端的消息receiveMessage();

def callback(event):#走棋
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
        print("客户端走的位置",pos)
        label1["text"]="客户端走的位置"+pos+"下一位为服务端"

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
                    Start=0
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
                    WIN_LOSE = 1
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

def drawOtherChess(x,y):#画对方棋子
        global turn,ID
        img1= imgs[turn]
        ID+=1
        c2=cv.create_image((x*40+20,y*40+20),image=img1,tag='tag{}'.format(ID))
        cv.pack()
        map[x][y]=str(turn)

        Map.append((x,y,ID))#将点存储起来,方便回溯

##        k=win_lose( )
## 
##        if win_lose( )==True:
##            if turn==0 :
##                showinfo(title="提示",message="黑方你赢了")
##            else:
##                showinfo(title="提示",message="白方你赢了")
        #换下一方走棋
        if turn==0 :
            turn=1
        else:
            turn=0
#接收消息 
def receiveMessage():
    global s, STO,Map,Myturn
    while True:
        data  = s.recv(1024).decode('utf-8','ignore')
        a=data.split("|")                     #分割数据
        if not data:
            print('client has exited!')
            break
        elif a[0] == 'exit':                  #对方退出信息
            print('对方退出!')
            label1["text"]='对方退出，游戏结束'
        elif a[0] == 'over':    #对方赢信息
            print('对方赢信息!')
            label1["text"]=data.split("|")[0] 
            showinfo(title="提示",message=data.split("|")[1] )
            time.sleep(5)
            showinfo(title="提示", message="可以进行重置游戏来重新开始一把游戏")
        elif a[0] == 'move':    #服务器走的位置信息
            print('received:',data)            
            p=a[1].split(",")
            x=int(p[0])
            y=int(p[1])
            print(p[0],p[1])
            label1["text"]="服务器走的位置"+p[0]+","+p[1]+"下一位为客户端"
            drawOtherChess(x,y)        #画对方棋子
        elif a[0]=='startover':#对方请求重新开始游戏
            global map
            if map==[]:
                showinfo(title="提示",message="现在已是新棋盘")
            print("对方请求重新开始游戏")
            result = tkinter.messagebox.askokcancel(title="重置游戏", message="对方请求重开~")
            if result == True:
                for i in range(1, ID + 1, 1):
                    cv.delete("tag{}".format(i))
                global WIN_LOSE
                WIN_LOSE=0
                map = [[" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "] for y in range(15)]
                if STO==0:
                    sendMessage("startover|")    # 重置游戏
                else :
                    STO=0
        elif a[0]=="begin":#来自服务端提供的下棋次序

            b=(int(a[1])+1)%2
            print("服务端的棋子为{}".format(chess[int(a[1])]))
            print("客户端的棋子是{}".format(chess[b]))
            global Myturn
            Myturn=b
            label1["text"] = "客户端:{},{}".format(chess[Myturn], chess_cons[Myturn]) 
        
    s.close()
 
#发送消息 
def sendMessage(pos):
    try:
        global s
        s.sendto(pos.encode(),(host,port))
    except NameError:#如果存在仅仅只有客户端上线时，进行此操作
        pass

#启动线程接收端的消息
def startNewThread( ):
        #启动一个新线程来接收服务器端的消息
        #thread.start_new_thread(function,args[,kwargs])函数原型，
        #其中function参数是将要调用的线程函数，args是传递给线程函数的参数，它必须是个元组类型，而kwargs是可选的参数
        #receiveMessage函数不需要参数，就传一个空元组
        thread=threading.Thread(target=receiveMessage,args=())
        thread.setDaemon(True)
        thread.start();     

    
#map = [[" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],
#       [" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "],]

#map =  [[" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "]] * 15错误

map =  [[" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "]for y in range(15)]
cv = Canvas(root, bg = 'steelblue', width = 610, height = 610)
drawQiPan( )


label1=Label(root,text="客户端....")
label1.pack()
v=StringVar()
entryServerIP = Entry(root, textvariable =v )#Entry组件
v.set("127.0.0.1") #设置StringVar变量的值，Entry组件文本自动更新
entryServerIP.pack()

f=Frame(root)
button0=Button(f,text="连接服务器")
button0.bind("<Button-1>", calljoin)
button0.pack(side='left')
button1=Button(f,text="重置")
button1.bind("<Button-1>", startover)
button1.pack(side='left')
button2=Button(f,text="退出游戏")
button2.bind("<Button-1>", callexit)
button2.pack(side='left')

f.pack()
#创建UDP SOCKET
s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
port = 8000          #服务器端口

root.mainloop()
 

#!/usr/bin/env python
# coding=utf-8
