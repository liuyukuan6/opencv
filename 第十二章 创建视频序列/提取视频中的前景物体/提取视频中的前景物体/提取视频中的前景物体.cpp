// 提取视频中的前景物体.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/opencv.hpp>



#include "videoprocessor.h"
#include "BGFGSegmentor.h"

int main()
{
#pragma region
	// 创建视频处理类的实例
	VideoProcessor processor;
	// 创建背景/前景的分割器
	BGFGSegmentor segmentor;
	segmentor.setThreshold(25);

	// 打开视频文件
	processor.setInput("bike.avi");
	// 设置帧处理对象
	processor.setFrameProcessor(&segmentor);
	// 声明显示视频的窗口
	processor.displayOutput("Extracted Foreground");
	// 用原始帧速率播放视频
	processor.setDelay(1000. / processor.getFrameRate());
	// 开始处理
	processor.run();
	cv::waitKey();
	cv::destroyAllWindows();
#pragma endregion

/*
#pragma region
	// 打开视频文件
	cv::VideoCapture capture("bike.avi");
	// 检查是否成功打开视频
	if (!capture.isOpened())
		return 0;
	// 当前视频帧
	cv::Mat frame;
	// 前景的二值图像
	cv::Mat foreground;
	// 背景图像
	cv::Mat background;
	cv::namedWindow("Extracted Foreground");
	// 混合高斯模型类的对象，全部采用默认参数
	cv::Ptr<cv::BackgroundSubtractor> ptrMOG =
		createBackgroundSubtractorMOG();
	bool stop(false);
	// 遍历视频中的所有帧
	while (!stop) {
		// 读取下一帧（如果有）
		if (!capture.read(frame))
			break;
		// 更新背景并返回前景
		ptrMOG->apply(frame, foreground, 0.01);
		// 改进图像效果
		cv::threshold(foreground, foreground, 128,
			255, cv::THRESH_BINARY_INV);
		// 显示前景和背景
		cv::imshow("Extracted Foreground", foreground);
		// 产生延时，或者按键结束
		if (cv::waitKey(10) >= 0)
			stop = true;
	}

#pragma endregion
*/
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
