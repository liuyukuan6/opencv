// 用分水岭算法实现图像分割.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "watershedSegmentation.h"


int main()
{
#pragma region Step1

	//读入原始图像
	cv::Mat image = cv::imread("group.jpg");
	if (!image.data) return 0;

	//显示原始图像(主要是为了展示一下二值图像里的前景和背景都是什么)
	cv::namedWindow("Orignal Image");
	cv::imshow("Orignal Image", image);

	//读入二值图像
	cv::Mat binary;
	binary = cv::imread("binary.bmp", cv::IMREAD_GRAYSCALE);

	//显示
	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image", binary);//前景（野牛，255），背景（草地，0）
	cv::waitKey();
#pragma endregion

#pragma region Step2

	///<提取前景物体，并准备好前景标记>

	//腐蚀二值图像4次，消除噪声和细小物体，显示前景物体
	cv::Mat fg;
	cv::erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 4);

	//显示前景图像（腐蚀后的图像）
	cv::namedWindow("Foreground Image");
	cv::imshow("Foreground Image", fg);//背景（草地，变黑）消失，前景（野牛，变白）保留

	///<提取背景物体，并做好背景标记>

	//膨胀二值图像4次，显示背景物体
	cv::Mat bg;
	cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 4);

	//图像阈值化
	cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);
	//bg=0   if(bg>1)  背景（野牛） =>变黑
	//bg=128   otherwise 背景（草地） =>变灰

	//显示背景图像
	cv::namedWindow("Background Image");
	cv::imshow("Background Image", bg);//背景（草地，变灰）保留，前景（野牛，变黑）消失
	cv::waitKey();

#pragma endregion

#pragma region Step3

	///<准备标记图像>

	//创建标记图像（灰度）
	cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));//初始值为0，大小与二值图像相等

	//将标记图像设置为：前景图像和背景图像的混合
	markers = fg + bg;

	//显示标记图像，每个非零像素代表一个标签
	cv::namedWindow("Markers");
	cv::imshow("Markers", markers);
	//白色255 => 前景（野牛），标签
	//灰色128   => 背景（草地），标签
	//黑色0  =>未知区域，非标签，也是分水岭算法要识别分割的区域，就是这部分区域最后
	//			要么分割为前景，要么划分为背景

	//创建分水岭分割类的对象
	WatershedSegmenter segmenter;

	//设置标记图像
	segmenter.setMarkers(markers);

	//执行分水岭算法，进行图像分割
	segmenter.process(image);

	//显示分割结果
	cv::namedWindow("Segmentation");
	cv::imshow("Segmentation", segmenter.getSegmentation());
	//白色255 =>前景（野牛）
	//灰色128 =>背景（草地）

	//显示分水岭（分割边缘）
	cv::namedWindow("Watersheds");
	cv::imshow("Watersheds", segmenter.getWatersheds());
	cv::waitKey();
	cv::destroyAllWindows();
#pragma endregion

#pragma region 用矩形获得标记图像

	image = cv::imread("tower.jpg");

	//创建标记图像（灰度）
	cv::Mat imageMask(image.size(), CV_8U, cv::Scalar(0));//初始值为0，大小与输入图像相等

	//在标记图像中标识背景像素（使用矩阵，颜色=255，白色框）
	cv::rectangle(imageMask,
		cv::Point(5, 5),//左上角
		cv::Point(image.cols - 5, image.rows - 5),//右下角
		cv::Scalar(255),//填充颜色，宽度
		3);

	//在标记图像中标识前景像素（使用矩阵，颜色=128，黑色框）
	cv::rectangle(imageMask,
		cv::Point(image.cols/2-10,image.rows/2-10),//左上角
		cv::Point(image.cols/2+10,image.rows/2+10),//右上角
		cv::Scalar(128),10);//填充颜色，宽度

	//显示标记图像，每个非零像素代表一个标签
	cv::namedWindow("Markers");
	cv::imshow("Markers", imageMask);
	//白色255 => 背景，标签
	//灰色128 => 前景，标签
	//黑色0   => 未知区域，非标签，也是分水岭算法要识别分割的区域，就是这部分区域最后要么划分为前景，要么为背景。

	//设置标记图像
	segmenter.setMarkers(imageMask);

	//执行分水岭算法，进行图像分割
	segmenter.process(image);

	//显示带有标记（两个矩阵）的图像
	cv::rectangle(image, cv::Point(5, 5),
		cv::Point(image.cols - 5, image.rows - 5), cv::Scalar(255, 255, 255), 3);
	cv::rectangle(image, cv::Point(image.cols / 2 - 10, image.rows / 2 - 10),
		cv::Point(image.cols / 2 + 10, image.rows / 2 + 10), cv::Scalar(1, 1, 1), 10);
	cv::namedWindow("Image with marker");
	cv::imshow("Image with marker", image);

	//显示分割结果
	cv::namedWindow("Segmentation");
	cv::imshow("Segmentation", segmenter.getSegmentation());
	//白色255 => 背景
	//灰色128 => 前景

	//显示分水岭（分割边缘）
	cv::namedWindow("Watershed");
	cv::imshow("Watershed", segmenter.getWatersheds());
	cv::waitKey();


#pragma endregion

	return 0;
}


