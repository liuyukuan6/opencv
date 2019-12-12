#include "colordetecter.h"
#include <vector>

cv::Mat ColorDetector::process(const cv::Mat& image)
{
	//�������ͼ��������ͼ��ĳߴ���ͬ�������ǵ�ͨ���Ҷ�ͼ�񣨶�ֵͼ��
	result.create(image.size(), CV_8U);

	//������������ͼ��ĵ�������û�г�ʼ������Ϊ���滹���ж����ĸ�ɫ�ʿռ䣩
	cv::Mat_<cv::Vec3b>::const_iterator it;
	cv::Mat_<cv::Vec3b>::const_iterator itend;

	//���ͼ��ĵ�����
	cv::Mat_<uchar>::iterator itout=result.begin<uchar>();

#pragma region ת��ɫ�ʿռ�
	//�ж��Ƿ���Ҫʹ��CIE L*a*b*ɫ�ʿռ�
	if (useLab)
		//������ͼ��Ĭ��ΪRGBɫ�ʿռ䣩ת��ΪCIE L*a*b*ɫ�ʿռ䣨���ΪLabɫ�ʿռ䣩
		cv::cvtColor(image, converted, CV_BGR2Lab);

#pragma endregion
	//��ʼ������ͼ�������
	if (useLab)//���ʹ��Labɫ�ʿռ�
	{
		it = converted.begin<cv::Vec3b>();
		itend = converted.end<cv::Vec3b>();

	}
	else
	{
		it = image.begin<cv::Vec3b>();
		itend = image.end<cv::Vec3b>();
	}

	for (; it != itend; ++it, ++itout)
	{
		///<�Ƚ�����ͼ���е�ÿ�����ص���ɫ��Ŀ����ɫ֮��Ĳ��>

		//�������ͼ���е�ǰ���ص���ɫ��Ŀ����ɫ�Ĳ�� < ������������
		if (getDistanceToTargetColor(*it) < maxDist)
			//���ͼ���е�ǰ���ص���ɫ��ֵΪ255����ɫ��
			*itout = 255;//��ʾ�����ص���ɫ��Ŀ����ɫ�ӽ�
		else
			//���ͼ���е�ǰ���ص���ɫ��ֵΪ0(��ɫ)
			*itout = 0;//��ʾ�����ص���ɫ��Ŀ����ɫ���ϴ�

	}
	//�������ͼ�񣨶�ֵͼ��
	return result;

}