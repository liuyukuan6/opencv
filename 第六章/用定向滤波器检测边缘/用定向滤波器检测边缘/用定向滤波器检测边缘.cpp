// 用定向滤波器检测边缘.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


int main()
{
	cv::Mat boldt = cv::imread("boldt.jpg", cv::IMREAD_GRAYSCALE);

	cv::imshow("boldt", boldt);

#pragma region 
	//水平方向滤波器的调用
	/*cv::Sobel(image, // 输入
		sobel, // 输出
		image_depth, // 图像类型
		xorder, yorder, // 内核规格
		kernel_size, // 正方形内核的尺寸
		alpha, beta); // 比例和偏移量*/

	cv::Mat sobelX;

	cv::Sobel(boldt, // 输入
		sobelX, // 输出
		CV_8U, // 图像类型
		1, 0, // 内核规格
		3, // 正方形内核的尺寸
		0.4, 128); // 比例和偏移量

	cv::imshow("sobelX", sobelX);

	//(7x7)
	cv::Sobel(boldt, sobelX, CV_8U, 1, 0, 7, 0.001, 128);

	cv::namedWindow("Sobel X Image (7x7)");
	cv::imshow("Sobel X Image (7x7)", sobelX);

	cv::waitKey();

	//垂直方向滤波的调用
	cv::Mat sobelY;
	cv::Sobel(boldt, // 输入
		sobelY, // 输出
		CV_8U, // 图像类型
		0, 1, // 内核规格
		3, // 正方形内核的尺寸
		0.4, 128); // 比例和偏移量

	cv::imshow("sobelY", sobelY);

	cv::waitKey();

#pragma endregion

#pragma 
	/*Sobel 算子的内核中既有正数又有负数，因此 Sobel 滤波器的计算结果通常
是 16 位的有符号整数图像（CV_16S）*/

	// 计算 Sobel 滤波器的范数
	cv::Sobel(boldt, sobelX, CV_16S, 1, 0);
	cv::Sobel(boldt, sobelY, CV_16S, 0, 1);
	cv::Mat sobel;
	// 计算 L1 范数
	sobel = abs(sobelX) + abs(sobelY);

	double sobmin, sobmax;
	cv::minMaxLoc(sobel, &sobmin, &sobmax);
	std::cout << "sobel value range: " << sobmin << "  " << sobmax << std::endl;

	// 转换成 8 位图像
// sobelImage = -alpha*sobel + 255 
	cv::Mat sobelImage;
	sobel.convertTo(sobelImage, CV_8U, -255. / sobmax, 255);
	//在 convertTo 方法中使用可选的缩放参数可得到一幅图像，图像中的白色用 0 表示，更黑的灰色阴影用大于 0 的值表示。
	cv::imshow("Sobel Image", sobelImage);

	// 加阈值(高)，得到图像轮廓的二值分布图
	cv::Mat sobelThresholded;
	cv::threshold(sobelImage, sobelThresholded, 225, 255, cv::THRESH_BINARY);

	cv::namedWindow("Binary Sobel Image (low)");
	cv::imshow("Binary Sobel Image (low)", sobelThresholded);

	//  加阈值（低）
	cv::threshold(sobelImage, sobelThresholded, 190, 255, cv::THRESH_BINARY);

	cv::namedWindow("Binary Sobel Image (high)");
	cv::imshow("Binary Sobel Image (high)", sobelThresholded);


	// Print window pixel values
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++)
			std::cout << std::setw(5) << static_cast<int>(sobel.at<short>(i + 79, j + 215)) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	cv::waitKey();


	// 计算 Sobel 算子，必须用浮点数类型 
	cv::Sobel(boldt, sobelX, CV_32F, 1, 0);
	cv::Sobel(boldt, sobelY, CV_32F, 0, 1);
	// 计算梯度的 L2 范数和方向
	cv::Mat norm, dir;
	// 将笛卡儿坐标换算成极坐标，得到幅值和角度
	cv::cartToPolar(sobelX, sobelY, norm, dir);

	cv::imshow("norm", norm);
	cv::imshow("dir", dir);
	cv::waitKey();

}