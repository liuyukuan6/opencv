#pragma once

#define COLGISTOGRAM

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>


class ColorHistogram
{
private:

	///<彩色图像，3个通道>
	int histSize[3];	//直方图每个维度（通道）上的箱子数量
	float hranges[2];	//直方图每个维度（通道）上像素的值范围（一般三个维度用同一个范围）
	const float* ranges[3];//指针数组，存储每个维度上指向直方图像素的值范围的指针
	int channels[3];	//需要处理的通道数量

public:

	ColorHistogram()
	{	
		//准备彩色图像的参数
		//每个通道的箱子数量的大小和范围是相等的
		histSize[0] = histSize[1] = histSize[2] = 256;//默认的直方图中每个通道的箱子数量相等

		//直方图每个维度上的像素值范围
		hranges[0] = 0.0;
		hranges[1] = 256.0;

		//用指针数组来存储每个维度像素值的范围
		//本类中所有通道的范围相同
		ranges[0] = hranges;
		ranges[0] = hranges;
		ranges[0] = hranges;

	
		//彩色通道
		channels[0] = 0;//B
		channels[1] = 1;//G
		channels[2] = 2;//R
	
	}

	void setSize(int size)
	{
		histSize[0] = histSize[1] = histSize[2] = size;
	}

	//计算直方图
	cv::Mat getHistogram(const cv::Mat& image)
	{
		
		cv::Mat hist;

		//用calcHist函数计算一维直方图
		cv::calcHist(
				&image,			//要计算直方图的图像
				1,				//计算多少幅图像的直方图
				channels,		//使用的通道(灰度图像，只有一个通道)
				cv::Mat(),		//不使用掩码(如果使用，只有部分筛选出来的像素会用来计算直方图)
				hist,			//作为输出结果的直方图
				1,				//直方图的维数（通道数），这是三维的直方图（与彩度图像对应）
				histSize,		//直方图每个维度（通道）上的箱子数量（一维，256个箱子）
				ranges			//直方图像素值的范围（计算值在那些范围内的像素，这里是(0,256)）
		);
		return hist;
	
	}
	//计算一维色调直方图
//minSaturation：饱和度阈值，忽略低饱和度的像素
	cv::Mat getHueHistogram(const cv::Mat& image, int minSaturation = 0)
	{
		//创建要计算的直方图
		cv::Mat hist;

		//转换为HSV色彩空间（要计算色调直方图）
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);

		//掩码（二值图像）
		cv::Mat mask;

		//根据需要创建验码
		if (minSaturation > 0)
		{
			//将三个通道分割进3副图像
			std::vector<cv::Mat>v;
			cv::split(hsv, v);

			//用阈值法屏蔽低饱和度的像素，饱和度小于minSaturation的像素就不考虑了
			cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);


		}

		///<准备一维色调直方图的参数>

		//色调的范围为0~180
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		//色调通道（只考虑一个通道）
		channels[0] = 0;

		cv::calcHist(
			&hsv,			//要计算直方图的图像(HSV色彩空间)
			1,				//计算多少副图像的直方图（仅计算一幅图像的直方图）
			channels,		//使用的通道(只有一个色调空间)
			mask,			//使用掩码
			hist,			//作为输出结果的直方图
			1,				//直方图的维数
			histSize,		//直方图每个维度上的箱子数量
			ranges			//直方图像素值的范围（计算在哪些范围内的像素，这里是[0,180]）
		);

		return hist;
	}

};

