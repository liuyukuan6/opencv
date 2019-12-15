// 读取视频序列.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <iomanip> //sefill
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include "videoprocessor.h"

void canny(cv::Mat& img, cv::Mat& out) {
	// 转换成灰度图像
	if (img.channels() == 3)
		cv::cvtColor(img, out, cv::COLOR_BGR2GRAY);
	// 计算 Canny 边缘
	cv::Canny(out, out, 100, 200);
	// 反转图像
	cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

int main()
{
#pragma region 读取视频序列
	// 打开视频文件
	cv::VideoCapture capture("bike.avi");
	// 检查视频是否成功打开
	if (!capture.isOpened())
		return 1;

	// 取得帧速率
	double rate = capture.get(cv::CAP_PROP_FPS);
	std::cout << "Frame rate: " << rate << "fps" << std::endl;

	bool stop(false);

	cv::Mat frame; // 当前视频帧
	cv::namedWindow("Extracted Frame");

	// 根据帧速率计算帧之间的等待时间，单位为 ms 
	int delay = 1000 / rate;
	// 循环遍历视频中的全部帧
	long long i = 0;
	std::string b = "bike";
	std::string ext = ".bmp";

	while (!stop) {
		// 读取下一帧（如果有）
		if (!capture.read(frame))
			break;
		cv::imshow("Extracted Frame", frame);

		std::string name(b);
		std::ostringstream ss;
		ss << std::setfill('0') << std::setw(3) << i;
		name += ss.str();
		i++;
		name += ext;

		std::cout << name << std::endl;

		cv::Mat test;
		//      cv::resize(frame, test, cv::Size(), 0.2,0.2);
		//		cv::imwrite(name, frame);
		//		cv::imwrite(name, test);

		// 等待一段时间，或者通过按键停止
		if (cv::waitKey(delay) >= 0)
			stop = true;
	}
	// 关闭视频文件
	// 不是必须的，因为类的析构函数会调用
	capture.release();
#pragma endregion

#pragma region 处理视频帧

	// 创建实例
	VideoProcessor processor;
	// 打开视频文件
	processor.setInput("bike.avi");
	// 声明显示视频的窗口
	processor.displayInput("Current Frame");
	processor.displayOutput("Output Frame");
	// 用原始帧速率播放视频
	processor.setDelay(1000. / processor.getFrameRate());
	// 设置处理帧的回调函数
	processor.setFrameProcessor(canny);
	// 开始处理
	processor.run();
	capture.release();


#pragma endregion

#pragma region 写入视频帧
	
	// 打开视频文件
	processor.setInput("bike.avi");
	processor.setFrameProcessor(canny);
	processor.setOutput("bikeOut", // 前缀 
		".jpg", // 扩展名
		3, // 数字的位数
		0); // 开始序号
	// 开始处理
	processor.run();
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
