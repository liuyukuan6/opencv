// 比较直方图搜索相似图像.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include "imageCompare.h"



#define NumOfImgs 6

int main()
{
	cv::Mat joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");
	if (!joker.data)
		return 0;

	cv::namedWindow("joker");
	cv::imshow("joker", joker);

	//创建直方图比较类的对象
	ImageComparator c;
	//设置参考图像
	c.setReferenceImage(joker);

	///<读入其他图像并于参考图像进行直方图比较，输出相似度>
	const char* images[NumOfImgs] = {
	"C:/Users/liuyukuan/Desktop/cat/1.jpg","C:/Users/liuyukuan/Desktop/cat/1_1.jpg","C:/Users/liuyukuan/Desktop/cat/2.jpg",
	"C:/Users/liuyukuan/Desktop/cat/3.jpg","C:/Users/liuyukuan/Desktop/cat/4.jpg","C:/Users/liuyukuan/Desktop/cat/5.jpg"
	};

	const char* imgNames[NumOfImgs] = {
	"1","1_1","2","3","4","5"
	};

	for (int i = 0; i < NumOfImgs; i++)
	{
		cv::Mat input = cv::imread(images[i]);
		std::cout << "3 vs" << imgNames[i] << ":" << c.compare(input) << std::endl;
		cv::namedWindow(imgNames[i]);
		cv::imshow(imgNames[i], input);

	}

	cv::waitKey();
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
