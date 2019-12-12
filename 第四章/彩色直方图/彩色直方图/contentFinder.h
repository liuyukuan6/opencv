#pragma once


#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "histogram.h"

class ContentFinder
{
private:
	///<ֱ��ͼ�����������Դ�������ͨ��>
	float hranges[2];//ֱ��ͼÿ��ά�ȣ�ͨ���������صķ�Χ��һ������ά����ͬһ�����أ�
	const float* ranges[3];//ָ�����飬�洢ÿ��ά�ȣ�ͨ������ָ��ֱ��ͼ���ص�ֵ��Χ��ָ��
	int channels[3];//��Ҫ�����ͨ������

	float threshold;//����ֵ�������õ���ֵ
	cv::Mat histogram;//����ֱ��ͼ

public:
	//���캯��
	ContentFinder() :threshold(0.1f)
	{
		//ֱ��ͼÿ��ά�ȣ�ͨ���������ص�ֵ��Χ[0,256)
		hranges[0] = 0.0;
		hranges[1] = 256.0;
	

		//����������ͨ���ķ�Χ��ͬ
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;

		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
	
	}
#pragma region setters and gatters
	//��������ֵ���������ֵ����ΧΪ[0,1]
	void setThreshold(float t)
	{
		threshold = t;
	}
	//��ȡ����ֵ�������õ���ֵ
	float getThreshold() { return threshold; }

	//����Ҫʹ�õ�ֱ��ͼ�����õ�ͬʱ���й�һ������
	void setHistogram(const cv::Mat& h)
	{
		//��һ��
		cv::normalize(h, histogram, 1.0);
	}
#pragma endregion

	//����ں�����ʹ�ù��캯����Ĭ�ϳ�ʼ������
	//Ҳ���Ե��������find()������ֻ��������Ĳ����϶࣬����򻯰�ʹ��Ĭ�ϲ���
	cv::Mat find(const cv::Mat& image)
	{
		//���������һ�����ص�find()����������Ĳ����϶�
		return find(image, hranges[0], hranges[1], channels);
	
	}

	//��������ֱ��ͼ������
	cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int* channels)
	{
		cv::Mat result;

		///<������ⲿֱ�ӵ��ñ���������Ҫ��ʼ��һЩ��Ա����>

		//ֱ��ͼÿ��ά�������ص�ֵ��Χ
		hranges[0] = minValue;
		hranges[1] = maxValue;

		//��������ͨ��
		for (int i = 0; i < histogram.dims; i++)
			this->channels[i] = channels[i];

		//����ͶӰֱ��ͼ
		cv::calcBackProject(
			&image,		//����ͼ��ĵ�ַ
			1,			//ֻʹ��1��ͼ��
			channels,	//�õ���ͨ����ȡ����ֱ��ͼ��ά��
			histogram,	//��Ҫ����ͶӰ��ֱ��ͼ
			result,		//����ͶӰ�õ��Ľ�������ʷֲ�ͼ��
			ranges,		//ֱ��ͼÿ��ά��������ֵ�ķ�Χ
			255.0	//ѡ�õĻ���ϵ�����Ѹ���ֵ��[0,1]ӳ�䵽[0,255]
		);
		//�����result�Ǹ��ʷֲ�ͼ������ĸ�����ָ����ͼ����ÿ�������������Ƕ����ROI����������ĸ���

		//�Է���ͶӰ�������ֵ�����õ���ֵͼ��
		if (threshold > 0.0)//��ֵ��Ҫ����0������ֱ�ӷ���ԭʼ�ĸ��ʷֲ�ͼ
			cv::threshold(result, result, 255.0 * threshold, 255.0, cv::THRESH_BINARY);

		return result;
	}



};

