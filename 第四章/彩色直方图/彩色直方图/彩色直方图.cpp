// 彩色直方图.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include "histogram.h"
#include "contentFinder.h"
#include "colorhistogram.h"


using namespace std;

int main()
{
#pragma region 反向投影直方图检测灰度图像

	//读入输入图像（灰度）
	cv::Mat joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg",cv::IMREAD_GRAYSCALE);
	if (!joker.data)
		return 0;

	//定义感兴趣区域(ROI，小丑头像)
	cv::Rect ROI = cv::Rect(270, 40, 450, 500);

	//取出感兴趣的区域(ROI)
	cv::Mat jokerROI;
	jokerROI = joker(ROI);

	//直方图
	Histogram1D h;
	cv::Mat hist = h.getHistogram(jokerROI);
	cv::namedWindow("joker histogram");
	cv::imshow("joker histogram", h.getHistogramImage(jokerROI));

	//创建内容搜索器
	ContentFinder finder;

	//设置用来反向投影的直方图
	finder.setHistogram(hist);
	//设置阈值化处理用的阈值
	finder.setThreshold(0.12f);

	//创建输出图像（二值图像）
	cv::Mat result;
	//进行反向投影，得到一个二值图像（白色为相似区域，黑色为非相似区域）
	result = finder.find(joker);

	cv::namedWindow("Detection Result");
	cv::imshow("Detection Result", result);
	
	//将感兴趣的区域显示在原始图像上
	cv::rectangle(joker, ROI, cv::Scalar(0, 0, 0));
	//显示画了感兴趣区域的原始图像

	cv::namedWindow("joker");
	cv::imshow("joker", joker);
	cv::waitKey();


#pragma endregion
  
#pragma region

	//读入彩色图像
	ColorHistogram hc;
	cv::Mat color = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");


	//定义感兴趣区域(ROI，小丑头像)
	cv::Rect ROI2 = cv::Rect(270, 40, 450, 500);

	//取出感兴趣的区域(ROI)
	jokerROI = joker(ROI2);

	//计算3D颜色直方图(每个通道有8个箱子)
	hc.setSize(8);
	cv::Mat shist = hc.getHistogram(jokerROI);

	//设置用来反向投影的直方图
	finder.setHistogram(shist);
	finder.setThreshold(0.05f);

	//进行反向投影，得到一个二值图像（白色为相似区域）
	result = finder.find(color);

	cv::namedWindow("joker hist");
	cv::imshow("joker hist", result);
	cv::waitKey();

#pragma endregion
}


