// sys09-21.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
using namespace std;
HANDLE se1 = nullptr;
HANDLE se2 = nullptr;
HANDLE se3 = nullptr;
HANDLE se4 = nullptr;
int N = 0;
 int count = 0;
void WINAPI printI(LPVOID lpParams) {
    if ( ::count >= N) {
        return ;
    }
    while (::count < N) {
        WaitForSingleObject(se1, INFINITE);
        cout << "I " ;
        ::count++;
        ReleaseSemaphore(se2, 1, nullptr);
    }
    return;
}
void WINAPI printLove(LPVOID lpParams) {
    if (::count >= N) {
        return;
    }
    while (::count < N) {
        WaitForSingleObject(se2, INFINITE);
        cout << "Love " ;
        ReleaseSemaphore(se3, 1, nullptr);
    }
    return;
}
void WINAPI printJinan(LPVOID lpParams) {
    if (::count >= N) {
        return;
    }
    while (::count < N) {
        WaitForSingleObject(se3, INFINITE);
        cout << "Jinan ";
        ReleaseSemaphore(se4, 1, nullptr);
    }
    return;
}
void WINAPI printUni(LPVOID lpParams) {
    if (::count >= N) {
        return;
    }
    while (::count < N) {
        WaitForSingleObject(se4, INFINITE);
        cout << "University" << endl;
        ReleaseSemaphore(se1, 1, nullptr);
    }
    return;
}
int main()
{
    printf("Please input Number:");
    scanf_s("%d", &N);
    se1 = CreateSemaphore(NULL, 1, 1, NULL);
    se2 = CreateSemaphore(NULL,0, 1, NULL);
    se3 = CreateSemaphore(NULL, 0, 1, NULL);
    se4 = CreateSemaphore(NULL, 0, 1, NULL);
    DWORD threadId = 0;
    HANDLE s1Thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)printI, nullptr, 0, &threadId);
    HANDLE s2Thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)printLove, nullptr, 0, &threadId);
    HANDLE s3Thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)printJinan, nullptr, 0, &threadId);
    HANDLE s4Thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)printUni, nullptr, 0, &threadId);
    system("pause");
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
