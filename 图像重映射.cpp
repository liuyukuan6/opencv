// 图像重映射.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp> 
#include <opencv2/imgproc.hpp> 
#include <opencv2/highgui.hpp>
#include <math.h>


// 重映射图像，创建波浪形效果
void wave(const cv::Mat& image, cv::Mat& result) {
	// 映射参数
	cv::Mat srcX(image.rows, image.cols, CV_32F);//描述x(列)方向的变化
	cv::Mat srcY(image.rows, image.cols, CV_32F);//描述y(行)方向的变化
	// 创建映射参数
	for (int i = 0; i < image.rows; i++) {//行循环
		for (int j = 0; j < image.cols; j++) {//列循环
			// (i,j)像素的新位置
			srcX.at<float>(i, j) = j; // 保持在同一列
			// 原来在第 i 行的像素，现在根据一个正弦曲线移动
			srcY.at<float>(i, j) = i + 5 * sin(j / 10.0);
		}
	}
	// 应用映射参数
	cv::remap(image, // 源图像
		result, // 目标图像
		srcX, // x 映射
		srcY, // y 映射
		cv::INTER_LINEAR); // 填补方法
}

int main()
{
	cv::Mat joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");
	cv::Mat result;

	wave(joker, result);

	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::waitKey();



}
