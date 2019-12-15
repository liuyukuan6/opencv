#pragma once

#if !defined WATERSHS
#define WATERSHS
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


class WatershedSegmenter
{
private:
	cv::Mat markers;

public:

	//markerImage:输入的标记图像（灰度）
	void setMarkers(const cv::Mat& markerImage)
	{
		//转换为整数型图像，32位有符号整数
		markerImage.convertTo(markers, CV_32S);
	
	}
	void process(const cv::Mat& image)
	{
		//应用分水岭，标记图像中每个非零像素代表一个标签
		//白色255  => 前景（野牛），标签
		//灰色128  => 背景（草地），标签
		//黑色0	   => 未知区域，非标签，也是分水岭算法要识别分割的区域，就是这部分区域最后
		//要么划分为前景，要么划分为背景
		cv::watershed(
			image,		//输入图像
			markers);	//标记图像，也是输出图像，32位有符号整数
		//分水岭对应的像素设为-1
	}

	//以图像的形式返回分割结果
	cv::Mat getSegmentation()
	{
		cv::Mat tmp;
		//结果图像转化为灰度图像
		markers.convertTo(tmp, CV_8U);//所有标签值大于255的区段都赋值为255（因为图像被转化为了CV_8U，单通道值）

		return tmp;
	}

	//以图像形式返回分水岭图像
	cv::Mat getWatersheds()
	{
		cv::Mat tmp;
		//转化为灰度图像，把每个像素p的值Ip转换为：255*Ip+255
		markers.convertTo(tmp, CV_8U, 255, 255);
		/*除了分水岭所在的像素255*(-1)+255=0以外，
		其他的像素都转化为255（大于255的区段都赋值为255，前景255*255+255->255，背景128*255+255->255）,
		所以图像结果是：分水岭为黑色，其他区域（前景和背景）都为白色*/
		return tmp;
	}

};







#endif