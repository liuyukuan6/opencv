// 用 Canny 算子检测图像轮廓.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "edgedetector.h"


int main()
{
	cv::Mat road = cv::imread("road.jpg",cv::IMREAD_GRAYSCALE);
	cv::imshow("Original road", road);

#pragma region sobel测试轮廓

	// Compute Sobel
	EdgeDetector ed;
	ed.computeSobel(road);

	// Display the Sobel orientation
	cv::namedWindow("Sobel (orientation)");
	cv::imshow("Sobel (orientation)", ed.getSobelOrientationImage());
	cv::imwrite("ori.bmp", ed.getSobelOrientationImage());

	// Display the Sobel low threshold
	cv::namedWindow("Sobel (low threshold)");
	cv::imshow("Sobel (low threshold)", ed.getBinaryMap(125));

	// Display the Sobel high threshold
	cv::namedWindow("Sobel (high threshold)");
	cv::imshow("Sobel (high threshold)", ed.getBinaryMap(350));

#pragma endregion

#pragma region canny
	/*它的核心理念是用两个不同的阈值来判断哪个点属于轮廓，一个是低阈值，一个是高阈值。*/
	/*具体做法是在低阈值边缘分布图上只保留具有连续路径的边缘点，同时把那些边缘点连接到属于高阈值边缘分布图的边缘上。*/

	cv::Mat contours;
	cv::Canny(road, // 灰度图像
		contours, // 输出轮廓
		125, // 低阈值
		350); // 高阈值

	cv::imshow("canny", 255 - contours);

	cv::waitKey();
#pragma endregion

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
