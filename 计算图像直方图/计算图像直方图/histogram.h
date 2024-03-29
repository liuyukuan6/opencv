#pragma once


#define HISTOGRAM

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>




//创建灰度图像的直方图
class Histogram1D
{
private:

	int histSize[1];		//直方图中箱子的数量
	float hranges[2];		//直方图像素的值范围
	const float* ranges[1];	//指针数组，存储一个指向直方图像素的值范围的指针
	int channels[1];		//要处理的通道数量



public:

	Histogram1D()			//构造函数
	{
		/// <准备一维直方图的默认参数>
		/// <这里之所以这么设置，是为了和后面的calHist()函数的输入参数对应起来>

		//直方图箱子个数
		histSize[0] = 256;

		//直方图像素值的范围
		hranges[0] = 0.0;		//从0开始
		hranges[1] = 256.0;
		ranges[0] = hranges;	//用指针数组来存储直方图像素值的范围

		//灰度图像只有一个通道
		channels[0] = 0;		//只关注通道0
	
	}

#pragma  region  Setters and Getters

	//设置要计算直方图的通道，默认情况下是通道0
	void setChannel(int c) { channels[0] = c; };

	//获取要计算直方图的通道
	int getChannel() { return channels[0]; };

	//设置像素值的范围，默认值是(0,256]
	void setRange(float minValue, float maxValue)
	{
		hranges[0] = minValue;
		hranges[1] = maxValue;
	}

	//获取最小像素值
	float getMinValue() { return hranges[0]; }

	//获取最大像素值
	float getMaxValue() { return hranges[1]; }

	//在直方图中设置箱子的数量，默认值是256
	void setNBins(int nbins) { histSize[0] = nbins; }

	//获取直方图中的桶数
	int getNBins() { return histSize[0]; }

#pragma endregion

	//计算一维直方图
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
			1,				//直方图的维数（通道数），这是一维的直方图（与灰度图像对应）
			histSize,		//直方图每个维度（通道）上的箱子数量（一维，256个箱子）
			ranges			//直方图像素值的范围（计算值在那些范围内的像素，这里是(0,256)）
			);
		return hist;
	}

	//计算一维直方图，并返回图像
	//zoom是缩放率
	cv::Mat getHistogramImage(const cv::Mat& image, int zoom = 2)
	{
		//计算直方图
		cv::Mat hist=getHistogram(image);


		//创建图像
		return Histogram1D::getImageOfHistogram(hist, zoom);
	
	}

#pragma region 静态方法（类方法）

	//创建一个表示直方图的图像（静态方法）
	static cv::Mat getImageOfHistogram(const cv::Mat& hist, int zoom)
	{
		//取得箱子值的最大值和最小值
		double maxVal = 0;//箱子中存放的最大值
		double minVal = 0;//箱子中存放的最小值
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		//取得直方图的大小（hist是列向量）
		int histSize = hist.rows;//256

		//用于显示直方图的正方型图像（白色）
		cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));//图像的高度等于宽度

		//设置最高点为90%的箱子个数（及图像高度的90%）
		int hpt = static_cast<int>(0.9 * histSize);

		///<为每个箱子画垂直线>

		//遍历所有箱子
		for (int h = 0; h < histSize; h++)
		{
			//取出第h个箱子里面的值
			float binVal = hist.at<float>(h);//注意，直方图中的值存储为float值

			if (binVal > 0)
			{
				//换算出当前箱子中值对应的高度
				/*
						线			值			线			值
					intensity	  binVal	 当前高度	  当前值
					————— = ————	————— = ————
						hpt		  maxVal	 最大高度	  最大值

						=>当前高度：intensity=binVal*hpt/maxVal
				*/
				int intensity = static_cast<int>(binVal * hpt / maxVal);

				//画出当前箱子的值对应的线
				cv::line(
					histImg,//输出图像
					cv::Point(h * zoom, histSize * zoom),//起始坐标
					cv::Point(h * zoom, (histSize - intensity) * zoom),//结束坐标
					cv::Scalar(0), //颜色:0
					zoom);//粗细:缩放率
				
				
			}
		}
	
	//返回直方图的图像
		return histImg;
	}


#pragma endregion

#pragma region

	//应用查找表将一个输入图像转化为一幅单通道图像
	static cv::Mat applyLookUp(const cv::Mat& image,
		const cv::Mat& lookup)//查找表，uchar类型的1*256数组
	{
		cv::Mat result;

		cv::LUT(
			image,
			lookup,//查找表（一维行数组）
			result);
	
		return result;
	}

#pragma endregion

#pragma region  伸展源图像（使用百分比）

	//percentile:要使黑色（值为0）的像素与白色（值为255）的像素个数占总像素的percentile%
	cv::Mat stretch(const cv::Mat& image, float percentile)
	{
		//计算占总像素percentile%的黑色与白色的个数
		float number = image.total() * percentile;

		//首先源图像的计算直方图
		cv::Mat hist = getHistogram(image);//256*1的列向量

		///<找到直方图的左极限>

		int imin = 0;//初始化左极限
		//从左到右开始遍历
		for (float count = 0; imin < 256; imin++)
		{
			//累加箱子里的像素数，当累加的像素数量>=number时，停止遍历
			if ((count += hist.at<float>(imin)) >= number)
				break;
		}
		int imax = 255;//右极限

		for (float count = 0; imax >= 0; imax--)
		{
			if ((count += hist.at<float>(imax)) >= number)
				break;
		
		}
	
		std::cout << "imin = " << imin << "imax =" << imax << std::endl;

		//创建查找表
		cv::Mat lookup(1, 256, CV_8U);
	
		//遍历整个直方图
		for (int i = 0; i < 256; i++)
		{
			//设置查找表的规则，伸展直方图
			if (i < imin)		//小于左极限imin的映射为0
				lookup.at<uchar>(i) = 0;
			else if (i > imax)
				lookup.at<uchar>(i) = 255;
			else
				lookup.at<uchar>(i) = cvRound(255.0 * (i - imin) / (imax - imin));//255*归一化

		}

		//应用查找表
		cv::Mat result;
		result = applyLookUp(image, lookup);

		return result;
	
	}



#pragma endregion

#pragma region 均衡化源图像
	static cv::Mat equalize(const cv::Mat& image)
	{
		cv::Mat result;

		//调用Opencv的直方图均衡图像
		cv::equalizeHist(image, result);

		return result;
	
	}


#pragma endregion

};

