import tkinter
from tkinter import messagebox
import random
from functools import partial
root = tkinter.Tk()
root.geometry('578x660+300+10')
root['bg'] = 'black'
root .title('数独游戏')
import random
def OneLine():
    l,ns = [[]],list('123456789')
    while len(l[-1]) != 9:
        l[-1] += ns.pop(random.randint(0,len(ns)-1))
    return l
def GetLen(l):
    L = []
    for _ in range(len(l)-1):
        L.append(l[_][len(l[-1])])
    return L
def Box(l):
    x ,y,L = len(l), len(l[-1]) ,[]
    if x <= 3:
        x = 0
    elif 4<= x <=6:
        x = 3
    else:
        x = 6

    if y < 3:
        y = [0,3]
    elif 3<= y <6:
        y = [3,6]
    else :
        y = [6,9]

    for _ in range(x,len(l)):
        L.extend(l[_][y[0]:y[1]])
    return L

def Check(l,n):
    if n not in l[-1] and n not in GetLen(l) and n not in Box(l):
        return True
    return False

def Create(): #
    l,y = OneLine(),0
    while len(l) != 9:
        ns,l,x = list('123456789'),l+[[]],0
        while len(l[-1]) != 9:
            x,n = x+1,ns.pop(random.randint(0,len(ns)-1))
            if x == 40:
                y += 1
                del l[-1]
                break
            if y == 10:
                del l[-1],l[-1]
                y = 0
                break
            if Check(l,n):
                l[-1]+=n
            else:
                ns.append(n)
    return l
def Lines(lines):
        for _ in range(9):
                n = random.randint(2,7)
                for __ in range(n):
                        n = random.randint(0,8)
                        lines[_][n] = ''
        return lines
def CheckSol(l):#检查是否成功
    lines = []
    while l != []:
        lines.append(l[:9])
        del l[:9]
    n = set('123456789')
    for line in lines:
        if n != set(line):
                return False
            
    for x in lines[:3],lines[3:6],lines[6:]:
        box1 = []
        box2 = []
        box3 = []
        [box1.extend(l[:3]) for l in x]
        [box2.extend(l[3:6]) for l in x]
        [box3.extend(l[6:9]) for l in x]
        if n != set(box1) or n != set(box2) or n != set(box3):
            return False
    for x in range(9):
        line = []
        for y in range(9):
            line.append(lines[y][x])
        if set(line) != n:
             return False
    return True
    
class app:
        def __init__(self):
                self.sol = Create()
                self.lines = Lines(tuple(list(tuple(l)) for l in self.sol ) )
                self.es = []
                self.vs = []

                self.b = 0
                x = 5
                y = 0
                n = 0
                for _ in range(9):
                        for __ in range(9):
                                self.vs.append(tkinter.StringVar())
                                self.es.append(tkinter.Entry(width=2,font=("",40,"bold"),textvariabl=self.vs[-1],justify = tkinter.CENTER,))
                                if self.lines[_][__]:
                                        self.es[n].config(bg='light green',fg='red')
                                        self.vs[n].set(self.lines[_][__])
                                        self.m2(n,self.lines[_][__])
                                else:
                                        self.m1(n)
                                self.es[-1].place(x=x,y=y)
                                n+=1
                                x +=62
                                if __ == 2 or __ == 5:
                                        x+=4
                        y+=66
                        x=5
                        if _ == 2 or _ == 5:
                                y+=4
                b1 = tkinter.Button(text='Quit', bg='black',fg='white', width=10, font=('',15,'bold'), bd=3,command=root.destroy)
                b1.place(x=440,y=610)
                b2 = tkinter.Button(text='Check',bg='black',fg='yellow',width=10,font=('',15,'bold'),bd=3,command=self.Check)
                b2.place(x=5,y=610)
                b3 = tkinter.Button(text='Try Again',bg='black',fg='blue',width=10,font=('',15,'bold'),bd=3,command=self.Again)
                b3.place(x=300,y=610)
                b4 = tkinter.Button(text='Show Solution', bg='black', fg='red', width=12, font=('',15,'bold'), bd=3, command=self.Sol)
                b4.place(x=140,y=610)
        def m1(self,v):
                class A:
                        def __init__(self,vs,v):
                                self.v = v
                                self.vs = vs
                        def a(self,*argv):
                                        s = self.vs[self.v].get()
                                        if s:
                                                if s.isnumeric() and "0" not in s:
                                                        self.vs[self.v].set(s[-1])
                                                else:
                                                        self.vs[self.v].set('')
                C = A(self.vs,v)
                self.es[-1].config(xscrollcommand=C.a)  
        def m2(self,v,x):
                class A:
                        def __init__(self,vs,v,x):
                                self.v = v
                                self.x = x
                                self.vs = vs
                        def a(self,*argv):
                                        s = self.vs[self.v].get()
                                        self.vs[self.v].set(self.x)
                                        
                C = A(self.vs,v,x)
                self.es[-1].config(xscrollcommand=C.a)
        def Sol(self):
                x = 0
                for _ in range(9):
                        for __ in range(9):
                                self.vs[x].set(self.sol[_][__])
                                x+=1
        def Check(self):
                l = []
                for _ in range(81):
                        l.append(self.vs[_].get())
                if not CheckSol(l):
                        messagebox.showinfo(' !','请再检查一遍 )=')
                        
                else:
                        self.Win()
        def Again(self):
                if messagebox.askyesno('؟','想要重玩吗 ?'):
                        app()
        def Win(self):
                messagebox.showinfo(' =)',' (=  (=  (=  You Win   =)  =)  =) =)')
                self.Again()        
app()

root .mainloop()