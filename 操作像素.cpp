// 操作像素.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <random>


void salt(cv::Mat image, int n) {
	// C++11 的随机数生成器
	//这里调用默认的构造函数，因此会用默认种子来设置初始状态
	//在不同的时刻执行从generator产生的随机数字序列总是相同的，因为种子保持不变
	//当然，也能自己提供种子
	std::default_random_engine generator;//定义一个默认随机数生成器

	//离散均匀分布的随机数的范围（上边界和下边界）
	//返回均匀分布在闭合范围[a，b]内的随机整数
	std::uniform_int_distribution<int>
		randomRow(0, image.rows - 1);//椒盐噪声的y坐标的随机数范围,随机产生的数可能会重复
	std::uniform_int_distribution<int>
		randomCol(0, image.cols - 1);//椒盐噪声的x坐标的随机数范围
	int i, j;
	for (int k = 0; k < n; k++) {//生成n个椒盐噪声
		// 随机生成图形位置
		i = randomCol(generator);//将随机数生成器对象传给随机数范围对象，生成椒盐噪声的x坐标
		j = randomRow(generator);//生成椒盐噪声的y坐标
		if (image.type() == CV_8UC1) { // 灰度图像
	   // 单通道 8 位图像
			image.at<uchar>(j, i) = 255;//颜色替换成白色，增加椒盐噪声
		}
		else if (image.type() == CV_8UC3) { // 彩色图像
			//可以替换成任意的颜色，不只是白色（255,255,255）

			//方式一
			// 3 通道图像，下标0,1,2是通道
			//Vec3b:含三个无符号字符(uchar)类型的数据。即Vec<uchar,3>
			//image.at<cv::Vec3b>(j, i)[0] = 255;//B
			//image.at<cv::Vec3b>(j, i)[1] = 255;//G
			//image.at<cv::Vec3b>(j, i)[2] = 255;//R

			//方式二
			//或者，直接使用短向量：颜色替换成白色 ，增加椒盐噪声
			image.at<cv::Vec3b>(j, i) = cv::Vec3b(0, 0, 255);
		}

	}
}

void salt2(cv::Mat image, int n)
{	//必须是灰度图像
	CV_Assert(image.type() == CV_8UC1);

	//C++11的默认随机数生成器
	std::default_random_engine b;
	std::uniform_int_distribution<int>randomRow(0, image.rows - 1);
	std::uniform_int_distribution<int>randomCol(0, image.cols - 1);

	//用Mat_模板操作图像(cv::Mat -> cv::Mat_)
	cv::Mat_<uchar> img(image);//将cv::Mat_类的对象img初始化为图像image，数据类型为unchar

	//或者使用引用:
	//创建一个cv::Mat_类的引用，但需要进行数据类型转换，将cv::Mat类型转换为cv::Mat_<unchar>&引用类型
	//cv::Mat_<uchar>& im2= reinterpret_cast<cv::Mat_<ucahr>&>(image);
	
	int i, j;
	for (int k = 0; k < n; k++)
	{
		//随机生成图形位置
		i = randomCol(b);
		j = randomRow(b);

		//这里只考虑灰度图像
		//颜色替换成白色，增加椒盐噪声
		img(j, i) = 255;//使用Mat_模板访问像素的方式
	}




}

int main()
{
#pragma region椒盐噪声

	cv::Mat joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");

	//椒盐噪声（本质上就是让这个点变成白色，也可以变成其他颜色）
	salt(joker, 10000);//噪声数量10000个，但不一定确实是10000个，他只是取了10000个点，这些点有重合的 

	cv::namedWindow("Image");
	cv::imshow("Image", joker);
	cv::waitKey(0);

#pragma endregion

#pragma region cv::Mat_操作图像

	joker = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg", cv::IMREAD_GRAYSCALE);

	//椒盐噪声
	salt2(joker, 10000);

	cv::namedWindow("joker2");
	cv::imshow("joker2", joker);
	cv::waitKey();

#pragma endregion
}

