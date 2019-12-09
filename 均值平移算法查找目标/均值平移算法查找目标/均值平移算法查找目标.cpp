// 均值平移算法查找目标.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "colorhistogram.h"
#include "contentFinder.h"
#include "histogram.h"


int main()
{
#pragma region 均值平移法查找目标

	//读取参考图
	cv::Mat image = cv::imread("C:/Users/liuyukuan/Desktop/cat/1.jpg");
	if (!image.data)
		return 0;

	//标记猫脸部的ROI
	cv::Rect rect(50, 40, 150, 120);
	//取出ROI
	cv::Mat imageROI = image(rect);

	//显示标记了ROI的原始图像
	cv::rectangle(image, rect, cv::Scalar(255, 0, 0));
	cv::namedWindow("Image 1");
	cv::imshow("Image 1", image);


	///<得到直方图>

	int minSat = 65;//忽略饱和度颜色的色调分量，低于65的像素不考虑
	ColorHistogram hc;
	//计算ROI的色调值
	cv::Mat colorhist = hc.getHueHistogram(imageROI, minSat);

	//得到的直方图传给contentFinder类的实例
	ContentFinder finder;
	finder.setHistogram(colorhist);
#pragma endregion

#pragma region 均值平移法查找目标（处理第二幅图）

	//打开第二幅图
	image = cv::imread("C:/Users/liuyukuan/Desktop/cat/1_1.jpg");

	//将要处理的图像从RGB转换为HSV色彩空间
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	//得到色调直方图的反向投影
	int ch[1] = { 0 };//用第一个通道
	finder.setThreshold(-1.0f);//不做阈值化

	//对第二幅图像进行反向投影，由于没有阈值化，得到原始的概率分布图像
	cv::Mat result = finder.find(hsv, 0.0f, 180.f, ch);

	//显示第二幅图像反向投影的结果
	cv::namedWindow("Backprojection on second image");
	cv::imshow("Backprojection on second image", result);

	//在图像上标记了原始的ROI
	cv::rectangle(image, rect, cv::Scalar(255, 0, 0));

	//设置搜索条件
	cv::TermCriteria criteria(
		//定义了两个停止条件：迭代次数达到最大值；窗口中心的偏移值小于某个限值，满足一个即可停止搜索
		cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
		10,//最多迭代10次
		1);//或者重心移动距离一个像素

	//对概率分布图图像使用均值平移法，定义第二幅图像中猴子脸部的位置
	cv::meanShift(result, rect, criteria);//rect即使输入也是输出

	cv::rectangle(image,rect,cv::Scalar(0,255,0));
	cv::namedWindow("Image2 result");
	cv::imshow("Image2 result", image);

#pragma endregion


	cv::waitKey();

	return 0;
}

