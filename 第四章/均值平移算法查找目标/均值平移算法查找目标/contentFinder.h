#pragma once


#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram.h"

class ContentFinder
{
private:
	///<直方图参数，最多可以处理三个通道>
	float hranges[2];//直方图每个维度（通道）上像素的范围（一般三个维度用同一个像素）
	const float* ranges[3];//指针数组，存储每个维度（通道）上指向直方图像素的值范围的指针
	int channels[3];//需要处理的通道数量

	float threshold;//做阈值化处理用的阈值
	cv::Mat histogram;//输入直方图

public:
	//构造函数
	ContentFinder() :threshold(0.1f)
	{
		//直方图每个维度（通道）上像素的值范围[0,256)
		hranges[0] = 0.0;
		hranges[1] = 256.0;
	

		//本类中所有通道的范围相同
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;

		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
	
	}
#pragma region setters and gatters
	//设置做阈值化处理的阈值，范围为[0,1]
	void setThreshold(float t)
	{
		threshold = t;
	}
	//获取做阈值化处理用的阈值
	float getThreshold() { return threshold; }

	//设置要使用的直方图，设置的同时进行归一化处理
	void setHistogram(const cv::Mat& h)
	{
		//归一化
		cv::normalize(h, histogram, 1.0);
	}
#pragma endregion

	//简化入口函数，使用构造函数的默认初始化参数
	//也可以调用下面的find()函数，只不过输入的参数较多，这个简化版使用默认参数
	cv::Mat find(const cv::Mat& image)
	{
		//这里调用另一个重载的find()函数，输入的参数较多
		return find(image, hranges[0], hranges[1], channels);
	
	}

	//查找属于直方图的像素
	cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int* channels)
	{
		cv::Mat result;

		///<如果从外部直接调用本函数，需要初始化一些成员变量>

		//直方图每个维度上像素的值范围
		hranges[0] = minValue;
		hranges[1] = maxValue;

		//设置三个通道
		for (int i = 0; i < histogram.dims; i++)
			this->channels[i] = channels[i];

		//反向投影直方图
		cv::calcBackProject(
			&image,		//输入图像的地址
			1,			//只使用1幅图像
			channels,	//用到的通道，取决于直方图的维度
			histogram,	//需要反向投影的直方图
			result,		//反向投影得到的结果（概率分布图）
			ranges,		//直方图每个维度上像素值的范围
			255.0	//选用的换算系数，把概率值从[0,1]映射到[0,255]
		);
		//这里的result是概率分布图，这里的概率是指整幅图像中每个像素属于我们定义的ROI区域内物体的概率

		//对反向投影结果做阈值化，得到二值图像
		if (threshold > 0.0)//阈值需要大于0，否则直接返回原始的概率分布图
			cv::threshold(result, result, 255.0 * threshold, 255.0, cv::THRESH_BINARY);

		return result;
	}



};

