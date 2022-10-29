// sys10-11.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <queue>
using namespace std;
class process {
private:
    int pid;
    char state[10];
    double priority;  //小数判断优先级更精确
    int timearrive;
    int timeinneed;
    int timeforwait;
public:
    void setPriority() {
        priority = double((timeforwait + timeinneed)) / double(timeinneed);  //计算响应比
    }
    void setTime(int tin_, int ta_,int tfw_) {
        timeinneed = tin_;
        timearrive = ta_;
        timeforwait = tfw_;
    }
    char* getState() {
        return state;
    }
    double getPriority() const{
        return priority;
    }
    void setPid(int id) {
        pid = id;
    }
    int getPid() {
        return pid;
    }
    void setState( char* state_set) {
        strcpy_s(state, state_set);
    }
    int getTimearrive() {
        return timearrive;
    }
    int getTimeinneed() const {
        return timeinneed;
    }
    int getTimeforwait() {
        return timeforwait;
    }
    void Waiton() {
        timeforwait++;
    }
};
void SJF(vector<process> processline) {   //为非抢占式SJF算法
    bool empty = true; //判断是否有进程正常运行
    int countdown;
    double wholewait = 0;
    vector<process> processlist;
    for (int i = 0; !processline.empty()||!empty; i++) {
        int idx = 0; //用于遍历进程
        while (idx < processline.size()) {
            if (processline[idx].getTimearrive() == i) {
                processlist.push_back(processline[idx]);
                processlist.back().setState("Waiting");   //同时刻要到达的进程塞进vector
            }
            idx++;
        }
        if (empty) {
            if (!processlist.empty()) {
                empty = false;
                process tmp = processlist.front();  //等待队列中的第一个元素开始运行
                processlist.front().setState("Running");
                countdown = tmp.getTimeinneed();
            }
        }
        else {
            countdown--;
            if (countdown == 0) {
                empty = true;
                cout << "process " << processlist.front().getPid() << " is done! " << endl;
                cout << "Arrival time is " << processlist.front().getTimearrive() << endl;
                cout << "Waiting time is " << processlist.front().getTimeforwait() << endl;
                cout<<"Running time is " << processlist.front().getTimeinneed() << endl;
                wholewait += processlist.front().getTimeforwait();
                processlist.front().setState("Finished");    //进程已结束
                processlist.erase(processlist.begin());   
                processline.erase(processline.begin());   //将进程从总队列移除
                if (processlist.size()>0){
                    processlist.front().Waiton();    //这一秒，后面即将运行的线程也要计时等待
                }  

            }
        }
        int j = 1;
        while (j < processlist.size()) {
            processlist[j++].Waiton();   //等待队列的线程的等待时间全部加一
        }
        
    }
    cout << "average time for wait is" << wholewait / 10 << endl;
}
class findx {
public:
    findx(const int sam) { test = sam; }
    int GetPid() { return test; }
    bool operator()(process& Value) {
        if (Value.getPid()==test)
            return true;
        else
            return false;
    }
private:
    int test;
};
void HRRN(vector<process> processline) {   //为非抢占式HRRN算法
    bool empty = true; //判断是否有进程正常运行
    int countdown;
    double wholewait = 0;
    vector<process> processlist;
    for (int i = 0; !processline.empty() || !empty; i++) {
        int idx = 0; //用于遍历进程
        while (idx < processline.size()) {
            if (processline[idx].getTimearrive() == i) {
                processlist.push_back(processline[idx]);
                processline[idx].setState("Waiting");   //同时刻要到达的进程塞进vector
            }
            idx++;
        }
        int t = 0;
        while (t < processlist.size()) {
            processlist[t++].setPriority();    //计算响应比
        }
        sort(processlist.begin(), processlist.end(), [=](const process& t1, const process& t2) {return t1.getPriority() > t2.getPriority(); });   //根据响应比在最新队列进行排序，后面第一个进程开始执行后直接擦除，所以不会影响到正在进行的进程
        process tmp;
        if (empty) {
            if (!processlist.empty()) {
                empty = false;
                tmp = processlist.front();  //等待队列中的第一个元素开始运行
                vector<process>::iterator it = find_if(processline.begin(), processline.end(), findx(tmp.getPid()));  //找到正在运行的进程在processline的位置
                int it_locate = distance(processline.begin(), it);
                processline[it_locate].setState("Running");   //将其状态设置为运行中
                processlist.erase(processlist.begin());
                countdown = tmp.getTimeinneed();
            }
        }
        else {
            countdown--;
            if (countdown == 0) {
                empty = true;
                cout << "process " << tmp.getPid() << " is done! " << endl;
                cout << "Arrival time is " << tmp.getTimearrive() << endl;
                cout << "Waiting time is " << tmp.getTimeforwait() << endl;
                cout << "Running time is " << tmp.getTimeinneed() << endl;
                wholewait += tmp.getTimeforwait();
                auto it = find_if(processline.begin(), processline.end(), findx(tmp.getPid()));
                processline[distance(processline.begin(), it)].setState("Finished");
                processline.erase(processline.begin() + distance(processline.begin(), it));
            }
        }
        int j = 0;
        while (j < processlist.size()) {
            processlist[j++].Waiton();   //等待队列的线程的等待时间全部加一
        }

    }
    cout << "average time for wait is" << wholewait / 10 << endl;
}
int main()
{
    srand(time(NULL));
    vector<process> sample;
    process tmp;
    for (int i = 0; i < 10; i++) {
        tmp.setPid(rand()%99999);
        tmp.setTime(rand() % 40 + 10, rand() % 20 + 1,0);
        sample.push_back(tmp);
    }
    sort(sample.begin(), sample.end(), [=](const process& t1, const process& t2) {return t1.getTimeinneed() < t2.getTimeinneed(); });   //通过运行时间来排序
    cout << "***********************************SJF**********************************" << endl;
    SJF(sample);
    cout << "***********************************HRRN**********************************" << endl;
    HRRN(sample);
    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
