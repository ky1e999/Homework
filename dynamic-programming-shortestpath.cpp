// alg-10-02.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <deque>
#define INFINITE 9999
#define MAXvex 100
void shortest(int graph[MAXvex][MAXvex], int start, int dest, int vecnum) {
    std::deque<int> tmp_des; //存放第k次计算的临时终点
    std::deque<int> tmp_sta;//存放第k次计算的临时起始点
    int path[MAXvex]; //存放路径
    int flag_repeat=0; //用于判断点是否重复
    tmp_des.push_back(dest);
    while (true) {
        while (tmp_des.size() > 0) {      //while循环找上一层顶点
            int j = tmp_des.front();
            for (int i = j - 1; i >= start; i--) {
                if (graph[i][j] < INFINITE) {
                    int x = 0;
                    flag_repeat = 0;
                    while (x < tmp_sta.size()) {  //判断新一轮的始顶点是否已存在队列中
                        if (tmp_sta.at(x) == i) {
                            flag_repeat = 1;
                            break;
                        }
                        x++;
                    }
                    if (flag_repeat == 0) {  //如果上一层的顶点队列中没有重复则存放
                        tmp_sta.push_front(i);
                    }
                }
            }
            tmp_des.pop_front();
        }
        while (tmp_sta.size() > 0) { 
            int startpoint = tmp_sta.front();
            for (int i = dest; i > startpoint; i--) {
                if (graph[startpoint][i] < INFINITE) {
                    if (graph[startpoint][i] + graph[i][dest] < graph[startpoint][dest]) {  //比较路径长短，找出最短路径
                        graph[startpoint][dest] = graph[startpoint][i] + graph[i][dest];
                        path[startpoint] = i;
                    }
                }
            }
            tmp_des.push_back(startpoint);
            if (tmp_sta.front() == start)
                goto breakloop; //跳出双重循环
            tmp_sta.pop_front();
        }
    }
breakloop:std::cout << "shortest path is found!" << std::endl;
    std::cout << "顶点" << start << " 到顶点" << dest << " 的最短距离为  " << graph[start][dest] << std::endl;
    std::cout << "路径为" << start;
    int pathpoint=start;
    while (path[pathpoint] <=dest&&path[pathpoint]>=0) {  //路径数组中倒数第二个节点Path[k] 没有存放destination
        std::cout << "->" << path[pathpoint];
        pathpoint = path[pathpoint];
    }
    std::cout << "->" << dest;
    return;

}
int main()
{
    int graph[MAXvex][MAXvex];
    for (int i = 0; i < MAXvex; i++) { //初始化图
        for (int j = 0; j < MAXvex; j++) {
            if(i==j)
                graph[i][j] = 0;
            else
            {
                graph[i][j] = INFINITE;
            }
        }
    }
    int vexnum;
    std::cout << "请输入总顶点数:" ;
    std::cin >> vexnum;
    
    char ch=0;
    int start = 0;
    int dest = 0;
    int x, y;
    while (true) {
        std::cout << "请输入边的起点与终点(输入0,0)结束输入：";
        scanf_s("%d,%d", &start,&dest);
        if (start >= vexnum || dest >= vexnum) {
            std::cout << "Out of the range!" << std::endl;
            continue;
        }
        if (start == 0 && dest == 0) {
            graph[0][0] = 0;
            std::cout << "Info is filed in successfully!" << std::endl;
            break;
        }
        std::cout << "请输入边的权值:";
        scanf_s("%d",&graph[start][dest]);
    }
    std::cout << "Please input the start and destination" << std::endl;
    std::cout << "start:";
    scanf_s("%d", &x);
    std::cout << "destination:";
    scanf_s("%d", &y);
    shortest(graph, x, y, vexnum);
    std::cout << "Graph:" << std::endl;
    for (int i = 0; i < vexnum; i++) {
        for (int j = 0; j < vexnum; j++) {
            std::cout << graph[i][j] << "\t";
        }
        std::cout << std::endl;
    }
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
