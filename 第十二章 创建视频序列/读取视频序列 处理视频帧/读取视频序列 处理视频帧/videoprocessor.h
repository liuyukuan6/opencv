#if !defined VPROCESSOR
#define VPROCESSOR

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// 帧处理的接口
class FrameProcessor {

public:
	// 处理方法
	virtual void process(cv::Mat& input, cv::Mat& output) = 0;
};

class VideoProcessor {

private:

	// OpenCV 视频捕获对象
	cv::VideoCapture capture;
	// // 处理每一帧时都会调用的回调函数
	void (*process)(cv::Mat&, cv::Mat&);
	// the pointer to the class implementing 
	// the FrameProcessor interface
	FrameProcessor* frameProcessor;
	//  布尔型变量，表示该回调函数是否会被调用
	bool callIt;
	// 输入窗口的显示名称
	std::string windowNameInput;
	// 输出窗口的显示名称
	std::string windowNameOutput;
	// 帧之间的延时
	int delay;
	// 已经处理的帧数 
	long fnumber;
	// 达到这个帧数时结束
	long frameToStop;
	// 结束处理
	bool stop;

	// 作为输入对象的图像文件名向量
	std::vector<std::string> images;
	//  图像向量的迭代器
	std::vector<std::string>::const_iterator itImg;

	// OpenCV 写视频对象
	cv::VideoWriter writer;
	// 输出文件名
	std::string outputFile;

	// 输出图像的当前序号
	int currentIndex;
	// 输出图像文件名中数字的位数
	int digits;
	// 输出图像的扩展名
	std::string extension;

	// 取得下一帧
	// 可以是视频文件、摄像机、图像向量
	// 它能根据输入内容，选择从视频读取还是从文
	// 件名向量读取。判断方法是查看图像文件名向量是否为空，如不为空就表明输入是图像序列。
	bool readNextFrame(cv::Mat& frame) {

		if (images.size() == 0)
			return capture.read(frame);
		else {

			if (itImg != images.end()) {

				frame = cv::imread(*itImg);
				itImg++;
				return frame.data != 0;
			}

			return false;
		}
	}

	// 写输出的帧
	// 可以是视频文件或图像组
	void writeNextFrame(cv::Mat& frame) {

		if (extension.length()) { // 写入到图像组

			std::stringstream ss;
			// 组合成输出文件名
			ss << outputFile << std::setfill('0') << std::setw(digits) << currentIndex++ << extension;
			cv::imwrite(ss.str(), frame);

		}
		else { // 写入到视频文件

			writer.write(frame);
		}
	}

public:

	// Constructor setting the default values
	VideoProcessor() : callIt(false), delay(-1),
		fnumber(0), stop(false), digits(0), frameToStop(-1),
		process(0), frameProcessor(0) {}

	// 设置视频文件的名称
	bool setInput(std::string filename) {

		fnumber = 0;
		//  防止已经有资源与 VideoCapture 实例关联
		capture.release();
		images.clear();

		//  打开视频文件
		return capture.open(filename);
	}

	// set the camera ID
	bool setInput(int id) {

		fnumber = 0;
		// In case a resource was already 
		// associated with the VideoCapture instance
		capture.release();
		images.clear();

		// Open the video file
		return capture.open(id);
	}

	// 设置输入图像的向量
	bool setInput(const std::vector<std::string>& imgs) {

		fnumber = 0;
		//  防止已经有资源与 VideoCapture 实例关联
		capture.release();

		// 将这个图像向量作为输入对象
		images = imgs;
		itImg = images.begin();

		return true;
	}

	// 设置输出视频文件
// 默认情况下会使用与输入视频相同的参数
	bool setOutput(const std::string& filename, int codec = 0, double framerate = 0.0, bool isColor = true) {

		outputFile = filename;
		extension.clear();

		if (framerate == 0.0)
			framerate = getFrameRate(); // 与输入相同

		char c[4];
		// 使用与输入相同的编解码器
		if (codec == 0) {
			codec = getCodec(c);
		}

		// 打开输出视频
		return writer.open(outputFile, // 文件名
			codec, // 所用的编解码器 
			framerate,      // 视频的帧速率
			getFrameSize(), // 帧的尺寸
			isColor);       // 彩色视频？
	}

	// 设置输出为一系列图像文件
	// 扩展名必须是.jpg 或.bmp
	bool setOutput(const std::string& filename, // 前缀
		const std::string& ext, // 图像文件的扩展名 
		int numberOfDigits = 3,   // 数字的位数
		int startIndex = 0) {     // 开始序号

		// 数字的位数必须是正数
		if (numberOfDigits < 0)
			return false;

		// 文件名和常用的扩展名
		outputFile = filename;
		extension = ext;

		// 文件编号方案中数字的位数
		digits = numberOfDigits;
		// 从这个序号开始编号
		currentIndex = startIndex;

		return true;
	}

	//  设置针对每一帧调用的回调函数
	void setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&)) {

		// invalidate frame processor class instance
		frameProcessor = 0;
		// this is the frame processor function that will be called
		process = frameProcessingCallback;
		callProcess();
	}

	
	//为 VideoProcessor 框架输入一个 FrameProcessor 实例，并把这个实
	//例赋给新增的成员变量 frameProcessor，这个成员变量是指向 FrameProcessor 对象的指针
	//设置实现 FrameProcessor 接口的实例
	void setFrameProcessor(FrameProcessor* frameProcessorPtr) {

		// 使回调函数失效
		process = 0;
		// 这个就是即将被调用的帧处理实例
		frameProcessor = frameProcessorPtr;
		callProcess();
	}

	// 指定多少帧停止
	void stopAtFrameNo(long frame) {

		frameToStop = frame;
	}

	// 需要调用回调函数 process
	void callProcess() {

		callIt = true;
	}

	// 不需要调用回调函数 process
	void dontCallProcess() {

		callIt = false;
	}

	// 用于显示输入的帧
	void displayInput(std::string wn) {

		windowNameInput = wn;
		cv::namedWindow(windowNameInput);
	}

	// 用于显示处理过的帧
	void displayOutput(std::string wn) {

		windowNameOutput = wn;
		cv::namedWindow(windowNameOutput);
	}

	// do not display the processed frames
	void dontDisplay() {

		cv::destroyWindow(windowNameInput);
		cv::destroyWindow(windowNameOutput);
		windowNameInput.clear();
		windowNameOutput.clear();
	}

	

	// a count is kept of the processed frames
	long getNumberOfProcessedFrames() {

		return fnumber;
	}

	// return the size of the video frame
	cv::Size getFrameSize() {

		if (images.size() == 0) {

			// get size of from the capture device
			int w = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
			int h = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

			return cv::Size(w, h);

		}
		else { // if input is vector of images

			cv::Mat tmp = cv::imread(images[0]);
			if (!tmp.data) return cv::Size(0, 0);
			else return tmp.size();
		}
	}

	// 返回下一帧的编号
	long getFrameNumber() {

		if (images.size() == 0) {

			// 从捕获设备获取信息
			long f = static_cast<long>(capture.get(cv::CAP_PROP_POS_FRAMES));
			return f;

		}
		else { // if input is vector of images

			return static_cast<long>(itImg - images.begin());
		}
	}

	// return the position in ms
	double getPositionMS() {

		// undefined for vector of images
		if (images.size() != 0) return 0.0;

		double t = capture.get(cv::CAP_PROP_POS_MSEC);
		return t;
	}

	// return the frame rate
	double getFrameRate() {

		// undefined for vector of images
		if (images.size() != 0) return 0;

		double r = capture.get(cv::CAP_PROP_FPS);
		return r;
	}

	// return the number of frames in video
	long getTotalFrameCount() {

		// for vector of images
		if (images.size() != 0) return images.size();

		long t = capture.get(cv::CAP_PROP_FRAME_COUNT);
		return t;
	}

	//  取得输入视频的编解码器
	int getCodec(char codec[4]) {

		// 本方法对图像向量无意义
		if (images.size() != 0) return -1;

		union { // 表示四字符代码的数据结构
			int value;
			char code[4];
		} returned;

		//取得代码
		returned.value = static_cast<int>(capture.get(cv::CAP_PROP_FOURCC));
		// 取得四个字符
		codec[0] = returned.code[0];
		codec[1] = returned.code[1];
		codec[2] = returned.code[2];
		codec[3] = returned.code[3];

		//返回代码的整数值
		return returned.value;
	}

	// go to this frame number
	bool setFrameNumber(long pos) {

		// for vector of images
		if (images.size() != 0) {

			// move to position in vector
			itImg = images.begin() + pos;
			// is it a valid position?
			if (pos < images.size())
				return true;
			else
				return false;

		}
		else { // if input is a capture device

			return capture.set(cv::CAP_PROP_POS_FRAMES, pos);
		}
	}

	// go to this position
	bool setPositionMS(double pos) {

		// not defined in vector of images
		if (images.size() != 0)
			return false;
		else
			return capture.set(cv::CAP_PROP_POS_MSEC, pos);
	}

	// go to this position expressed in fraction of total film length
	bool setRelativePosition(double pos) {

		// for vector of images
		if (images.size() != 0) {

			// move to position in vector
			long posI = static_cast<long>(pos * images.size() + 0.5);
			itImg = images.begin() + posI;
			// is it a valid position?
			if (posI < images.size())
				return true;
			else
				return false;

		}
		else { // if input is a capture device

			return capture.set(cv::CAP_PROP_POS_AVI_RATIO, pos);
		}
	}

	// 设置帧之间的延时，
// 0 表示每一帧都等待，
// 负数表示不延时
	void setDelay(int d) {

		delay = d;
	}

	// 结束处理
	void stopIt() {

		stop = true;
	}

	// 处理过程是否已经停止？
	bool isStopped() {

		return stop;
	}

	// 捕获设备是否已经打开？
	bool isOpened() {

		return capture.isOpened() || !images.empty();
	}

	//  抓取（并处理）序列中的帧
	void run() {

		// 当前帧
		cv::Mat frame;
		// 输出帧
		cv::Mat output;

		// 如果没有设置捕获设备
		if (!isOpened())
			return;

		stop = false;

		while (!isStopped()) {

			// 读取下一帧（如果有）
			if (!readNextFrame(frame))
				break;

			// 显示输入的帧
			if (windowNameInput.length() != 0)
				cv::imshow(windowNameInput, frame);

			//  ** 调用处理函数或方法 **
			if (callIt) {

				//  处理帧
				if (process) //如果是回调函数
					process(frame, output);
				else if (frameProcessor)
					// 如果是类的接口
					frameProcessor->process(frame, output);//->的功能就是提供了一种对象指针更方便的访问对象成员的方法
				// 递增帧数
				fnumber++;

			}
			else {
				//没有处理
				output = frame;
			}

			//  ** 写入到输出的序列 **
			if (outputFile.length() != 0)
				writeNextFrame(output);

			//  显示输出的帧
			if (windowNameOutput.length() != 0)
				cv::imshow(windowNameOutput, output);

			// 产生延时
			if (delay >= 0 && cv::waitKey(delay) >= 0)
				stopIt();

			// 检查是否需要结束
			if (frameToStop >= 0 && getFrameNumber() == frameToStop)
				stopIt();
		}
	}
};

#endif
