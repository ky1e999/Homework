import hashlib
import threading
import random
import sys
length=1
lock=threading.Lock()
def Mine(miner):
    global length
    while(length<9):
        lock.acquire()  #保护区块内容，避免多个矿工同时在挖这个区块
        lines_num=len(open("coursework.txt",'r').readlines())
        line_index=open("coursework.txt").read().splitlines()
        lock.release() 
        if lines_num==0:
            h_old=str(0)
        else:
            h_old=line_index[lines_num-2]
        ra=random.randint(0,sys.maxsize)%100000
        h_new=hashlib.sha256((str(lines_num/2)+h_old+str(ra)).encode('utf-8')).hexdigest()
        if(int(h_new,16)<(pow(2,256)-pow(2,244))):  #成功找到符合挖矿要求的区块
            lock.acquire()
            with open("coursework.txt",'a+') as f:
                f.write(str(h_new)+'\n')   
                f.write(str(ra)+'\n')
                f.close()
            print("Miner {0} already put block which randomint is {1} \n".format(str(miner),str(ra)))
            length=length+1
            lock.release()
        else:
            print("Can't verify the POW\n")
def main():
    for i in range(3):
        miner = i + 1  #3个矿工
        m = threading.Thread(target=Mine, args=(miner,))
        m.start()

if __name__ == "__main__":
    main()
