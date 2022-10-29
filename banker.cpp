// sys10-25.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <stdlib.h>
using namespace std;
int m = 3;
int n = 5;
vector<int> Available;
vector<vector<int>> Max(n,vector<int>(m));
vector<vector<int>> Allocation(n,vector<int>(m,0));
vector<vector<int>> Need(n,vector<int>(m));
vector<vector<int>> Max_sam = { {5,5,9},{5,3,6},{4,0,11},{4,2,5},{4,2,4} };
vector<vector<int>> Alloc_sam = { {2,1,2},{4,0,2},{2,0,4},{4,0,5},{3,1,4} };
vector<vector<int>> need_sam = { {3,4,7},{1,3,4},{0,0,6},{2,2,1},{1,1,0} };
vector<int> ava_sam = { 2,3,3 };
void init() {
    Available = { rand() % 10 + 1,rand() % 10 + 1,rand() % 10 + 1 };
    cout << "*******Initial Available********" << endl;
    for (int i = 0; i < m; i++) {
        cout << ava_sam[i] << "\t";
    }
    for (int i = 0; i < n; i++) {
        Max[i] = { rand() % Available[0] + 1,rand() % Available[1] + 1,rand() % Available[2] + 1};
    }
    int flag = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (Available[j] == 0)
                continue;
            Allocation[i][j] = rand() % 2 * (rand() % Max[i][j] + 1);
            if (Available[j] >= Allocation[i][j]) 
                Available[j] -= Allocation[i][j];
            else
                Allocation[i][j] = 0;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
    cout << endl;
    cout << "********Max*********" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << Max_sam[i][j] << "\t" ;
        }
        cout << endl;
    }
    cout << "********Allocation*********"<<endl ;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << Alloc_sam[i][j] << "\t" ;
        }
        cout << endl;
    }
    cout << "********Need*********" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << need_sam[i][j] << "\t" ;
        }
        cout << endl;
    }
    //cout << "*******Available********" << endl;
    //for (int i = 0; i < m; i++) {
    //    cout << Available[i] << "\t" ;
   // }
    //cout << endl;
}

void banker() {
    vector<int> done;
    vector<int>temp(ava_sam);
    for(int i=0;i<n;i++) {
        if (find(done.begin(),done.end(),i)==done.end()&&temp[0] >= need_sam[i][0] && 
            temp[1] >= need_sam[i][1] && temp[2] >= need_sam[i][2]) {
            done.push_back(i);
            temp[0] += Alloc_sam[i][0];
            temp[1] += Alloc_sam[i][1];
            temp[2] += Alloc_sam[i][2];
            i = -1;
        }
    }
    if (done.size() != n) {
        cout << "DeadLock!" << endl;
        return;
    }
    cout << done[0]+1 ;
    for (int i = 1; i < done.size(); i++) {
        cout << " --> " << done[i]+1;
    }
    cout << endl;
}
int main()
{
    srand(time(NULL));
    init();
    banker();
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
