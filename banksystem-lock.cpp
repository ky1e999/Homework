// sys-10-02.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <time.h>
using namespace std;
typedef struct member {
    char name[10];
    int money;
};
mutex mtx;
member family[7];
int balance = 10;
condition_variable_any cond;
int flag = 0;
void withdraw(member ppl) {
    lock_guard<mutex> locker(mtx);
    while (balance < ppl.money) {
        if (flag == 0) {
            cout << ppl.name << " is waiting" << endl;
            flag = 1;
        }
        cond.wait(mtx);
    }
    balance -= ppl.money;
    cout << ppl.name << " withdrew " << ppl.money << ", now the balance is " << balance << endl;
    flag = 0;
}
void Deposit(member ppl) {
    lock_guard<mutex>locker(mtx);
    balance += ppl.money;
    cout << ppl.name << " deposited " << ppl.money << ", now the balance is " << balance << endl;
    cond.notify_one();
    }
int main()
{
    srand(time(NULL));
    thread bank[14];
    family[0] = { "Father",10 };
    family[1] = { "Mother",20 };
    family[2] = { "Grandma",30 };
    family[3] = { "Grandpa",40 };
    family[4] = { "Uncle",50 };
    family[5] = { "Mary",50 };
    family[6] = { "Sally",100 };
    int countt[7] = { 0 };
    member operate[14];
    for (int i = 0; i < 14; i++) {
        int x;
        while (true) {

            x = rand() % 7;
            if (countt[x] < 2)
                break;
        }
        strcpy_s(operate[i].name , family[x].name);
        operate[i].money = family[x].money;
        countt[x]++;
    }
    for (int i = 0; i < 14; i++) {
        if (!strcmp(operate[i].name, family[5].name)) {
            bank[i] = thread(withdraw, ref(operate[i]));
        }
        else if (!strcmp(operate[i].name, family[6].name) ){
            bank[i] = thread(withdraw, ref(operate[i]));
        }
        else {
            bank[i] = thread(Deposit, ref(operate[i]));
        }
    }
    for (int i = 0; i < 14; i++) {
        bank[i].join();
    }
    return 0;

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
