#if !defined BGFGSeg
#define BGFGSeg

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

#include "videoprocessor.h"

class BGFGSegmentor : public FrameProcessor {

	cv::Mat gray;			// 当前灰度图像
	cv::Mat background;		// 累积的背景
	cv::Mat backImage;		// 当前背景图像
	cv::Mat foreground;		// 前景图像
	double learningRate;    // 累计背景时使用的学习速率
	int threshold;			// 提取前景的阈值

public:

	BGFGSegmentor() : threshold(10), learningRate(0.01) {}

	// Set the threshold used to declare a foreground
	void setThreshold(int t) {

		threshold = t;
	}

	// Set the learning rate
	void setLearningRate(double r) {

		learningRate = r;
	}

	// 处理方法
	void process(cv::Mat& frame, cv::Mat& output) {

		// 转换成灰度图像
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// 采用第一帧初始化背景
		if (background.empty())
			gray.convertTo(background, CV_32F);

		// 将背景转换成 8U 类型
		background.convertTo(backImage, CV_8U);

		// 计算图像与背景之间的差异
		cv::absdiff(backImage, gray, foreground);

		// 在前景图像上应用阈值
		cv::threshold(foreground, output, threshold, 255, cv::THRESH_BINARY_INV);

		// 累积背景
		cv::accumulateWeighted(gray, background,
			// alpha*gray + (1-alpha)*background
			learningRate,  // 学习速率 alpha
			output);       // 掩码
	}
};

#endif
