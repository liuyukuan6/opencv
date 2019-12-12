#include<opencv2/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>
#include<iostream>
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp>

#define COLORDETECT



#pragma once
class ColorDetector
{
private:
	// 允许要探测的颜色与目标颜色的最大差距
	int maxDist;

	// 目标颜色(BGR)
	cv::Vec3b target;

	//存储Lab色彩空间下的输入图像
	cv::Mat converted;
	bool useLab;//是否使用Lab色彩空间

	// 存储二值映像结果的图像
	cv::Mat result;

public:
#pragma region 构造函数
	//空结构函数，在此初始化默认参数
	ColorDetector():maxDist(100), target(0, 0, 0), useLab(false) {}

	//额外的构造函数，使用Lab色彩空间
	ColorDetector(bool useLab) :maxDist(100), target(0, 0, 0), useLab(useLab) {}

	//完整的构造函数
	ColorDetector(uchar blue,uchar green,uchar red,int mxDist=100,bool useLab=false):
		maxDist(mxDist), useLab(useLab)
	{
		//设置目标颜色
		setTargetColor(blue, green, red);
	}

#pragma endregion 
	// 计算与目标颜色的差距
	int getDistanceToTargetColor(const cv::Vec3b& color) const 
	{
		return getColorDistance(color, target);
	}
	 //图像处理函数，返回一个单通道二值图像
	cv::Mat process(const cv::Mat& image);

	//计算两个颜色之间的城区距离（RGB值差距的绝对值）
	//这里用cv::Vec3d存储三个无符号字符型，即颜色的RGB值
	//color1:要检测像素的颜色值
	//color2:目标颜色值
	int getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const
	{
		//||+||+||
		return abs(color1[0] - color2[0]) +
			abs(color1[1] - color2[1]) +
			abs(color1[2] - color2[2]);
	}
#pragma region 使用CIE L*a*b色彩空间和仿函数

	//重载运算符（），第二种计算颜色向量间的距离方法
	cv::Mat operator()(const cv::Mat& image)
	{
		cv::Mat input;

		//如果需要转换成Lab彩色空间
		if (useLab)
			cv::cvtColor(image, input, CV_BGR2Lab);
		else
			input = image;

		cv::Mat output;
		//计算输入图像与目标颜色的距离的绝对值
		cv::absdiff(input, cv::Scalar(target), output);
		//得到的是：每个通道与目标颜色对应通道的差值（|B|+|G|+|R|）

		//把输出结果图像的通道拆分成3副单通道图像，分别对应B,G,R
		std::vector<cv::Mat> images;
		cv::split(output, images);

		//3个通道的差值相加（这里可能出现饱和的情况）
		output = images[0] + images[1] + images[2];

		//对图像进行阈值化，化成二值图像
		cv::threshold(
			output,//输入图像
			output,//输出图像
			maxDist,//要探测的颜色与目标颜色的最大差距
			255,//要填充的最大值
			cv::THRESH_BINARY_INV//阈值化模式
		);
		//			  {	0		if output(x,y)>maxDist	大于允许颜色差距的像素都赋值为0
		//output(x,y)={
		//			  {	255		otherwise				小于允许颜色差距的像素都赋值为255
		//返回阈值化后的二值图像
		return output;


	}

#pragma endregion

#pragma region Getters and Setters
	//获取颜色差距的阈值
	int getColorDistanceThreshold() const {
		return maxDist;
	}

	//设置要探测的颜色与目标颜色的最大差距
	void setColorDistanceThreshold(int distance)
	{
		//最大差距必须是正数，否则就设为0
		if (distance < 0)
			distance = 0;

		maxDist = distance;
	}
	//获取目标颜色
	cv::Vec3b getTargetColor() const
	{
		return target;
	}

	//设置目标颜色
	void setTargetColor(cv::Vec3b color)
	{
		target = color;
	}

	//设置目标颜色（输入为RGB色彩空间）
	void setTargetColor(uchar blue, uchar green, uchar red)
	{
		//初始化目标颜色，次序为BGR
		target = cv::Vec3b(blue, green, red);
	
	}

};

