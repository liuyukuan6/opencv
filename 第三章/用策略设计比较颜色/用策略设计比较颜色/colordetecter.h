#include<opencv2/imgproc.hpp>
#include<opencv2/imgproc/types_c.h>
#include<iostream>
#include <opencv2/core.hpp> 
#include <opencv2/highgui.hpp>

#define COLORDETECT



#pragma once
class ColorDetector
{
private:
	// ����Ҫ̽�����ɫ��Ŀ����ɫ�������
	int maxDist;

	// Ŀ����ɫ(BGR)
	cv::Vec3b target;

	//�洢Labɫ�ʿռ��µ�����ͼ��
	cv::Mat converted;
	bool useLab;//�Ƿ�ʹ��Labɫ�ʿռ�

	// �洢��ֵӳ������ͼ��
	cv::Mat result;

public:
#pragma region ���캯��
	//�սṹ�������ڴ˳�ʼ��Ĭ�ϲ���
	ColorDetector():maxDist(100), target(0, 0, 0), useLab(false) {}

	//����Ĺ��캯����ʹ��Labɫ�ʿռ�
	ColorDetector(bool useLab) :maxDist(100), target(0, 0, 0), useLab(useLab) {}

	//�����Ĺ��캯��
	ColorDetector(uchar blue,uchar green,uchar red,int mxDist=100,bool useLab=false):
		maxDist(mxDist), useLab(useLab)
	{
		//����Ŀ����ɫ
		setTargetColor(blue, green, red);
	}

#pragma endregion 
	// ������Ŀ����ɫ�Ĳ��
	int getDistanceToTargetColor(const cv::Vec3b& color) const 
	{
		return getColorDistance(color, target);
	}
	 //ͼ������������һ����ͨ����ֵͼ��
	cv::Mat process(const cv::Mat& image);

	//����������ɫ֮��ĳ������루RGBֵ���ľ���ֵ��
	//������cv::Vec3d�洢�����޷����ַ��ͣ�����ɫ��RGBֵ
	//color1:Ҫ������ص���ɫֵ
	//color2:Ŀ����ɫֵ
	int getColorDistance(const cv::Vec3b& color1, const cv::Vec3b& color2) const
	{
		//||+||+||
		return abs(color1[0] - color2[0]) +
			abs(color1[1] - color2[1]) +
			abs(color1[2] - color2[2]);
	}
#pragma region ʹ��CIE L*a*bɫ�ʿռ�ͷº���

	//����������������ڶ��ּ�����ɫ������ľ��뷽��
	cv::Mat operator()(const cv::Mat& image)
	{
		cv::Mat input;

		//�����Ҫת����Lab��ɫ�ռ�
		if (useLab)
			cv::cvtColor(image, input, CV_BGR2Lab);
		else
			input = image;

		cv::Mat output;
		//��������ͼ����Ŀ����ɫ�ľ���ľ���ֵ
		cv::absdiff(input, cv::Scalar(target), output);
		//�õ����ǣ�ÿ��ͨ����Ŀ����ɫ��Ӧͨ���Ĳ�ֵ��|B|+|G|+|R|��

		//��������ͼ���ͨ����ֳ�3����ͨ��ͼ�񣬷ֱ��ӦB,G,R
		std::vector<cv::Mat> images;
		cv::split(output, images);

		//3��ͨ���Ĳ�ֵ��ӣ�������ܳ��ֱ��͵������
		output = images[0] + images[1] + images[2];

		//��ͼ�������ֵ�������ɶ�ֵͼ��
		cv::threshold(
			output,//����ͼ��
			output,//���ͼ��
			maxDist,//Ҫ̽�����ɫ��Ŀ����ɫ�������
			255,//Ҫ�������ֵ
			cv::THRESH_BINARY_INV//��ֵ��ģʽ
		);
		//			  {	0		if output(x,y)>maxDist	����������ɫ�������ض���ֵΪ0
		//output(x,y)={
		//			  {	255		otherwise				С��������ɫ�������ض���ֵΪ255
		//������ֵ����Ķ�ֵͼ��
		return output;


	}

#pragma endregion

#pragma region Getters and Setters
	//��ȡ��ɫ������ֵ
	int getColorDistanceThreshold() const {
		return maxDist;
	}

	//����Ҫ̽�����ɫ��Ŀ����ɫ�������
	void setColorDistanceThreshold(int distance)
	{
		//�����������������������Ϊ0
		if (distance < 0)
			distance = 0;

		maxDist = distance;
	}
	//��ȡĿ����ɫ
	cv::Vec3b getTargetColor() const
	{
		return target;
	}

	//����Ŀ����ɫ
	void setTargetColor(cv::Vec3b color)
	{
		target = color;
	}

	//����Ŀ����ɫ������ΪRGBɫ�ʿռ䣩
	void setTargetColor(uchar blue, uchar green, uchar red)
	{
		//��ʼ��Ŀ����ɫ������ΪBGR
		target = cv::Vec3b(blue, green, red);
	
	}

};

