// alg09-22.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstdlib>  //随机数的包
using namespace std;
void swap(int* src, int a, int b) {
	int tmp = a;
	a = b;
	b = tmp;
}
void array_swap(int* src, int a, int b) {
	int tmp = src[a];
	src[a] = src[b];
	src[b] = tmp;
}
void quicksort(int *src, int beg, int end) {
	if (beg < end) {
		srand(time(0));
		int position = (rand() % (end - beg)) + beg; //随机设置基准数位置
		swap(src, position, beg);    //将基准数与最低位互换
		int key = src[beg];
		int i = beg;
		int j = end;
		while (i < j) {
			while (i<j && src[j]>key) {   //后面往前找到比基准数小的
				j--;
			}
			if (i < j) {
				src[i] = src[j];
				i++;
			}
			while (i < j && src[i] < key) {  //前面往后找到比基准数大的
				i++;
			}
			if (i < j) {
				src[j] = src[i];
				j--;
			}
		}
		src[i] = key;
		quicksort(src, i + 1, end);		//开始递归
		quicksort(src, beg, i - 1);
	}
}
void bubblesort(int* src,int len) {
	for (int i = 0; i < len; i++) {
		for (int j = 0; j <len-1-i; j++) {
			if (src[j] >src[j+1]) {
				array_swap(src, j,j+1);
			}
		}
	}
	return;
}

int main()
{
	std::cout << "please input an array of int:" << endl;
	int sample[500];
	int x = 0; //x记录长度
	char c = 0; 
	int tmp = 0;
	std::cin >> sample[x++]; //第一个字符
	while (c = getchar() != '\n') {  //直至换行，第二个字符
		std::cin >> tmp;  //第三个字符，依次进行
		sample[x++] = tmp;
	}
	cout<<"QuickSort:"<<endl;
	quicksort(sample, 0, x- 1);
	for (int i = 0; i < x; i++) {
		std::cout << sample[i] << " ";
	}
	cout<<"\nBubbleSort:"<<endl;
	bubblesort(sample,x);
	for (int i = 0; i < x; i++) {
		std::cout << sample[i] << " ";
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
