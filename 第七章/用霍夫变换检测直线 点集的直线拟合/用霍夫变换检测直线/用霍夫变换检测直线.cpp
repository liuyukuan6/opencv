// 用霍夫变换检测直线.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "linefinder.h"

#define PI 3.1415926

int main()
{
#pragma region 画直线

	// Create a test image
	cv::Mat test(200, 200, CV_8U, cv::Scalar(0));
	cv::line(test, cv::Point(100, 0), cv::Point(200, 200), cv::Scalar(255));
	cv::line(test, cv::Point(0, 50), cv::Point(200, 200), cv::Scalar(255));
	cv::line(test, cv::Point(0, 200), cv::Point(200, 0), cv::Scalar(255));
	cv::line(test, cv::Point(200, 0), cv::Point(0, 200), cv::Scalar(255));
	cv::line(test, cv::Point(100, 0), cv::Point(100, 200), cv::Scalar(255));
	cv::line(test, cv::Point(0, 100), cv::Point(200, 100), cv::Scalar(255));

	// Display the test image
	cv::namedWindow("Test Image");
	cv::imshow("Test Image", test);
	cv::imwrite("test.bmp", test);

	cv::waitKey();
#pragma endregion
#pragma region 测试霍夫变换检测直线(结果不是太好)
	cv::Mat image = cv::imread("road.jpg", cv::IMREAD_GRAYSCALE);


	cv::imshow("Original Image", image);
	// 应用 Canny 算法
	cv::Mat contours;
	cv::Canny(image, contours, 125, 350);
	// 用霍夫变换检测直线
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(test, lines, 
		1, //半径步长为 1，表示函数将搜索所有可能的半径
		PI / 180, // 角度步长为 π / 180，表示函数将搜索所有可能的角度
		60); // 最小投票数

	
	cv::Mat result(contours.rows, contours.cols, CV_8U, cv::Scalar(255));
	image.copyTo(result);

	std::cout << "Lines detected: " << lines.size() << std::endl;
	
	std::vector<cv::Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0]; // 第一个元素是距离 rho 
		float theta = (*it)[1]; // 第二个元素是角度 theta 
		if (theta < PI / 4. || theta > 3. * PI / 4.) { // 垂直线（大致）
		// 直线与第一行的交叉点
			cv::Point pt1(rho / cos(theta), 0);
			// 直线与最后一行的交叉点
			cv::Point pt2((rho - result.rows * sin(theta)) /cos(theta), result.rows);
			// 画白色的线
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);
		}
		else { // 水平线（大致）
				// 直线与第一列的交叉点
			cv::Point pt1(0, rho / sin(theta));
			// 直线与最后一列的交叉点
			cv::Point pt2(result.cols,(rho - result.cols * cos(theta)) / sin(theta));
			// 画白色的线
			cv::line(result, pt1, pt2, cv::Scalar(255), 1);
		}
		std::cout << "line: (" << rho << "," << theta << ")\n";
		++it;
	}
	// Display the detected line image
	cv::namedWindow("Lines with Hough");
	cv::imshow("Lines with Hough", result);

	cv::waitKey();
	cv::destroyAllWindows();
#pragma endregion


#pragma region 概率霍夫检测
	//输出的是线段，不是直线,所以会先确定两个点
	


	//创建 LineFinder 类的实例
	LineFinder ld;

	// 设置概率霍夫变换的参数
	ld.setLineLengthAndGap(100, 20);
	ld.setMinVote(60);

	// 检测直线并画线
	std::vector<cv::Vec4i> li = ld.findLines(contours);
	std::cout << li[0] << li[1] << li[2] << li[3] << ")\n";//输出两个点的坐标
	ld.drawDetectedLines(image);
	cv::namedWindow("Lines with HoughP");
	cv::imshow("Lines with HoughP", image);
	cv::waitKey();
	cv::destroyAllWindows();

	std::vector<cv::Vec4i>::const_iterator it2 = li.begin();//输出两个点的坐标
	while (it2 != li.end()) {

		std::cout << "(" << (*it2)[0] << "," << (*it2)[1] << ")-("
			<< (*it2)[2] << "," << (*it2)[3] << ")" << std::endl;

		++it2;
	}
#pragma endregion

#pragma region 提取直线,点集的直线拟合
	// 一条线（5条线循环）
	image = cv::imread("road.jpg", 0);
	int n = 0;
	for (int n=0; n < 5; n++)
	{
		cv::line(image, cv::Point(li[n][0], li[n][1]), cv::Point(li[n][2], li[n][3]), cv::Scalar(255), 5);

		cv::imshow("One line of the Image", image);

		// Extract the contour pixels of the first detected line
		cv::Mat oneline(image.size(), CV_8U, cv::Scalar(0));
		cv::line(oneline, cv::Point(li[n][0], li[n][1]), cv::Point(li[n][2], li[n][3]), cv::Scalar(255), 5);
		cv::bitwise_and(contours, oneline, oneline);//// 轮廓与白色直线进行“与”运算
		cv::imshow("One line", 255 - oneline);
		cv::waitKey();

		std::vector<cv::Point> points;

		//  迭代遍历像素，得到所有点的位置
		for (int y = 0; y < oneline.rows; y++) {

			uchar* rowPtr = oneline.ptr<uchar>(y);

			for (int x = 0; x < oneline.cols; x++) {

				// 如果在轮廓上
				if (rowPtr[x]) {

					points.push_back(cv::Point(x, y));//在末尾添加数据
				}
			}
		}
		//找到最好的拟合直线
		cv::Vec4f line;
		cv::fitLine(points, line,
			cv::DIST_L2, // 距离类型
			0, // L2 距离不用这个参数
			0.01, 0.01); // 精度

		//形式是一个单位方向向量（cvVec4f 的前两个数值）和直线上一个点的坐标（cvVec4f 的后两个数值）。最后两个参数是所需的直线精度。
		std::cout << "line: (" << line[0] << "," << line[1] << ")(" << line[2] << "," << line[3] << ")\n";
		int x0 = line[2]; // a point on the line
		int y0 = line[3];
		int x1 = x0 + 100 * line[0]; // add a vector of length 100
		int y1 = y0 + 100 * line[1];
	

		//image= cv::imread("road.jpg",0);
		// 画线
		cv::line(image, cv::Point(x0, y0), cv::Point(x1, y1), 0, 2);
		cv::namedWindow("Fitted line");
		cv::imshow("Fitted line", image);

		
	}
	cv::destroyAllWindows();
	



#pragma endregion

#pragma region
	/*
	// 创建霍夫累加器
// 这里的图像类型为 uchar；实际使用时应该用 int 
	cv::Mat acc(200, 180, CV_8U, cv::Scalar(0));
	// 选取一个像素点
	int x = 50, y = 30;
	// 循环遍历所有角度
	for (int i = 0; i < 180; i++) {
		double theta = i * PI / 180.;
		// 找到对应的 rho 值
		double rho = x * std::cos(theta) + y * std::sin(theta);
		// j 对应-100~100 的 rho 
		int j = static_cast<int>(rho + 100.5);
		std::cout << i << "," << j << std::endl;
		// 增值累加器
		acc.at<uchar>(j, i)++;
	}

	// draw the axes
	cv::line(acc, cv::Point(0, 0), cv::Point(0, acc.rows - 1), 255);
	cv::line(acc, cv::Point(acc.cols - 1, acc.rows - 1), cv::Point(0, acc.rows - 1), 255);
	cv::imshow("acc", acc);

	cv::waitKey();*/
#pragma endregion

}

