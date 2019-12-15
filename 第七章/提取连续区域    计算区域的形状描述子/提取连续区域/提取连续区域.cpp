// 提取连续区域.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	cv::Mat image = cv::imread("binaryGroup.bmp", 0);
	if (!image.data)
		return 0;

	cv::namedWindow("Binary Image");
	cv::imshow("Binary Image", image);


	// 用于存储轮廓的向量
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image,
		contours, // 存储轮廓的向量
		cv::RETR_EXTERNAL, // 检索外部轮廓
		cv::CHAIN_APPROX_NONE); // 每个轮廓的全部像素

	// 输出轮廓
	std::cout << "Contours: " << contours.size() << std::endl;
	std::vector<std::vector<cv::Point> >::const_iterator itContours = contours.begin();
	for (; itContours != contours.end(); ++itContours) {

		std::cout << "Size: " << itContours->size() << std::endl;
	}

	// 在白色图像上画黑色轮廓
	cv::Mat result(image.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(result, contours,
		-1, // 画全部轮廓
		cv::Scalar(0), // 用黑色画
		2); // 宽度为 2


	cv::imshow("Contours", result);

	// 删除太短或太长的轮廓
	int cmin = 50; // 最小轮廓长度
	int cmax = 1000; // 最大轮廓长度
	std::vector<std::vector<cv::Point>>::iterator itc = contours.begin();
	// 针对所有轮廓
	while (itc != contours.end()) {
		// 验证轮廓大小
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}

#pragma region 在原始图像上画轮廓
	cv::Mat original = cv::imread("group.jpg");

	cv::drawContours(original, contours,
		-1, // 画所有轮廓
		cv::Scalar(255, 255, 255), // 白色
		2); // 宽度2

	cv::namedWindow("Contours on Animals");
	cv::imshow("Contours on Animals", original);
	cv::waitKey();
	cv::destroyAllWindows();


#pragma endregion 



#pragma region 计算区域的形状描述子


	image = cv::imread("binaryGroup.bmp", 0);
	// 测试边界框(右下角)
	cv::Rect r0 = cv::boundingRect(contours[0]);
	// 画矩形
	cv::rectangle(result, r0, 0, 2);

	// 测试覆盖圆（右上角）
	float radius;
	cv::Point2f center;
	cv::minEnclosingCircle(contours[1], center, radius);
	// 画圆形
	cv::circle(result, center, static_cast<int>(radius),
		cv::Scalar(0), 2);

	// 测试多边形逼近(左一)
	std::vector<cv::Point> poly;
	cv::approxPolyDP(contours[2], poly, 5, true);
	// 画多边形
	cv::polylines(result, poly, true, 0, 2);
	std::cout << "Polygon size: " << poly.size() << std::endl;

	// 测试凸包（另类多边形逼近）左二
	std::vector<cv::Point> hull;
	cv::convexHull(contours[3], hull);
	// 画多边形
	cv::polylines(result, hull, true, 0, 2);

	// 测试轮廓矩
// 迭代遍历所有轮廓
	itc = contours.begin();
	while (itc != contours.end()) {
		// 计算所有轮廓矩
		cv::Moments mom = cv::moments(cv::Mat(*itc++));
		// 画重心
		cv::circle(result,
			// 将重心位置转换成整数
			cv::Point(mom.m10 / mom.m00, mom.m01 / mom.m00),
			2, cv::Scalar(0), 2); // 画黑点
	}
	cv::namedWindow("Some Shape descriptors");
	cv::imshow("Some Shape descriptors", result);

#pragma endregion

#pragma region 所有闭合轮廓
	cv::findContours(image,
		contours, // 存放轮廓的向量
		cv::RETR_LIST, // 检索全部轮廓
		cv::CHAIN_APPROX_NONE); // 全部像素
	cv::Mat result1(image.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(result1, contours,
		-1, // 画全部轮廓
		cv::Scalar(0), // 用黑色画
		2); // 宽度为 2
	cv::imshow("result1", result1);

#pragma endregion
	cv::waitKey();

#pragma region
	image = cv::imread("binaryGroup.bmp", 0);

	// Get the contours of the connected components
	cv::findContours(image,
		contours, //存放轮廓的向量
		cv::RETR_LIST, // 检索全部轮廓
		cv::CHAIN_APPROX_NONE); // 全部像素

	//画黑色轮廓
	result.setTo(255);
	cv::drawContours(result, contours,
		-1,  // 画所有轮廓
		0,  // 黑色
		2); // 宽度
	cv::namedWindow("All Contours");
	cv::imshow("All Contours", result);

	// mser后的图像
	cv::Mat components;
	components = cv::imread("mser.bmp", 0);

	// 创建二值图像
	components = components == 255;
	// 打开图像（包含白背景）
	cv::morphologyEx(components, components, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 3);

	cv::namedWindow("MSER image");
	cv::imshow("MSER image", components);

	contours.clear();
	//翻转图像（背景必须是黑色的）
	cv::Mat componentsInv = 255 - components;
	// 得到连续区域的轮廓
	cv::findContours(componentsInv,
		contours, // 轮廓的向量
		cv::RETR_EXTERNAL, // 检索外部轮廓
		cv::CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	// 白色图像
	cv::Mat quadri(components.size(), CV_8U, 255);

	// 针对全部轮廓
	std::vector<std::vector<cv::Point> >::iterator it = contours.begin();
	while (it != contours.end()) {
		poly.clear();
		//  用多边形逼近轮廓
		cv::approxPolyDP(*it, poly, 5, true);

		//  是否为四边形？
		if (poly.size() == 4) {
			// 画出来
			cv::polylines(quadri, poly, true, 0, 2);
		}

		++it;
	}

	cv::namedWindow("MSER quadrilateral");
	cv::imshow("MSER quadrilateral", quadri);

#pragma endregion

	cv::waitKey();
	return 0;


}
