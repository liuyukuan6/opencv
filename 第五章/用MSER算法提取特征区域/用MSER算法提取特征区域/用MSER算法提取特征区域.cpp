// 用MSER算法提取特征区域.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>



int main()
{
	cv::Mat image = cv::imread("building.jpg", cv::IMREAD_GRAYSCALE);
	if (!image.data) return 0;

	cv::namedWindow("Orignal Image");
	cv::imshow("Orignal Image", image);

#pragma region 矩形区域的MSER检测
	//创建MSER检测器对象指针
	cv::Ptr<cv::MSER>ptrMSER = cv::MSER::create(
		5,			//局部检测时使用的增量值，用以测量相对稳定性
		200,		//要检测区域的最小面积
		2000);		//要检测区域的最大面积

	//保存被检测区域点击的向量（输出变量），点集是一个点构成的向量
	std::vector<std::vector<cv::Point>>points;	//每个区域用组成它的像素点表示
	//保存被检测区域的矩形向量（输出变量）
	std::vector<cv::Rect>rects;		//每个矩形包围一个区域

	//检测特征区域
	ptrMSER->detectRegions(image, points, rects);

	//输出检测到的特征区域的个数
	std::cout << points.size() << "MSERs detected" << std::endl;
	
	//创建三通道白色图像，在图像上用不同的颜色显示检测到的特征区域
	cv::Mat output(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));

	//创建Opencv的随机数生成器对象，准备生成随机颜色
	cv::RNG rng;

	///<遍历所有特征区域，用不同的颜色显示检测到的特征区域>

	//遍历每个检测到的特征区域，反向迭代，从最后一个元素开始迭代
	for (std::vector<std::vector<cv::Point>>::reverse_iterator it = points.rbegin();
		it != points.rend(); ++it)//遍历点集向量向量中的每一个点集（点构成的向量），一个点集表示组成当前特征区域的所有像素点
	{
		//输出当前特征区域包含的像素点个数
		std::cout << "MSER size= " << it->size() << std::endl;

		//生成随机颜色
		cv::Vec3b c(
			rng.uniform(0, 254),
			rng.uniform(0, 254),
			rng.uniform(0, 254));

		//遍历当前特征区域的点集中的每个点
		for (std::vector<cv::Point>::iterator itPts = it->begin();
			itPts != it->end(); ++itPts)
		{
			//修改每一个点的颜色
			if (output.at<cv::Vec3b>(*itPts)[0] == 255)	//如果该店没被修改过（颜色通道1的值还是255）
				output.at<cv::Vec3b>(*itPts) = c;		//修改该点颜色

		}
	}

	//显示结果
	cv::namedWindow("MSER point sets");
	cv::imshow("MSER point sets", output);

	///<将特征区域中属于矩形的提取出来>

	//创建被检测区域的矩形向量的迭代器
	std::vector<cv::Rect>::iterator itr = rects.begin();
	//创建被检测区域点集的向量的迭代器
	std::vector<std::vector<cv::Point>>::iterator itp = points.begin();

	//遍历每一个检测区域的矩形
	for (; itr != rects.end(); ++itr, ++itp)
	{
		//如果当前特征区域的面积/特征区域的矩形的面积>0.6
		//即特征区域的点有60%以上属于矩形所围成的区域，就认为这个特征区域是矩形区域
		if (static_cast<double>(itp->size()) / itr->area() > 0.6)
			//在原图像上将这个矩形区域用白色画出来
			cv::rectangle(image, *itr, cv::Scalar(255), 2);
	}

	//显示结果图像
	cv::namedWindow("Rectangle MSERs");
	cv::imshow("Rectangle MSERs", image);
	cv::imwrite("MSER.jpg", image);
	cv::waitKey();
	cv::destroyAllWindows();
#pragma endregion

#pragma region 椭圆区域的MSER检测

	//重新读入输入图像
	image = cv::imread("building.jpg", cv::IMREAD_GRAYSCALE);
	if (!image.data)return 0;

	///<将特征区域中属于椭圆形的提取出来>

	//遍历点集向量中的每一个点集（点构成的向量），一个点集表示组成当前特征区域的所有像素点
	for (std::vector<std::vector<cv::Point>>::iterator it = points.begin();
		it != points.end(); ++it)
	{
		//针对当前的特征区域提取包覆其点集最小斜矩阵
		cv::RotatedRect rr = cv::minAreaRect(*it);

		//检查椭圆的长宽比，保留0.6<长宽比<1.6的特征区域
		if (rr.size.height / rr.size.width > 0.6 || rr.size.height / rr.size.width < 1.6)
			//在原图像上将这个椭圆区域用白色画出来
			cv::ellipse(image, rr, cv::Scalar(255), 2);

	}

	//显示结果图像
	cv::namedWindow("MSER ellipses");
	cv::imshow("MSER ellipses", image);
	cv::waitKey();
#pragma endregion
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
