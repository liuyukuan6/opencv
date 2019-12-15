
#if !defined LAPLACEZC
#define LAPLACEZC

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class LaplacianZC {

private:
	// 拉普拉斯算子
	cv::Mat laplace;

	// 拉普拉斯内核的孔径大小
	int aperture;

public:

	LaplacianZC() : aperture(3) {}

	// 设置内核的孔径大小
	void setAperture(int a) {

		aperture = a;
	}

	// Get the aperture size of the kernel
	int getAperture() const {

		return aperture;
	}

	// 计算浮点数类型的拉普拉斯算子
	cv::Mat computeLaplacian(const cv::Mat& image) {

		// 计算拉普拉斯算子
		cv::Laplacian(image, laplace, CV_32F, aperture);

		return laplace;
	}

	// 获得拉普拉斯结果，存在 8 位图像中
	// 0 表示灰度级 128
	// 如果不指定缩放比例，那么最大值会放大到 255
	// 在调用这个函数之前，必须先调用 computeLaplacian
	cv::Mat getLaplacianImage(double scale = -1.0) {

		if (scale < 0) {

			double lapmin, lapmax;
			// 取得最小和最大拉普拉斯值
			cv::minMaxLoc(laplace, &lapmin, &lapmax);
			// 缩放拉普拉斯算子到 127
			scale = 127 / std::max(-lapmin, lapmax);
		}

		cv::Mat laplaceImage;
		laplace.convertTo(laplaceImage, CV_8U, scale, 128);

		return laplaceImage;
	}

	// 获得过零点的二值图像
	// 拉普拉斯图像的类型必须是 CV_32F
	cv::Mat getZeroCrossings(cv::Mat laplace) {

		// 阈值为 0 
		// 负数用黑色
		// 正数用白色
		cv::Mat signImage;
		cv::threshold(laplace, signImage, 0, 255, cv::THRESH_BINARY);

		// 把+/-图像转换成 CV_8U
		cv::Mat binary;
		signImage.convertTo(binary, CV_8U);

		// 膨胀+/-区域的二值图像
		cv::Mat dilated;
		cv::dilate(binary, dilated, cv::Mat());

		// 返回过零点的轮廓
		return dilated - binary;
	}
};


#endif
