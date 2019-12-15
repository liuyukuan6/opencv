// 用滤波器进行缩减像素采样.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
#pragma region 错误的示范，直接缩小图像
	// 的图像缩小到 1/4 后得到的，方法是只保留每四行（列）像素中的一行（列）
	cv::Mat image = cv::imread("boldt.jpg", 0);

	cv::imshow("Original Image", image);

	cv::Mat reduced(image.rows / 4, image.cols / 4, CV_8U);

	for (int i = 0; i < reduced.rows; i++)
		for (int j = 0; j < reduced.cols; j++)
			reduced.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);

	// 展示刚缩小的图像
	cv::namedWindow("Badly reduced Image");
	cv::imshow("Badly reduced Image", reduced);

	//图像放大四倍(行、列放大四倍)
	cv::resize(reduced, reduced, 
		cv::Size(),
		4, //行放大的倍数
		4, //列放大的倍数
		cv::INTER_NEAREST);//插值方式：此处插值方式，单纯的让行列乘以4

	// 展示放大后的图像
	cv::namedWindow("Badly reduced");
	cv::imshow("Badly reduced", reduced);
	cv::waitKey();
	cv::destroyAllWindows();
#pragma endregion

#pragma region 先在原始图像上应用低通滤波器(低通滤波的作用是消除在缩减后的图像中无法表示的高频部分)

	// 首先去除高频成分
	cv::GaussianBlur(image, image, cv::Size(11, 11), 2.0);
	cv::Mat reduced2(image.rows / 4, image.cols / 4, CV_8U);
	for (int i = 0; i < reduced2.rows; i++)
		for (int j = 0; j < reduced2.cols; j++)
			reduced2.at<uchar>(i, j) = image.at<uchar>(i * 4, j * 4);

	// 展示刚缩小的图像
	cv::namedWindow("Reduced Image");
	cv::imshow("Reduced Image", reduced2);

	//图像放大四倍
	cv::resize(reduced2, reduced2,
		cv::Size(),
		3, 
		4, 
		cv::INTER_NEAREST);

	// 展示放大后的图像
	cv::namedWindow("Reduced");
	cv::imshow("Reduced", reduced2);

	cv::Mat newImage;
	// resizing with bilinear
	cv::resize(reduced2, newImage, 
		cv::Size(),
		2, 
		2, 
		cv::INTER_LINEAR);//线性插值
	
	// Display the (resized) reduced image
	cv::namedWindow("Bilinear resizing");
	cv::imshow("Bilinear resizing", newImage);
	
	cv::waitKey();
	cv::destroyAllWindows();

#pragma endregion

#pragma region Nyquist-Shannon 定理，它表明如果把图像缩小一半，那么其可见的频率带宽也将减少一半
	cv::Mat reducedImage; // 用于存储缩小后的图像
	cv::pyrDown(image, reducedImage);// 图像尺寸缩小一半(即行列缩小一半)
	

	cv::imshow("REUCEDIMAGE", reducedImage);
	cv::waitKey();
	cv::destroyAllWindows();

#pragma endregion

#pragma region 

	// Creating an image pyramid
	cv::Mat pyramid(image.rows, 
		image.cols + image.cols / 2 + image.cols / 4 + image.cols / 8,
		CV_8U,
		cv::Scalar(255));
	image.copyTo(pyramid(cv::Rect(0, 0, image.cols, image.rows)));

	cv::pyrDown(image, reduced); //图像尺寸缩小一半
	reduced.copyTo(pyramid(cv::Rect(image.cols, image.rows / 2, image.cols / 2, image.rows / 2)));

	cv::pyrDown(reduced, reduced2); //图像尺寸缩小一半,存到reduced2
	reduced2.copyTo(pyramid(cv::Rect(image.cols + image.cols / 2, image.rows - image.rows / 4, image.cols / 4, image.rows / 4)));

	cv::pyrDown(reduced2, reduced); // 图像尺寸缩小一半
	reduced.copyTo(pyramid(cv::Rect(image.cols + image.cols / 2 + image.cols / 4, image.rows - image.rows / 8, image.cols / 8, image.rows / 8)));

	// 展示图像
	cv::namedWindow("Pyramid of images");
	cv::imshow("Pyramid of images", pyramid);

#pragma endregion

	cv::waitKey();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
