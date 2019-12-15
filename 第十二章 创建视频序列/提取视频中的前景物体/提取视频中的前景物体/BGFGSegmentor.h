#if !defined BGFGSeg
#define BGFGSeg

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

#include "videoprocessor.h"

class BGFGSegmentor : public FrameProcessor {

	cv::Mat gray;			// ��ǰ�Ҷ�ͼ��
	cv::Mat background;		// �ۻ��ı���
	cv::Mat backImage;		// ��ǰ����ͼ��
	cv::Mat foreground;		// ǰ��ͼ��
	double learningRate;    // �ۼƱ���ʱʹ�õ�ѧϰ����
	int threshold;			// ��ȡǰ������ֵ

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

	// ������
	void process(cv::Mat& frame, cv::Mat& output) {

		// ת���ɻҶ�ͼ��
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// ���õ�һ֡��ʼ������
		if (background.empty())
			gray.convertTo(background, CV_32F);

		// ������ת���� 8U ����
		background.convertTo(backImage, CV_8U);

		// ����ͼ���뱳��֮��Ĳ���
		cv::absdiff(backImage, gray, foreground);

		// ��ǰ��ͼ����Ӧ����ֵ
		cv::threshold(foreground, output, threshold, 255, cv::THRESH_BINARY_INV);

		// �ۻ�����
		cv::accumulateWeighted(gray, background,
			// alpha*gray + (1-alpha)*background
			learningRate,  // ѧϰ���� alpha
			output);       // ����
	}
};

#endif
