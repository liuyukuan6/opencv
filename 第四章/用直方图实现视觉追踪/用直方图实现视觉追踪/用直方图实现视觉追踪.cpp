// 用直方图实现视觉追踪.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "histogram.h"
#include "integral.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



int main()
{
	cv::Mat image = cv::imread("bike55.bmp", cv::IMREAD_GRAYSCALE);

	///<定义图像的ROI>

	//ROI的左上角坐标
	int x0 = 97, y0 = 112;
	//ROI的高度，宽度
	int width = 25, height = 30;
	//创建ROI，在图像中为骑自行车的女孩
	cv::Mat roi(image, cv::Rect(x0, y0, width, height));

	///<获取ROI的直方图>

	//创建灰度直方图对象
	Histogram1D h;
	//设置创建直方图的箱子为16个
	h.setNBins(16);
	//计算原图像ROI中的直方图
	cv::Mat refHistogram = h.getHistogram(roi);

	cv::namedWindow("Reference Histogram");
	cv::imshow("Reference Histogram", h.getHistogramImage(roi,16));//放大16倍

	//读入二幅图像（灰度）
	cv::Mat secondImage = cv::imread("boke65.bmp", cv::IMREAD_GRAYSCALE);
	if (!secondImage.data)
		return 0;

	/*考虑到要计算大量的直方图，为了提高运算效率，可以把图像转换为
	由二值平面组成的多通道图像，每个通道关联直方图的一个箱子*/

	//创建16个通道的二值图像
	cv::Mat planes;

	//将目标图像转化为16个通道的0-1二值图像，每个通道对应一个箱子
	//且通道中值为1的像素表示属于该箱子的像素，值为0的像素表示不属于该箱子的像素
	convertToBinaryPlanes(secondImage, planes, 16);

	//然后计算16个通道（箱子）的积分图像
	/*
		1、一个通道对应一个箱子，16通道为由0-1构成的16个二值图像
		2、计算16个通道的积分图像后，当我们在目标函数上遍历时，每遍历一个与原图像ROI相同的区域，只要用到四个点的加减
		和运算就可以算出该区域内的直方图，也就是每个通道（箱子）上属于该箱子的像素加和的一个一维数组
	*/
	IntegralImage<float, 16>intHistogram(planes);//调用构造函数

	double maxSimilarity = 0.0;//最大相似度
	int xbest, ybest;//最佳位置（左上角）的坐标
	cv::Vec<float, 16>histogram;

	//遍历原始图像中女孩位置周围的水平长条
	for (int y = 110; y < 120; y++)
	{
		for (int x = 0; x < secondImage.cols - width; x++)
		{
			//用积分图像计算16个箱子的直方图
			histogram = intHistogram(x, y, width, height);

			//计算与基准直方图的位置
			double distance = cv::compareHist(refHistogram, histogram, cv::HISTCMP_INTERSECT);

			//找到最相似直方图的位置
			if (distance > maxSimilarity)//新位置的相似度大于老位置
			{
				//更新最佳位置（左上角）的坐标
				xbest = x;
				ybest = y;
				//更新相似度
				maxSimilarity = distance;
			}
		}
	}

	//输出最佳位置（左上角）和最大相似度
	std::cout << "Best solution = (" << xbest << "," << ybest << ") =" << maxSimilarity << std::endl;


	cv::waitKey();
	return 0;
}
