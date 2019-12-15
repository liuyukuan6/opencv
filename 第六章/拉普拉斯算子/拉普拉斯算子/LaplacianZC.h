
#if !defined LAPLACEZC
#define LAPLACEZC

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class LaplacianZC {

private:
	// ������˹����
	cv::Mat laplace;

	// ������˹�ں˵Ŀ׾���С
	int aperture;

public:

	LaplacianZC() : aperture(3) {}

	// �����ں˵Ŀ׾���С
	void setAperture(int a) {

		aperture = a;
	}

	// Get the aperture size of the kernel
	int getAperture() const {

		return aperture;
	}

	// ���㸡�������͵�������˹����
	cv::Mat computeLaplacian(const cv::Mat& image) {

		// ����������˹����
		cv::Laplacian(image, laplace, CV_32F, aperture);

		return laplace;
	}

	// ���������˹��������� 8 λͼ����
	// 0 ��ʾ�Ҷȼ� 128
	// �����ָ�����ű�������ô���ֵ��Ŵ� 255
	// �ڵ����������֮ǰ�������ȵ��� computeLaplacian
	cv::Mat getLaplacianImage(double scale = -1.0) {

		if (scale < 0) {

			double lapmin, lapmax;
			// ȡ����С�����������˹ֵ
			cv::minMaxLoc(laplace, &lapmin, &lapmax);
			// ����������˹���ӵ� 127
			scale = 127 / std::max(-lapmin, lapmax);
		}

		cv::Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);

		return laplaceImage;
	}

	// ��ù����Ķ�ֵͼ��
	// ������˹ͼ������ͱ����� CV_32F
	cv::Mat getZeroCrossings(cv::Mat laplace) {

		// ��ֵΪ 0 
		// �����ú�ɫ
		// �����ð�ɫ
		cv::Mat signImage;
		cv::threshold(laplace, signImage, 0, 255, cv::THRESH_BINARY);

		// ��+/-ͼ��ת���� CV_8U
		cv::Mat binary;
		signImage.convertTo(binary, CV_8U);

		// ����+/-����Ķ�ֵͼ��
		cv::Mat dilated;
		cv::dilate(binary, dilated, cv::Mat());

		// ���ع���������
		return dilated - binary;
	}
};


#endif
