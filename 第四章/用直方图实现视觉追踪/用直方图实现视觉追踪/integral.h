#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
//转换为二值图层组成的多通道图像
//nPlanes必须是2的幂
//input是输入的灰度图像
void convertToBinaryPlanes(const cv::Mat& input, cv::Mat& output, int nPlanes)
{
	///<减色运算，将灰度图像数量减少为256/2^n,>
	//本例中只用到了16个箱子，所以先把灰度图像先减少为16个

	//需要屏蔽的位数 n=8-log(16)/log(2)=4
	int n = 8 - static_cast<int>(log(static_cast<double>(nPlanes)) / log(2.0));
	//用来消除最低有效位的掩码
	uchar mask = 0xFF << n;
	//0xFF         1 1 1 1 1 1 1 1
	//0xFF << 4    1 1 1 1 0 0 0 0
	std::cout << "mask=0x" << std::hex << static_cast<int>(mask) << std::endl;

	//消除最低有效位，减色，相当于灰度颜色数量减为256/2^n
	cv::Mat reduced = input & mask;

	//Test减色效果

	//创建一个图像向量
	std::vector<cv::Mat>planes;





}