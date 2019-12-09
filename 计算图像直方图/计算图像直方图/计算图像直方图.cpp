// 计算图像直方图.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include "histogram.h"


using namespace std;

int main()
{
	cv::Mat joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg", cv::IMREAD_GRAYSCALE);
	if (!joker.data)
		return 0;

	cv::namedWindow("joker");
	cv::imshow("joker", joker);


#pragma region 计算直方图（自定义类）
	//创建直方图对象
	Histogram1D h;

	//计算直方图
	cv::Mat histo = h.getHistogram(joker);//返回一个一维数组（列向量256*1）,包括256个元素

	//循环遍历每个箱子（bin）
	for (int i = 0; i < 256; i++)
	{
		//输出每个箱子中的像素个数
		cout << "Value" << i << "=" << histo.at<float>(i);
		i != 0 && i % 5 == 0 ? cout << endl : cout << "   ";
	};

#pragma endregion
#pragma region 显示直方图
	cv::namedWindow("Histogram");
		cv::imshow("Histogram", h.getHistogramImage(joker));


#pragma endregion

#pragma region 用直方图划分前景和背景
		///<重新显示直方图，应用一个选好的阈值>

		//获得直方图图像
		cv::Mat hi = h.getHistogramImage(joker);

		//画出阈值（60）所在的直线，即像素为60的位置
		cv::line(hi, cv::Point(60,0),cv::Point(60,510),cv::Scalar(128));

		//显示带有阈值的直方图
		cv::namedWindow("Histogram with thresold value");
		cv::imshow("Histogram with thresold value", hi);

		//创建带有阈值化的二值图像
		cv::Mat thresholded;

		//阈值化
		cv::threshold(
			joker, thresholded,//输入与输出图像
			100,//阈值
			255,//
			cv::THRESH_BINARY//阈值类型
		);

		//显示应用直方图阈值后的图像
		cv::namedWindow("Binary Image");
		cv::imshow("Binary Image", thresholded);

#pragma endregion


#pragma region 利用查找表修改图像外观
		//创建一个查找表
		cv::Mat lut(1, 256, CV_8U);//1*256的一维行数组

		//设置的查找表规则，图像反色
		for (int i = 0; i < 256; i++)
			//0->255,1->254,以此类推
			lut.at<uchar>(i) = 255 - i;

		cv::namedWindow("Negative image");
		cv::imshow("Negative image", h.applyLookUp(joker, lut));//应用查找表


#pragma endregion

#pragma region 伸展图像，提高图像对比度

		//伸展图像，使得黑色（值为0）的像素与白色（值为255）的像素占总像素的1%
		cv::Mat str = h.stretch(joker, 0.1f);

		cv::namedWindow("Stretched Image");
		cv::imshow("Stretched Image", str);

		cv::namedWindow("Stretched H");
		cv::imshow("Stretched H", h.getHistogramImage(str));

#pragma endregion

#pragma region 直方图均衡化

		//均衡化图像
		cv::Mat eq = h.equalize(joker);
		//或者
		//cv::Mat eq=HistogramlD::equalize(image);

		//显示直方图均衡后的图像
		cv::namedWindow("Equalize Image");
		cv::imshow("Equalize Image", eq);

		cv::namedWindow("Equalize Histogram Image");
		cv::imshow("Equalize Histogram Image", h.getHistogramImage(eq));




#pragma endregion
	cv::waitKey();

	return 0;
}

