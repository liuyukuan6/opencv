#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
//ת��Ϊ��ֵͼ����ɵĶ�ͨ��ͼ��
//nPlanes������2����
//input������ĻҶ�ͼ��
void convertToBinaryPlanes(const cv::Mat& input, cv::Mat& output, int nPlanes)
{
	///<��ɫ���㣬���Ҷ�ͼ����������Ϊ256/2^n,>
	//������ֻ�õ���16�����ӣ������ȰѻҶ�ͼ���ȼ���Ϊ16��

	//��Ҫ���ε�λ�� n=8-log(16)/log(2)=4
	int n = 8 - static_cast<int>(log(static_cast<double>(nPlanes)) / log(2.0));
	//�������������Чλ������
	uchar mask = 0xFF << n;
	//0xFF         1 1 1 1 1 1 1 1
	//0xFF << 4    1 1 1 1 0 0 0 0
	std::cout << "mask=0x" << std::hex << static_cast<int>(mask) << std::endl;

	//���������Чλ����ɫ���൱�ڻҶ���ɫ������Ϊ256/2^n
	cv::Mat reduced = input & mask;

	//Test��ɫЧ��

	//����һ��ͼ������
	std::vector<cv::Mat>planes;





}