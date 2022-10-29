// sys09-26.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef struct member {
    char name[10];
    int money;
    int limit;
};
CRITICAL_SECTION bank;
member family[7];
int balance = 10;
HANDLE thread_fam[7 * 2];
DWORD WINAPI withdraw(LPVOID lpParam) {
    member* ppl = (member*)lpParam;
    if (ppl->limit <= 0) {
        return 0;
    }
    else {
        EnterCriticalSection(&bank);
        balance -= ppl->money;
        ppl->limit--;
        cout << ppl->name << " withdrew " << ppl->money << ", now the balance is " << balance << endl;
        LeaveCriticalSection(&bank);
        Sleep(100);
        return 1;
    }
}
DWORD WINAPI Deposit(LPVOID Member) {
    member* ppl = (member*)Member;
    if (ppl->limit <= 0) {
        return 0;
    }
    else {
        EnterCriticalSection(&bank);
        balance += ppl->money;
        ppl->limit--;
        cout << ppl->name << " deposited " << ppl->money << ", now the balance is " << balance << endl;
        LeaveCriticalSection(&bank);
        Sleep(100);
        return 1;
    }
}
int main()
{
    family[0] = { "Father",10,2 };
    family[1] = { "Mother",20,2 };
    family[2] = { "Grandma",30,2 };
    family[3] = { "Grandpa",40,2 };
    family[4] = { "Uncle",50,2 };
    family[5] = { "Mary",50,2 };
    family[6] = { "Sally",100,2 };
    InitializeCriticalSection(&bank);
    int i = 0;
    DWORD ThreadID;
    while (true) {
        if (i >= 14) {
            break;
        }
        else {
            int x = rand() & 7;
            if (x < 5 && family[x].limit>0) {
                thread_fam[i] = CreateThread(NULL, 0, Deposit, &family[x], 0, &ThreadID);
                WaitForSingleObject(thread_fam[i],INFINITE);
                i++;
            }
            else if (x >= 5 && x < 7 && family[x].limit>0) {
                thread_fam[i] = CreateThread(NULL, 0, withdraw, &family[x], 0, &ThreadID);
                WaitForSingleObject(thread_fam[i], INFINITE);
                i++;
            }
        }
    }
    DeleteCriticalSection(&bank);
    for (int i = 0; i < 14; i++) {
        CloseHandle(thread_fam[i]);
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
