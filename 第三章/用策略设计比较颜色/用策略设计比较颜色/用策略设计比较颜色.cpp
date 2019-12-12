// 用策略设计比较颜色.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include  "colordetecter.h"
#include <opencv2/core.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>

#include<opencv2/imgproc/types_c.h>


int main()
{
#pragma region RBG
	// 1.创建图像处理器对象
	ColorDetector cdetect;
	// 2.读取输入的图像
	cv::Mat image = cv::imread("C:/Users/liuyukuan/Desktop/cat/6.jpg");
	if (image.empty()) return 0;
	// 3.设置输入参数
	cdetect.setTargetColor(230, 190, 130); // 这里表示蓝天
	// 4.处理图像并显示结果
	cv::namedWindow("result");

	cv::Mat result = cdetect.process(image);

	cv::imshow("result", result);
	cv::waitKey();
#pragma endregion

#pragma region 用CIE L*a*b色彩空间
	//这里的颜色距离计算使用CIE L*a*b色彩空间
	ColorDetector colordetector
	(
		200, 100, 100,	//要探测的目标颜色
		45,				//允许的最大差距
		true);
	
	//处理图像并显示二值结果
	result = colordetector(image);//调用仿函数

	cv::namedWindow("result(functor)");
	cv::imshow("result(functor)", result);
	cv::waitKey();


#pragma endregion floodFill函数
	/*从起始点/种子点开始填充一个连接域。连通性由像素值的接近程度来衡量
	*/
	
	//测试floodFill函数
	cv::floodFill(
		image,
		cv::Point(100, 50),
		cv::Scalar(255, 255, 255),
		(cv::Rect*)0,
		cv::Scalar(35, 35, 35),
		cv::Scalar(35, 35, 35),
		cv::FLOODFILL_FIXED_RANGE
	);

	cv::namedWindow("floodFill");
	cv::imshow("floodFill", image);
	//这里是把天空画成白色。即使其他地方有颜色接近的像素（例如水面），除非他们与天空相连，否则也不会识别出来
	cv::waitKey();
	
	

#pragma endregion
	return 0;
}

