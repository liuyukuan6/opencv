// 初级入门（3）.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main()
{
#pragma region 定义感兴趣区域（Region of Interest,ROI）

	/*
		要把一个小图像复制到一个大图向上。例如要把一个logo插入到测试图像中
	*/

	cv::Mat image = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat logo = cv::imread("C:/Users/liuyukuan/Desktop/cat/1.jpg", cv::IMREAD_GRAYSCALE);

	//在图像的右下角定义一个ROI（初始化），放logo的位置
	cv::Mat imageROI(image,//ROI 左上角坐标
		cv::Rect(image.cols - logo.cols, image.rows - logo.rows, logo.cols,logo.rows));//ROI 大小（宽度和高度）
	/*ROI 实际上就是一个cv::Mat 对象，他与他的父母图像指向同一个数据缓存区，并且在头部表明了ROI的坐标。
	*/

	//插入logo，将logo这幅图像拷贝到image上定义的一个ROI（ImageROI）中去
	logo.copyTo(imageROI);

	cv::namedWindow("Image");//define an image window
	cv::imshow("Image", image);
	cv::waitKey(0);

#pragma region 图像掩码
	//重新读入原始图像
	image = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg", cv::IMREAD_GRAYSCALE);

	//在图像的右下角定义一个ROI（赋值）
	imageROI = image(cv::Rect(image.cols-logo.cols,image.rows-logo.rows,//ROI 左上角坐标
		logo.cols,logo.rows));//ROI 大小（宽度高度）

	//ROI还可以用行和列的值域来描述
	/*imageROI=image(cv::Range(image.rows-logo.rows,image.rows),//ROI 行（高度）范围
					 cv::Range(image.cols-logo.cols,image.cols));//ROI 列（宽度）范围
					 */

	//把标志作为掩码（必须是灰度图像）
	cv::Mat mask(logo);//浅拷贝，mask和logo指向同一副图像

	//插入标志，只复制掩码mask中不为0的位置
	logo.copyTo(imageROI, mask);
	//logo和mask是同一幅图像，都是二值图像，即像素值不是0就是255，只有黑色和白色组成
	//这里相当于只复制了logo中值为255的像素

	cv::imshow("Image", image);
	cv::waitKey(0);

#pragma endregion
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
