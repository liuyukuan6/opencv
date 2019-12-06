#pragma once

#define COLGISTOGRAM

#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>


class ColorHistogram
{
private:

	///<��ɫͼ��3��ͨ��>
	int histSize[3];	//ֱ��ͼÿ��ά�ȣ�ͨ�����ϵ���������
	float hranges[2];	//ֱ��ͼÿ��ά�ȣ�ͨ���������ص�ֵ��Χ��һ������ά����ͬһ����Χ��
	const float* ranges[3];//ָ�����飬�洢ÿ��ά����ָ��ֱ��ͼ���ص�ֵ��Χ��ָ��
	int channels[3];	//��Ҫ�����ͨ������

public:

	ColorHistogram()
	{	
		//׼����ɫͼ��Ĳ���
		//ÿ��ͨ�������������Ĵ�С�ͷ�Χ����ȵ�
		histSize[0] = histSize[1] = histSize[2] = 256;//Ĭ�ϵ�ֱ��ͼ��ÿ��ͨ���������������

		//ֱ��ͼÿ��ά���ϵ�����ֵ��Χ
		hranges[0] = 0.0;
		hranges[1] = 256.0;

		//��ָ���������洢ÿ��ά������ֵ�ķ�Χ
		//����������ͨ���ķ�Χ��ͬ
		ranges[0] = hranges;
		ranges[0] = hranges;
		ranges[0] = hranges;

	
		//��ɫͨ��
		channels[0] = 0;//B
		channels[1] = 1;//G
		channels[2] = 2;//R
	
	}

	void setSize(int size)
	{
		histSize[0] = histSize[1] = histSize[2] = size;
	}

	//����ֱ��ͼ
	cv::Mat getHistogram(const cv::Mat& image)
	{
		
		cv::Mat hist;

		//��calcHist��������һάֱ��ͼ
		cv::calcHist(
				&image,			//Ҫ����ֱ��ͼ��ͼ��
				1,				//������ٷ�ͼ���ֱ��ͼ
				channels,		//ʹ�õ�ͨ��(�Ҷ�ͼ��ֻ��һ��ͨ��)
				cv::Mat(),		//��ʹ������(���ʹ�ã�ֻ�в���ɸѡ���������ػ���������ֱ��ͼ)
				hist,			//��Ϊ��������ֱ��ͼ
				1,				//ֱ��ͼ��ά����ͨ��������������ά��ֱ��ͼ����ʶ�ͼ���Ӧ��
				histSize,		//ֱ��ͼÿ��ά�ȣ�ͨ�����ϵ�����������һά��256�����ӣ�
				ranges			//ֱ��ͼ����ֵ�ķ�Χ������ֵ����Щ��Χ�ڵ����أ�������(0,256)��
		);
		return hist;
	
	}
	//����һάɫ��ֱ��ͼ
//minSaturation�����Ͷ���ֵ�����Եͱ��Ͷȵ�����
	cv::Mat getHueHistogram(const cv::Mat& image, int minSaturation = 0)
	{
		//����Ҫ�����ֱ��ͼ
		cv::Mat hist;

		//ת��ΪHSVɫ�ʿռ䣨Ҫ����ɫ��ֱ��ͼ��
		cv::Mat hsv;
		cv::cvtColor(image, hsv, CV_BGR2HSV);

		//���루��ֵͼ��
		cv::Mat mask;

		//������Ҫ��������
		if (minSaturation > 0)
		{
			//������ͨ���ָ��3��ͼ��
			std::vector<cv::Mat>v;
			cv::split(hsv, v);

			//����ֵ�����εͱ��Ͷȵ����أ����Ͷ�С��minSaturation�����ؾͲ�������
			cv::threshold(v[1], mask, minSaturation, 255, cv::THRESH_BINARY);


		}

		///<׼��һάɫ��ֱ��ͼ�Ĳ���>

		//ɫ���ķ�ΧΪ0~180
		hranges[0] = 0.0;
		hranges[1] = 180.0;
		//ɫ��ͨ����ֻ����һ��ͨ����
		channels[0] = 0;

		cv::calcHist(
			&hsv,			//Ҫ����ֱ��ͼ��ͼ��(HSVɫ�ʿռ�)
			1,				//������ٸ�ͼ���ֱ��ͼ��������һ��ͼ���ֱ��ͼ��
			channels,		//ʹ�õ�ͨ��(ֻ��һ��ɫ���ռ�)
			mask,			//ʹ������
			hist,			//��Ϊ��������ֱ��ͼ
			1,				//ֱ��ͼ��ά��
			histSize,		//ֱ��ͼÿ��ά���ϵ���������
			ranges			//ֱ��ͼ����ֵ�ķ�Χ����������Щ��Χ�ڵ����أ�������[0,180]��
		);

		return hist;
	}

};

