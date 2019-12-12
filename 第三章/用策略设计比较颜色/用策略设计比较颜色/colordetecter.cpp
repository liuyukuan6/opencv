#include "colordetecter.h"
#include <vector>

cv::Mat ColorDetector::process(const cv::Mat& image)
{
	//创建输出图像，与输入图像的尺寸相同，不过是单通道灰度图像（二值图像）
	result.create(image.size(), CV_8U);

	//创建遍历输入图像的迭代器（没有初始化，因为后面还得判断用哪个色彩空间）
	cv::Mat_<cv::Vec3b>::const_iterator it;
	cv::Mat_<cv::Vec3b>::const_iterator itend;

	//输出图像的迭代器
	cv::Mat_<uchar>::iterator itout=result.begin<uchar>();

#pragma region 转换色彩空间
	//判断是否需要使用CIE L*a*b*色彩空间
	if (useLab)
		//将输入图像（默认为RGB色彩空间）转换为CIE L*a*b*色彩空间（简称为Lab色彩空间）
		cv::cvtColor(image, converted, CV_BGR2Lab);

#pragma endregion
	//初始化输入图像迭代器
	if (useLab)//如果使用Lab色彩空间
	{
		it = converted.begin<cv::Vec3b>();
		itend = converted.end<cv::Vec3b>();

	}
	else
	{
		it = image.begin<cv::Vec3b>();
		itend = image.end<cv::Vec3b>();
	}

	for (; it != itend; ++it, ++itout)
	{
		///<比较输入图像中的每个像素的颜色与目标颜色之间的差距>

		//如果输入图像中当前像素的颜色与目标颜色的差距 < 可允许的最大差距
		if (getDistanceToTargetColor(*it) < maxDist)
			//输出图像中当前像素的颜色赋值为255（白色）
			*itout = 255;//表示该像素的颜色与目标颜色接近
		else
			//输出图像中当前像素的颜色赋值为0(黑色)
			*itout = 0;//表示该像素的颜色与目标颜色差距较大

	}
	//返回输出图像（二值图像）
	return result;

}