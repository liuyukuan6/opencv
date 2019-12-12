#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define NTESTS 3
#define NITERATIONS 10

#pragma region 用指针扫描图像
void colorReduce(cv::Mat image, int div = 64)//在原始图像上更改
{
	//图像的行数
	int nl = image.rows;
	//每行的元素个数
	int nc = image.cols * image.channels();//PPT

	for (int j = 0; j < nl; j++)//行循环
	{
		//取得行的地址
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; i++)
		{
			//处理每个像素--------

			//减色算法，三个通道一起处理，指向数组的指针和数组名用法想通
			data[i] = data[i] / div * div + div / 2;//三个通道同时被处理
			//像素处理结束
		}

		//一行结束
	}

	
}
//使用输入和输出参数
	//有的程序不希望对原始图像进行修改
void colorReduceIO(const cv::Mat& image, cv::Mat& result, int div = 64)
{
	int nl = image.rows;
	int nc = image.cols;
	int nchannels = image.channels();//图像的通道数

	//这里创建一个新图像，但如果新图像的大小和类型与原图像相同，例如：调用时，colorReduceIO(image,image)
	//这个方法就不会执行任何操作，也不会修改实例，而只是直接返回，就相当于在原图像上修改
	result.create(image.rows,image.cols,image.type());

	for (int j = 0; j < nl; j++)
	{
		//取得输入图像的行j地址
		const uchar* data_in = image.ptr<uchar>(j);
		//取得输出图像的行j地址
		uchar* data_out = result.ptr<uchar>(j);

		for (int i = 0; i < nc * nchannels; i++)
		{	//处理每个像素
			data_out[i] = data_in[i] / div * div + div / 2;

			//像素处理结束

		}//一行结束
	}
}
#pragma endregion

#pragma region  用迭代器扫描图像
void colorReduce2(cv::Mat image, int div = 64)
{
	//获取迭代器（两种写法）
	cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();//用cv::Mat_类获取迭代器
	cv::MatIterator_<cv::Vec3b> itend=image.end<cv::Vec3b>();//用cv::MatIterator_类获取迭代器

	//不需要知道行数和列数
	for (; it != itend; ++it)//迭代器就类似于一个指针，遍历整个图像
	{
		//处理每个像素

		//减色算法，每个通道单独处理
		(*it)[0] = (*it)[0] / div * div + div / 2;
		(*it)[1] = (*it)[1] / div * div + div / 2;
		(*it)[2] = (*it)[2] / div * div + div / 2;

		//像素处理结束
	}
}
#pragma endregion


#pragma region 用at方法扫描图像
void colorReduce3(cv::Mat image, int div = 64)
{
	int nl = image.rows;
	int nc = image.cols;

	for (int j = 0; j < nl; j++)
	{
		for (int i = 0; i < nc; i++)
		{
			//处理每个像素

			//掐面学到的at方法访问像素，也是每个通道单独处理
			image.at<cv::Vec3b>(j, i)[0] = image.at<cv::Vec3b>(j, i)[0] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[1] = image.at<cv::Vec3b>(j, i)[1] / div * div + div / 2;
			image.at<cv::Vec3b>(j, i)[2] = image.at<cv::Vec3b>(j, i)[2] / div * div + div / 2;

		}
	}
}
#pragma endregion
int main()
{
#pragma region 减色测试
	//<用指针扫描图像>

	cv::Mat joker1 = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");

	//减色
	colorReduce(joker1,64);//N=64

	cv::namedWindow("joker1");
	cv::imshow("joker1", joker1);
	cv::waitKey();

	//<用迭代器扫描图像>
	cv::Mat joker2 = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");


	colorReduce2(joker2, 64);

	cv::namedWindow("joker2");
	cv::imshow("joker2", joker2);
	cv::waitKey();

	//<用at扫描图像>
	cv::Mat joker3 = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");


	colorReduce3(joker3, 64);

	cv::namedWindow("joker3");
	cv::imshow("joker3", joker3);
	cv::waitKey();

#pragma endregion

#pragma region 运行时间测试
	int64 t[NTESTS], tinit;

	//每个函数对应的运行时间初始化为0
	for (int i = 0; i < NTESTS; i++)
		t[i] = 0;

	cv::Mat images[NTESTS];
	cv::Mat result;

	//用来测试的函数指针数组
	typedef void(*FunctionPointer)(cv::Mat, int);
	FunctionPointer functions[NTESTS] = { colorReduce,colorReduce2,colorReduce3 };
	//也可以直接这么写
	//void(*functions[NTESTS])(cv::Mat,int)={ colorReduce,colorReduce2,colorReduce3 };

	//重复测试这些函数NITERATIONS次
	int n = NITERATIONS;
	for (int k = 0; k < n; k++)
	{
		std::cout << k << "of" << n << std::endl;

		//测试每个用不同扫描方式实现的减色函数
		for (int c = 0; c < NTESTS; c++)//遍历每个减色函数
		{
			//读入图像
			images[c] = cv::imread("C:/Users/liuyukuan/Desktop/cat/3.jpg");

			//设置计时器
			tinit = cv::getTickCount();
			//调用减色函数
			functions[c](images[c], 64);
			//计算每次函数运行的时间
			t[c] += cv::getTickCount() - tinit;

			std::cout << ".";
		}
		std::cout << std::endl;
	}
	//每个函数的简短描述
	std::string descriptions[NTESTS] = {
		"用指针扫描图像：",
		"用迭代器扫描图像",
		"用at方法扫描图像" };

	//打印出平均执行时间
	std::cout << std::endl << "---------------------------" << std::endl << std::endl;
	for (int i = 0; i < NTESTS; i++)
		std::cout << i << "." << descriptions[i] << 1000. * t[i] / cv::getTickFrequency() / n << "ms" << std::endl;

#pragma endregion

	return 0;
}

