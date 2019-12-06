#define ICOMPARATOR

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "colorhistogram.h"

class ImageComparator
{
private:
	cv::Mat refH;//参考图像的直方图
	cv::Mat inputH;//输入图像的直方图

	ColorHistogram hist;//计算彩色直方图类的对象
	int nBins;//每个通道使用的直方图箱子数量

public:

	//构造函数，比较直方图时每个颜色通道用的箱子数都默认为8
	ImageComparator() :nBins(8) {}

	//设置比较直方图时使用的箱子数量
	void getNumberOfBins(int bins) { nBins=bins; }

	//获取比较直方图时使用的箱子数
	int getNumberOfBins() { return nBins; }

	//设置并计算参考图像的直方图
	void setReferenceImage(const cv::Mat& image)
	{
		//设置每个通道的箱子数量
		hist.setSize(nBins);
		//计算参考图像的直方图
		refH = hist.getHistogram(image);
	
	}
	double compare(const cv::Mat& image)
	{
		//计算输入图像的直方图
		inputH = hist.getHistogram(image);

		//用交叉法比较直方图，返回相似度
		return cv::compareHist(
			refH,//参考图像的直方图
			inputH,//输入图像的直方图
			cv::HISTCMP_INTERSECT);
		//使用交叉点方法：逐个箱子地比较每个直方图中的数值，并保存最小的值，然后把这些最小值累加，作为相似度测量值


	
	
	}



};
