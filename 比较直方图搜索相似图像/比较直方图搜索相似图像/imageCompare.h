#define ICOMPARATOR

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "colorhistogram.h"

class ImageComparator
{
private:
	cv::Mat refH;//�ο�ͼ���ֱ��ͼ
	cv::Mat inputH;//����ͼ���ֱ��ͼ

	ColorHistogram hist;//�����ɫֱ��ͼ��Ķ���
	int nBins;//ÿ��ͨ��ʹ�õ�ֱ��ͼ��������

public:

	//���캯�����Ƚ�ֱ��ͼʱÿ����ɫͨ���õ���������Ĭ��Ϊ8
	ImageComparator() :nBins(8) {}

	//���ñȽ�ֱ��ͼʱʹ�õ���������
	void getNumberOfBins(int bins) { nBins=bins; }

	//��ȡ�Ƚ�ֱ��ͼʱʹ�õ�������
	int getNumberOfBins() { return nBins; }

	//���ò�����ο�ͼ���ֱ��ͼ
	void setReferenceImage(const cv::Mat& image)
	{
		//����ÿ��ͨ������������
		hist.setSize(nBins);
		//����ο�ͼ���ֱ��ͼ
		refH = hist.getHistogram(image);
	
	}
	double compare(const cv::Mat& image)
	{
		//��������ͼ���ֱ��ͼ
		inputH = hist.getHistogram(image);

		//�ý��淨�Ƚ�ֱ��ͼ���������ƶ�
		return cv::compareHist(
			refH,//�ο�ͼ���ֱ��ͼ
			inputH,//����ͼ���ֱ��ͼ
			cv::HISTCMP_INTERSECT);
		//ʹ�ý���㷽����������ӵرȽ�ÿ��ֱ��ͼ�е���ֵ����������С��ֵ��Ȼ�����Щ��Сֵ�ۼӣ���Ϊ���ƶȲ���ֵ


	
	
	}



};
