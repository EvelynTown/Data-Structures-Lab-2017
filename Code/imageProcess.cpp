#include "ImageProcess.h"

//通过.ptr<>函数得到一行的指针，并用[]操作符访问某一列的像素值。
void PixImage::colorRead(cv::Mat &image) {
	Mat imageClone = image.clone();
	int rowNumber = imageClone.rows;
	int colNumber = imageClone.cols;
	int div = 64;
	for (int i = 0; i < rowNumber; i++)	{
		color[i] = imageClone.ptr<uchar>(i);
		colorR[i] = new uchar[colNumber];
		colorG[i] = new uchar[colNumber];
		colorB[i] = new uchar[colNumber];
		for (int j = 0; j < colNumber; j++) {
			colorR[i][j] = color[i][3 * j];
			colorG[i][j] = color[i][3 * j + 1];
			colorB[i][j] = color[i][3 * j + 2];
		}
	}
}

/*对图像中的像素表达进行量化。如常见的RGB24图像有256×256×256中颜色，
通过Reduce Color将每个通道的像素减少8倍至256/8=32种，则图像只有32×32×32种颜色。
假设量化减少的倍数是N，则代码实现时就是简单的value/N*N，
通常会再加上N/2以得到相邻的N的倍数的中间值，
最后图像被量化为(256/N)×(256/N)×(256/N)种颜色。*/
void PixImage::colorReduce(cv::Mat &image, cv::Mat &imageColorReduce) {
	imageColorReduce = image.clone();
	int rowNumber = imageColorReduce.rows;
	int colNumber = imageColorReduce.cols*imageColorReduce.channels();
	int div = 64;
	for (int i = 0; i < rowNumber; i++)	{
		uchar* data = imageColorReduce.ptr<uchar>(i);
		for (int j = 0; j < colNumber; j++)	{
			data[j] = data[j] / div*div + div / 2;
		}
	}
}

//均值滤波器消除噪声
void PixImage::Bluring(cv::Mat &image, cv::Mat &imageBluring, int winSize) {
	imageBluring = image.clone();
	int rowNumber = imageBluring.rows;
	int colNumber = imageBluring.cols;
	uchar **data = new uchar*[rowNumber];
	int pos = (winSize - 1) / 2, temp = 0;

	//模糊处理中间一圈
	for (int i = pos; i < rowNumber - pos; i++) {
		data[i] = imageBluring.ptr<uchar>(i);
		for (int j = pos; j < colNumber - pos; j++) {
			temp = 0;
			//处理红色像素
			for (int winRow = 0; winRow < winSize; winRow++)
				for (int winCol = 0; winCol < winSize; winCol++)
					temp += colorR[i - pos + winRow][j - pos + winCol];
			data[i][j * 3] = temp / (winSize*winSize);
			temp = 0;
			//处理绿色像素
			for (int winRow = 0; winRow < winSize; winRow++)
				for (int winCol = 0; winCol < winSize; winCol++)
					temp += colorG[i - pos + winRow][j - pos + winCol];
			data[i][j * 3 + 1] = temp / (winSize*winSize);
			temp = 0;
			//处理蓝色像素
			for (int winRow = 0; winRow < winSize; winRow++)
				for (int winCol = 0; winCol < winSize; winCol++)
					temp += colorB[i - pos + winRow][j - pos + winCol];
			data[i][j * 3 + 2] = temp / (winSize*winSize);
		}
	}
	data[0] = imageBluring.ptr<uchar>(0);
	data[rowNumber - 1] = imageBluring.ptr<uchar>(rowNumber - 1);

	//处理左上角像素
	temp = 0;
	for (int i = 0; i <= pos; i++)
		for (int j = 0; j <= pos; j++)
			temp += colorR[i][j];
	data[0][0] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = 0; i <= pos; i++)
		for (int j = 0; j <= pos; j++)
			temp += colorG[i][j];
	data[0][1] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = 0; i <= pos; i++)
		for (int j = 0; j <= pos; j++)
			temp += colorB[i][j];
	data[0][2] = temp / ((pos + 1)*(pos + 1));

	//处理右上角像素
	temp = 0;
	for (int i = 0; i <= pos; i++)
		for (int j = colNumber-pos-1; j < colNumber; j++)
			temp += colorR[i][j];
	data[0][3 * (colNumber - 1)] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = 0; i <= pos; i++)
		for (int j = colNumber - pos - 1; j < colNumber; j++)
			temp += colorG[i][j];
	data[0][3 * (colNumber - 1) + 1] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = 0; i <= pos; i++)
		for (int j = colNumber - pos - 1; j < colNumber; j++)
			temp += colorB[i][j];
	data[0][3 * (colNumber - 1) + 2] = temp / ((pos + 1)*(pos + 1));

	//处理左下角像素
	temp = 0;
	for (int i = rowNumber - pos - 1; i < rowNumber; i++)
		for (int j = 0; j <= pos; j++)
			temp += colorR[i][j];
	data[rowNumber - 1][0] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = rowNumber - pos - 1; i < rowNumber; i++)
		for (int j = 0; j <= pos; j++)
			temp += colorG[i][j];
	data[rowNumber - 1][1] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = rowNumber - pos - 1; i < rowNumber; i++)
		for (int j = 0; j <= pos; j++)
			temp += colorB[i][j];
	data[rowNumber - 1][2] = temp / ((pos + 1)*(pos + 1));

	//处理右下角像素
	temp = 0;
	for (int i = rowNumber - pos - 1; i < rowNumber; i++)
		for (int j = colNumber - pos - 1; j < colNumber; j++)
			temp += colorR[i][j];
	data[rowNumber - 1][3 * (colNumber - 1)] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = rowNumber - pos - 1; i < rowNumber; i++)
		for (int j = colNumber - pos - 1; j < colNumber; j++)
			temp += colorG[i][j];
	data[rowNumber - 1][3 * (colNumber - 1) + 1] = temp / ((pos + 1)*(pos + 1));
	temp = 0;
	for (int i = rowNumber - pos - 1; i < rowNumber; i++)
		for (int j = colNumber - pos - 1; j < colNumber; j++)
			temp += colorB[i][j];
	data[rowNumber - 1][3 * (colNumber - 1) + 2] = temp / ((pos + 1)*(pos + 1));
	delete[]data;
}

//三通道彩色图片转换成单通道图片
int PixImage::GrayImage(cv::Mat &image, cv::Mat &imageSobel) {
	int threshL = 0, thresh = 0;
	imageSobel = image.clone();
	int rowNumber = imageSobel.rows;
	int colNumber = imageSobel.cols;
	for (int i = 0; i < rowNumber; i++) {
		uchar* data = imageSobel.ptr<uchar>(i);
		for (int j = 0; j < colNumber; j++) {
			data[j * 3] = (uchar)(0.136*data[j * 3] + 0.514*data[j * 3 + 1] + 0.350*data[j * 3 + 2]);
				data[j * 3 + 1] = data[j * 3];
				data[j * 3 + 2] = data[j * 3];
				threshL += data[3 * j];
		}
		thresh += (threshL / colNumber);
		threshL = 0;
	}
	thresh = thresh/rowNumber;
	thresh = sqrt(200 * thresh);
	return thresh;
}

//利用索贝尔算子检测图片边缘
void PixImage::Sobel(int thresh, cv::Mat &imageSobel, cv::Mat &gray) {
	imageSobel = image.clone();
	int rowNumber = imageSobel.rows;
	int colNumber = imageSobel.cols;
	uchar **data = new uchar*[rowNumber];
	uchar **temp = new uchar*[rowNumber];
	
	for (int i = 0; i < rowNumber; i++) {
		data[i] = image.ptr<uchar>(i);
		temp[i] = imageSobel.ptr<uchar>(i);
	}

	for (int i = 1; i < rowNumber - 1; i++) {
		for (int j = 1; j < colNumber - 1; j++) {
			int Gx = (data[i + 1][3 * (j - 1)] + 2 * data[i + 1][j * 3] + data[i + 1][3 * (j + 1)])
				- (data[i - 1][3 * (j - 1)] + 2 * data[i - 1][j * 3] + data[i - 1][3 * (j + 1)]);
			int Gy = (data[i - 1][3 * (j - 1)] + 2 * data[i][3 * (j - 1)] + data[i + 1][3 * (j - 1)])
				- (data[i - 1][3 * (j + 1)] + 2 * data[i][3 * (j + 1)] + data[i + 1][3 * (j + 1)]);
			uchar G = sqrt(Gx*Gx + Gy*Gy);
			if (G > thresh)
				temp[i][j*3] = (uchar)255;
			else
				temp[i][j*3] = (uchar)0;
			temp[i][j * 3 + 1] = temp[i][j * 3];
			temp[i][j * 3 + 2] = temp[i][j * 3];
		}
	}
	delete[]data;
	delete[]temp;
}

//显示图片
void PixImage::PicShow() {
	int timeShow = 1000;
	if (imageCR.empty()) {
		cout << "没有进行像素减少处理" << endl;
	}
	else {
		namedWindow("PicShow");
		imshow("PicShow", imageCR);
		waitKey(timeShow);
	}
	if (imageBluring.empty()) {
		cout << "没有进行模糊处理" << endl;
	}
	else {
		namedWindow("PicShow");
		imshow("PicShow", imageBluring);
		waitKey(timeShow);
	}
	if (imageSobel.empty()) {
		cout << "没有进行图像边缘处理" << endl;
	}
	else {
		namedWindow("PicShow");
		imshow("PicShow", imageSobel);
		waitKey(timeShow);
	}
}

//保存图片
void PixImage::PicSave(string name) {
	if (imageCR.empty()) {
		cout << "没有进行像素减少处理" << endl;
	}
	else
		cv::imwrite(name + "_像素减少.jpg", imageCR);
	if (imageBluring.empty()) {
		cout << "没有进行模糊处理" << endl;
	}
	else
		cv::imwrite(name + "_模糊处理.jpg", imageBluring);
	if (imageSobel.empty()) {
		cout << "没有进行图像边缘处理" << endl;
	}
	else
		cv::imwrite(name + "_图像边缘处理.jpg", imageSobel);
}