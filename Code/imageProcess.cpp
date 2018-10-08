#include "ImageProcess.h"

//ͨ��.ptr<>�����õ�һ�е�ָ�룬����[]����������ĳһ�е�����ֵ��
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

/*��ͼ���е����ر������������糣����RGB24ͼ����256��256��256����ɫ��
ͨ��Reduce Color��ÿ��ͨ�������ؼ���8����256/8=32�֣���ͼ��ֻ��32��32��32����ɫ��
�����������ٵı�����N�������ʵ��ʱ���Ǽ򵥵�value/N*N��
ͨ�����ټ���N/2�Եõ����ڵ�N�ı������м�ֵ��
���ͼ������Ϊ(256/N)��(256/N)��(256/N)����ɫ��*/
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

//��ֵ�˲�����������
void PixImage::Bluring(cv::Mat &image, cv::Mat &imageBluring, int winSize) {
	imageBluring = image.clone();
	int rowNumber = imageBluring.rows;
	int colNumber = imageBluring.cols;
	uchar **data = new uchar*[rowNumber];
	int pos = (winSize - 1) / 2, temp = 0;

	//ģ�������м�һȦ
	for (int i = pos; i < rowNumber - pos; i++) {
		data[i] = imageBluring.ptr<uchar>(i);
		for (int j = pos; j < colNumber - pos; j++) {
			temp = 0;
			//�����ɫ����
			for (int winRow = 0; winRow < winSize; winRow++)
				for (int winCol = 0; winCol < winSize; winCol++)
					temp += colorR[i - pos + winRow][j - pos + winCol];
			data[i][j * 3] = temp / (winSize*winSize);
			temp = 0;
			//������ɫ����
			for (int winRow = 0; winRow < winSize; winRow++)
				for (int winCol = 0; winCol < winSize; winCol++)
					temp += colorG[i - pos + winRow][j - pos + winCol];
			data[i][j * 3 + 1] = temp / (winSize*winSize);
			temp = 0;
			//������ɫ����
			for (int winRow = 0; winRow < winSize; winRow++)
				for (int winCol = 0; winCol < winSize; winCol++)
					temp += colorB[i - pos + winRow][j - pos + winCol];
			data[i][j * 3 + 2] = temp / (winSize*winSize);
		}
	}
	data[0] = imageBluring.ptr<uchar>(0);
	data[rowNumber - 1] = imageBluring.ptr<uchar>(rowNumber - 1);

	//�������Ͻ�����
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

	//�������Ͻ�����
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

	//�������½�����
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

	//�������½�����
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

//��ͨ����ɫͼƬת���ɵ�ͨ��ͼƬ
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

//�������������Ӽ��ͼƬ��Ե
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

//��ʾͼƬ
void PixImage::PicShow() {
	int timeShow = 1000;
	if (imageCR.empty()) {
		cout << "û�н������ؼ��ٴ���" << endl;
	}
	else {
		namedWindow("PicShow");
		imshow("PicShow", imageCR);
		waitKey(timeShow);
	}
	if (imageBluring.empty()) {
		cout << "û�н���ģ������" << endl;
	}
	else {
		namedWindow("PicShow");
		imshow("PicShow", imageBluring);
		waitKey(timeShow);
	}
	if (imageSobel.empty()) {
		cout << "û�н���ͼ���Ե����" << endl;
	}
	else {
		namedWindow("PicShow");
		imshow("PicShow", imageSobel);
		waitKey(timeShow);
	}
}

//����ͼƬ
void PixImage::PicSave(string name) {
	if (imageCR.empty()) {
		cout << "û�н������ؼ��ٴ���" << endl;
	}
	else
		cv::imwrite(name + "_���ؼ���.jpg", imageCR);
	if (imageBluring.empty()) {
		cout << "û�н���ģ������" << endl;
	}
	else
		cv::imwrite(name + "_ģ������.jpg", imageBluring);
	if (imageSobel.empty()) {
		cout << "û�н���ͼ���Ե����" << endl;
	}
	else
		cv::imwrite(name + "_ͼ���Ե����.jpg", imageSobel);
}