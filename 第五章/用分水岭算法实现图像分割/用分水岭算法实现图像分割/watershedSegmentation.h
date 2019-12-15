#pragma once

#if !defined WATERSHS
#define WATERSHS
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>


class WatershedSegmenter
{
private:
	cv::Mat markers;

public:

	//markerImage:����ı��ͼ�񣨻Ҷȣ�
	void setMarkers(const cv::Mat& markerImage)
	{
		//ת��Ϊ������ͼ��32λ�з�������
		markerImage.convertTo(markers, CV_32S);
	
	}
	void process(const cv::Mat& image)
	{
		//Ӧ�÷�ˮ�룬���ͼ����ÿ���������ش���һ����ǩ
		//��ɫ255  => ǰ����Ұţ������ǩ
		//��ɫ128  => �������ݵأ�����ǩ
		//��ɫ0	   => δ֪���򣬷Ǳ�ǩ��Ҳ�Ƿ�ˮ���㷨Ҫʶ��ָ�����򣬾����ⲿ���������
		//Ҫô����Ϊǰ����Ҫô����Ϊ����
		cv::watershed(
			image,		//����ͼ��
			markers);	//���ͼ��Ҳ�����ͼ��32λ�з�������
		//��ˮ���Ӧ��������Ϊ-1
	}

	//��ͼ�����ʽ���طָ���
	cv::Mat getSegmentation()
	{
		cv::Mat tmp;
		//���ͼ��ת��Ϊ�Ҷ�ͼ��
		markers.convertTo(tmp, CV_8U);//���б�ǩֵ����255�����ζ���ֵΪ255����Ϊͼ��ת��Ϊ��CV_8U����ͨ��ֵ��

		return tmp;
	}

	//��ͼ����ʽ���ط�ˮ��ͼ��
	cv::Mat getWatersheds()
	{
		cv::Mat tmp;
		//ת��Ϊ�Ҷ�ͼ�񣬰�ÿ������p��ֵIpת��Ϊ��255*Ip+255
		markers.convertTo(tmp, CV_8U, 255, 255);
		/*���˷�ˮ�����ڵ�����255*(-1)+255=0���⣬
		���������ض�ת��Ϊ255������255�����ζ���ֵΪ255��ǰ��255*255+255->255������128*255+255->255��,
		����ͼ�����ǣ���ˮ��Ϊ��ɫ����������ǰ���ͱ�������Ϊ��ɫ*/
		return tmp;
	}

};







#endif