#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE	CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat laplacianfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output, input_gray;

	cvtColor(input, input_gray, COLOR_RGB2GRAY);

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);
	output = laplacianfilter(input_gray); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Laplacian Filter", WINDOW_AUTOSIZE);
	imshow("Laplacian Filter", output);


	waitKey(0);

	return 0;
}


Mat laplacianfilter(const Mat input) {

	//Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Laplacian Filter Kernel N
		   
	Mat L;
	L = Mat::zeros(3, 3, CV_32S);
	
	L.at<int>(0, 1) = 1; L.at<int>(2, 1) = 1;
	L.at<int>(1, 0) = 1; L.at<int>(1, 1) = -4; L.at<int>(1, 2) = 1;
	

	Mat output = Mat::zeros(row, col, input.type());

	int tempa, tempb;
	float sum;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			sum = 0.0;

			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process
					
					// laplacian filter with "mirroring" process:

					if (i + a > row - 1) {  //mirroring for the border pixels
						tempa = i - a;
					}
					else if (i + a < 0) {
						tempa = -(i + a);
					}
					else {
						tempa = i + a;
					}
					if (j + b > col - 1) {
						tempb = j - b;
					}
					else if (j + b < 0) {
						tempb = -(j + b);
					}
					else {
						tempb = j + b;
					}
					sum += L.at<int>(a + 1, b + 1)*(float)input.at<G>(tempa, tempb);			
				}
			}
			sum = abs(sum)*4;
			output.at<G>(i, j) = (G)sum;
		}
	}
	return output;
}