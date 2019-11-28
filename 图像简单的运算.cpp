// 图像简单的运算.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>

int main()
{
	cv::Mat joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/1.jpg");
	cv::Mat rain = cv::imread("C:/Users/liuyukuan/Desktop/cat/1_1.jpg");

	cv::namedWindow("joker");
	cv::imshow("joker", joker);
	cv::namedWindow("rain");
	cv::imshow("rain", rain);
	cv::waitKey();

#pragma region 图像相加
	//图像相加
	cv::Mat result;
	//两幅图像相加（使用函数）
	//要求两张图的像素一样，渠道一样
	cv::addWeighted(joker,0.7,rain,0.9,0.,result);//result=0.7*joker+0.9*image2+0.

	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::waitKey();
   
	//两幅图像相加（使用重载运算符）
	cv::Mat result2 = 0.7 * joker + 0.9 * rain + 0;

	cv::namedWindow("result2");
	cv::imshow("result2", result2);
	cv::waitKey();
#pragma endregion

#pragma region 分割图像通道
	rain = cv::imread("C:/Users/liuyukuan/Desktop/cat/1_1.jpg",cv::IMREAD_GRAYSCALE);

	// 创建三幅图像的向量
	std::vector<cv::Mat> planes;
	// 将一个三通道图像分割为三个单通道图像
	cv::split(joker, planes);
	// 将图1（灰度，单通道）的蓝色通道加到2上
	planes[0] += rain;
	// 将三个单通道图像合并为一个三通道图像
	cv::merge(planes, result);

	cv::namedWindow("result3");
	cv::imshow("result3", result);
	cv::waitKey();

#pragma endregion
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
