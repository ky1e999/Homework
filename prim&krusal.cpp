// alg10-21.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#define INFINITE 999
#define MAX 100
int Graph[MAX][MAX];
int ans[MAX][MAX];
int find(std::vector<int> arr,int tar) {  //寻找元素是否在队列之中
    int i = 0;
    while (i < arr.size()) {
        if (arr[i++] == tar) {
            return i;
        }
    }
    return -1;
}
int minedge(int vex,int above) {  //寻找矩阵中大于above的最小元素
    int min = INFINITE;
    for (int i = 0; i < vex; i++) {
        for (int j = 0; j < vex; j++) {
            if (min < Graph[i][j]&&min>above) {
                min = Graph[i][j];
            }
        }
    }
    return min;
}
void Krusal(int vexnum) {  //Krusal算法
    std::vector<int> linked;
    int next = 0;
    while (linked.size() < vexnum) {  //当已连接的结点数量为顶点数量时结束循环
        int min = minedge(vexnum, next);  //找出图中目前除了上一次操作的最小距离
        for (int i = 0; i < vexnum; i++) {
            for (int j = 0; j < vexnum; j++) {
                if (Graph[i][j] == min) {  //当权值等于相对最小距离时 将两点塞进队列
                    if(find(linked,i)==-1)
                        linked.push_back(i);
                    if(find(linked,j)==-1)
                        linked.push_back(j);
                    ans[i][j] = Graph[i][j];
                }
            }
        }
        next = min;  //最小距离相对值为这次操作的最小值
    }
    std::cout << "Krusal Result:" << std::endl;
    for (int i = 0; i < vexnum; i++) {
        for (int j = 0; j < vexnum; j++) {
            std::cout << ans[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
void Prim(int start,int vex) {
    std::vector<int> linked;
    linked.push_back(start);  //将起始点塞进队列
    while (linked.size()<vex) {  //当已连接的结点数量为顶点数量时结束循环
        int min=INFINITE, min_sta, min_fin;
        for (int i = 0; i < linked.size(); i++) {
            int tmp_sta = linked[i];  //临时起点为已连接队列中的元素
            for (int j = 0; j < vex; j++) {
                if (find(linked, j) == -1&&min>Graph[tmp_sta][j]) {  //当j不在已连接队列中
                    min = Graph[tmp_sta][j];
                    min_sta = tmp_sta;
                    min_fin = j;
                }
            }
        }
        linked.push_back(min_fin);
        ans[min_sta][min_fin] = Graph[min_sta][min_fin];
    }
    std::cout << "Prim Result:" << std::endl;
    for (int i = 0; i < vex; i++) {
        for (int j = 0; j < vex; j++) {
            std::cout << ans[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}
int main()
{
    for (int i = 0; i < MAX; i++) { //初始化图
        for (int j = 0; j < MAX; j++) {
                Graph[i][j] = INFINITE; 
                ans[i][j] = INFINITE;
        }
    }
    int vexnum;
    std::cout << "请输入总顶点数:";
    std::cin >> vexnum;
    int start = 0;
    int dest = 0;
    while (true) {
        int length;
        std::cout << "请输入边的起点与终点与权值(输入0,0,0)结束输入：";
        scanf_s("%d,%d,%d", &start, &dest,&length);
        if (start >= vexnum || dest >= vexnum) {
            std::cout << "Out of the range!" << std::endl;
            continue;
        }
        if (start == 0 && dest == 0) {
            std::cout << "Info is filed in successfully!" << std::endl;
            break;
        }
        Graph[start][dest] = length;
        Graph[dest][start] = length;
    }
    Prim( 0, vexnum);
    Krusal( vexnum);
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
