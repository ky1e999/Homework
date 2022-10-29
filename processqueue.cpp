// sys10-17.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <deque>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>
using namespace std;

class process {
private:
    int pid;
    string state;
    int priority; 
    int timeinneed;
    int timeforwait;
    int timeused;
public:
    void setPriority(int i) {
        priority = i;  //优先级
    }
    void setTime(int tin_, int tus_,int tfw_) {
        timeinneed = tin_;
        timeused = tus_;
        timeforwait = tfw_;
    }
    string getState() {
        return state;
    }
    int getPriority() const {
        return priority;
    }
    void setPid(int id) {
        pid = id;
    }
    int getPid() {
        return pid;
    }
    void setState(string state_set) {
        state = state_set;
    }
    void calTimeused(int time) {
        timeused += time;
    }

    int getTimeinneed() const {
        return timeinneed;
    }
    int getTimeused() {
        return timeused;
    }
    int getTimeforwait() {
        return timeforwait;
    }
    void Waiton(int time) {
        timeforwait+=time;
    }
};
deque<deque<process>> Processlist; //进程队列
mutex mtx1, mtx2;  //mtx1 为generator和scheduler 的互斥锁  mtx2为generator和executor的互斥锁
int processcount = 0;
int current = 0;
bool alldone = false;  
condition_variable_any cv;
int volume = 4;   //进程总数
void waittime(int level, process sample) {   //所有队列除了当前运行的都要加上时间片时间
    for (int x = 0; x < Processlist.size(); x++) {
        for (int y = 0; y < Processlist[x].size(); y++) {
            if (Processlist[x][y].getPid() != sample.getPid()) {
                Processlist[x][y].Waiton(int(pow(2.0, level)) * 10);
            }
        }
    }
    return;
}
string Executor(process& task, int listnum) {
    unique_lock<mutex> lock(mtx2);
    task.setState("running");  //运行中
    task.calTimeused(int(pow(2.0, listnum) * 10));
    std::this_thread::sleep_for(std::chrono::milliseconds(int(pow(2.0, listnum)) * 10));  //系统沉睡时间片时间
    waittime(listnum, task);
    if (task.getTimeused() >= task.getTimeinneed()) {
        task.setState("Done");
        cout << "Executor:  The Process "<<task.getPid() << " is done!  Total wait time is " << task.getTimeforwait() << "ms" << endl;
        return "done";  //若已用时间大于等于需要时间返回字符串done
    }
    else {
        cout << "Executor:  The Process " << task.getPid() << " cost " << int(pow(2.0, listnum) * 10) << "ms" << " , Going to queue ";
        if (listnum < 4) {
            cout << listnum + 2 << endl;
        }
        else {
            cout << 4 << endl;
        }
        return "running";
    }
}
void Scheduler() {
    while (!alldone) {
        if (processcount == volume && current == 0) {  //当 当前队列没有进程以及产生了足够的进程后，跳出循环
            alldone = true;
            break;
        }
        unique_lock<mutex> lk(mtx1);
        cv.wait(lk, [] {return current != 0; });//当 当前队列没有进程时 需要条件变量等待generator
        if (!Processlist[0].empty()) {
            if (Executor(Processlist[0].back(), 0) == "running") {
                Processlist[1].push_front(Processlist[0].back());  //前4个队列FCFS
                Processlist[1].front().setPriority(2);  //优先级改变
                Processlist[1].front().setState("waiting");  //状态改变 
            }
            else 
                current--;
            Processlist[0].pop_back();
        }
        else if (!Processlist[1].empty()) {
            if (Executor(Processlist[1].back(), 1) == "running") {
                Processlist[2].push_front(Processlist[1].back());  //前4个队列FCFS
                Processlist[2].front().setPriority(3);
                Processlist[2].front().setState("waiting");
            }
            else 
                current--;
            Processlist[1].pop_back();
        }
        else if (!Processlist[2].empty()) {
            if (Executor(Processlist[2].back(), 2) == "running") {
                Processlist[3].emplace_front(Processlist[2].back());  //前4个队列FCFS
                Processlist[3].front().setPriority(4);
                Processlist[3].front().setState("waiting");
            }
            else 
                current--;
            Processlist[2].pop_back();
        }
        else if (!Processlist[3].empty()) {
            if (Executor(Processlist[3].back(), 3) == "running") {
                Processlist[4].emplace_front(Processlist[3].back());  //前4个队列FCFS
                Processlist[4].front().setPriority(5);   
                Processlist[4].front().setState("waiting");
            }
            else {
                current--;
            }
            Processlist[3].pop_back();
        }
        else if (!Processlist[4].empty()) {
            if (Executor(Processlist[4].back(), 4) == "running") {
                Processlist[4].emplace_front(Processlist[4].back()); //最后一个队列round robin
                Processlist[4].front().setState("waiting");
            }
            else 
                current--;
            Processlist[4].pop_back();
        }
        else if (current == 0) {
            break;
        }
    }
}

void generator() {
    process task;
    int times;
    while (processcount < volume) {
        {
        times = rand() % 99 + 1;
        cout << "System sleeps for " << times << " ms " << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(times));
        lock_guard<mutex> lk1(mtx1);  //需要注意两个锁的顺序 先锁调度再锁执行 否则有一瞬间可能会调度没有锁 但是以及使用了executor函数 传送到了执行器
        task.setTime(rand() % 198 + 2, 1, 0);
        task.setPriority(1);
        task.setPid(rand() % 99999);
        task.setState("waiting");
        cout << "Generator: A new process is generated! " << "Id: " << task.getPid() << " Need " << task.getTimeinneed() << "ms to finish" << endl;
        {
            lock_guard<mutex> lk2(mtx2);
            Processlist[0].push_front(task);  //插入队列
        }
        processcount++;
        current++;
        cv.notify_one();  //通知scheduler说 generator以及产生了一个进程了 可以开始调度了
        times = rand() % 99 + 1;
        cout << "System sleeps for " << times << " ms " << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(times));
        }
        }
    return;
    
}
int main()
{
    srand(time(NULL));
    deque<process>d1, d2, d3, d4, d5;  //初始化5个队列
    Processlist.push_back(d1);
    Processlist.push_back(d2);
    Processlist.push_back(d3);
    Processlist.push_back(d4);
    Processlist.push_back(d5);
    thread gen_thr(generator);
    thread sce_thr(Scheduler);
    gen_thr.join();
    sce_thr.join();
    std::cout << "Hello World!\n";
    return 0;
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
