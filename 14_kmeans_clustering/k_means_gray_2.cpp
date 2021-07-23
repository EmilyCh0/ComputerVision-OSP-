#include <iostream>
#include <opencv2/opencv.hpp>

#define IM_TYPE	CV_8UC3

using namespace cv;


// Note that this code is for the case when an input data is a color value.
int main() {
	Mat rgbimg = imread("lena.jpg", IMREAD_COLOR);
	Mat input;

	cvtColor(rgbimg, input, COLOR_RGB2GRAY);
	Mat output;


	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);


	Mat samples(input.rows * input.cols, 3, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++) {
			samples.at<float>(y + x*input.rows, 0) = input.at<uchar>(y, x);
			samples.at<float>(y + x*input.rows, 1) = x/input.cols;
			samples.at<float>(y + x*input.rows, 2) = y/input.rows;
		}
	
			
	// Note that the intensity value is not normalized here (0~1). You should normalize both intensity and position when using them simultaneously.
	int clusterCount = 10;
	Mat labels;
	int attempts = 5;
	Mat centers;

	//kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	kmeans(samples, clusterCount, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(input.size(), input.type());

	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int idx = y + x*input.rows;
			int cluster_idx = labels.at<int>(idx);
			//Fill code that finds for each pixel of each channel of the output image the intensity of the cluster center.
			int tmp = centers.at<float>(cluster_idx, 0);
			new_image.at<uchar>(y, x) = (uchar)tmp;
		}
	imshow("clustered image", new_image);


	waitKey(0);

	return 0;
}

