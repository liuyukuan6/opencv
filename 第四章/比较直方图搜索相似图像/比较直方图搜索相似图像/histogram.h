#pragma once


#define HISTOGRAM

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>




//�����Ҷ�ͼ���ֱ��ͼ
class Histogram1D
{
private:

	int histSize[1];		//ֱ��ͼ�����ӵ�����
	float hranges[2];		//ֱ��ͼ���ص�ֵ��Χ
	const float* ranges[1];	//ָ�����飬�洢һ��ָ��ֱ��ͼ���ص�ֵ��Χ��ָ��
	int channels[1];		//Ҫ�����ͨ������



public:

	Histogram1D()			//���캯��
	{
		/// <׼��һάֱ��ͼ��Ĭ�ϲ���>
		/// <����֮������ô���ã���Ϊ�˺ͺ����calHist()���������������Ӧ����>

		//ֱ��ͼ���Ӹ���
		histSize[0] = 256;

		//ֱ��ͼ����ֵ�ķ�Χ
		hranges[0] = 0.0;		//��0��ʼ
		hranges[1] = 256.0;
		ranges[0] = hranges;	//��ָ���������洢ֱ��ͼ����ֵ�ķ�Χ

		//�Ҷ�ͼ��ֻ��һ��ͨ��
		channels[0] = 0;		//ֻ��עͨ��0
	
	}

#pragma  region  Setters and Getters

	//����Ҫ����ֱ��ͼ��ͨ����Ĭ���������ͨ��0
	void setChannel(int c) { channels[0] = c; };

	//��ȡҪ����ֱ��ͼ��ͨ��
	int getChannel() { return channels[0]; };

	//��������ֵ�ķ�Χ��Ĭ��ֵ��(0,256]
	void setRange(float minValue, float maxValue)
	{
		hranges[0] = minValue;
		hranges[1] = maxValue;
	}

	//��ȡ��С����ֵ
	float getMinValue() { return hranges[0]; }

	//��ȡ�������ֵ
	float getMaxValue() { return hranges[1]; }

	//��ֱ��ͼ���������ӵ�������Ĭ��ֵ��256
	void setNBins(int nbins) { histSize[0] = nbins; }

	//��ȡֱ��ͼ�е�Ͱ��
	int getNBins() { return histSize[0]; }

#pragma endregion

	//����һάֱ��ͼ
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
			1,				//ֱ��ͼ��ά����ͨ������������һά��ֱ��ͼ����Ҷ�ͼ���Ӧ��
			histSize,		//ֱ��ͼÿ��ά�ȣ�ͨ�����ϵ�����������һά��256�����ӣ�
			ranges			//ֱ��ͼ����ֵ�ķ�Χ������ֵ����Щ��Χ�ڵ����أ�������(0,256)��
			);
		return hist;
	}

	//����һάֱ��ͼ��������ͼ��
	//zoom��������
	cv::Mat getHistogramImage(const cv::Mat& image, int zoom = 2)
	{
		//����ֱ��ͼ
		cv::Mat hist=getHistogram(image);


		//����ͼ��
		return Histogram1D::getImageOfHistogram(hist, zoom);
	
	}

#pragma region ��̬�������෽����

	//����һ����ʾֱ��ͼ��ͼ�񣨾�̬������
	static cv::Mat getImageOfHistogram(const cv::Mat& hist, int zoom)
	{
		//ȡ������ֵ�����ֵ����Сֵ
		double maxVal = 0;//�����д�ŵ����ֵ
		double minVal = 0;//�����д�ŵ���Сֵ
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

		//ȡ��ֱ��ͼ�Ĵ�С��hist����������
		int histSize = hist.rows;//256

		//������ʾֱ��ͼ��������ͼ�񣨰�ɫ��
		cv::Mat histImg(histSize * zoom, histSize * zoom, CV_8U, cv::Scalar(255));//ͼ��ĸ߶ȵ��ڿ��

		//������ߵ�Ϊ90%�����Ӹ�������ͼ��߶ȵ�90%��
		int hpt = static_cast<int>(0.9 * histSize);

		///<Ϊÿ�����ӻ���ֱ��>

		//������������
		for (int h = 0; h < histSize; h++)
		{
			//ȡ����h�����������ֵ
			float binVal = hist.at<float>(h);//ע�⣬ֱ��ͼ�е�ֵ�洢Ϊfloatֵ

			if (binVal > 0)
			{
				//�������ǰ������ֵ��Ӧ�ĸ߶�
				/*
						��			ֵ			��			ֵ
					intensity	  binVal	 ��ǰ�߶�	  ��ǰֵ
					���������� = ��������	���������� = ��������
						hpt		  maxVal	 ���߶�	  ���ֵ

						=>��ǰ�߶ȣ�intensity=binVal*hpt/maxVal
				*/
				int intensity = static_cast<int>(binVal * hpt / maxVal);

				//������ǰ���ӵ�ֵ��Ӧ����
				cv::line(
					histImg,//���ͼ��
					cv::Point(h * zoom, histSize * zoom),//��ʼ����
					cv::Point(h * zoom, (histSize - intensity) * zoom),//��������
					cv::Scalar(0), //��ɫ:0
					zoom);//��ϸ:������
				
				
			}
		}
	
	//����ֱ��ͼ��ͼ��
		return histImg;
	}


#pragma endregion
};

