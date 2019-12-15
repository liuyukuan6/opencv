// 拉普拉斯算子.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "laplacianZC.h"

int main()
{

	cv::Mat image = cv::imread("boldt.jpg", 0);
	if (!image.data)
		return 0;

	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);

	cv::Mat laplace;
	cv::Laplacian(image, laplace, CV_8U, 1, 1, 128);

	cv::namedWindow("Laplacian Image");
	cv::imshow("Laplacian Image", laplace);

	// 用 LaplacianZC 类计算拉普拉斯算子
	LaplacianZC laplacian;
	laplacian.setAperture(7); // 7×7 的拉普拉斯算子
	cv::Mat flap = laplacian.computeLaplacian(image);

	// display min max values of the lapalcian
	double lapmin, lapmax;
	cv::minMaxLoc(flap, &lapmin, &lapmax);

	laplace = laplacian.getLaplacianImage();
	cv::namedWindow("Laplacian Image (7x7)");
	cv::imshow("Laplacian Image (7x7)", laplace);
	cv::waitKey();

#pragma region 
	int cx(238), cy(90);
	int dx(12), dy(12);

	// 抓取了image上的一块小区域
	cv::Mat window(image, cv::Rect(cx, cy, dx, dy));
	cv::namedWindow("Image window");
	cv::imshow("Image window", window);
	cv::imwrite("window.bmp", window);

	// 输出图像像素
	std::cout << std::endl;
	std::cout << "Image values:\n\n";
	for (int i = 0; i < dx; i++) {
		for (int j = 0; j < dy; j++)
			std::cout << std::setw(5) << static_cast<int>(image.at<uchar>(i + cy, j + cx)) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	// 输出拉普拉斯值
	std::cout << "Laplacian value range=[" << lapmin << "," << lapmax << "]\n";
	std::cout << std::endl;
	for (int i = 0; i < dx; i++) {
		for (int j = 0; j < dy; j++)
			std::cout << std::setw(5) << static_cast<int>(flap.at<float>(i + cy, j + cx) / 100) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

#pragma endregion

#pragma region

	// 计算和输出过零点图
	cv::Mat zeros;
	zeros = laplacian.getZeroCrossings(flap);
	cv::namedWindow("Zero-crossings");
	cv::imshow("Zero-crossings", 255 - zeros);
	cv::waitKey();

	// 输出图像的像素值
	std::cout << "Zero values:\n\n";
	for (int i = 0; i < dx; i++) {
		for (int j = 0; j < dy; j++)
			std::cout << std::setw(2) << static_cast<int>(zeros.at<uchar>(i + cy, j + cx)) / 255 << " ";
		std::cout << std::endl;
	}

#pragma endregion

	// down-sample and up-sample the image
	cv::Mat reduced, rescaled;
	cv::pyrDown(image, reduced);
	cv::pyrUp(reduced, rescaled);

	// Display the rescaled image
	cv::namedWindow("Rescaled Image");
	cv::imshow("Rescaled Image", rescaled);

	cv::waitKey();
	cv::destroyAllWindows();

#pragma region 高斯差分
	
	/*现在用两个不同带宽的高斯滤波器对一幅图像做滤波，然后将这两个结果相减，就能得到由较高的频率构成的图像。这些频率被一个滤波器保留，
被另一个滤波器丢弃。*/

	// 计算高斯差分
	cv::Mat dog;
	cv::subtract(rescaled, image, dog, cv::Mat(), CV_16S);
	cv::Mat dogImage;
	dog.convertTo(dogImage, CV_8U, 1.0, 128);

	cv::namedWindow("DoG Image (from pyrdown/pyrup)");
	cv::imshow("DoG Image (from pyrdown/pyrup)", dogImage);

	// 使用两个高斯滤波
	cv::Mat gauss05;
	cv::Mat gauss15;
	cv::GaussianBlur(image, gauss05, cv::Size(), 0.5);
	cv::GaussianBlur(image, gauss15, cv::Size(), 1.5);

	// 计算高斯差分 
	cv::subtract(gauss15, gauss05, dog, cv::Mat(), CV_16S);
	dog.convertTo(dogImage, CV_8U, 2.0, 128);

	
	cv::namedWindow("DoG Image");
	cv::imshow("DoG Image", dogImage);

	//使用两次高斯滤波
	cv::Mat gauss20;
	cv::Mat gauss22;
	cv::GaussianBlur(image, gauss20, cv::Size(), 2.0);
	cv::GaussianBlur(image, gauss22, cv::Size(), 2.2);

	//高斯差分
	cv::subtract(gauss22, gauss20, dog, cv::Mat(), CV_32F);
	dog.convertTo(dogImage, CV_8U, 10.0, 128);


	cv::namedWindow("DoG Image (2)");
	cv::imshow("DoG Image (2)", dogImage);

	// DoG的过零点图 
	zeros = laplacian.getZeroCrossings(dog);
	cv::namedWindow("Zero-crossings of DoG");
	cv::imshow("Zero-crossings of DoG", 255 - zeros);

	// Display the image with window
	cv::rectangle(image, cv::Rect(cx, cy, dx, dy), cv::Scalar(255, 255, 255));
	cv::namedWindow("Original Image with window");
	cv::imshow("Original Image with window", image);

	cv::waitKey();
#pragma endregion
	return 0;
}

