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

Mat sobelfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat output;

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);
	output = sobelfilter(input);

	namedWindow("Sobel Filter", WINDOW_AUTOSIZE);
	imshow("Sobel Filter", output);


	waitKey(0);

	return 0;
}


Mat sobelfilter(const Mat input) {

	//Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Sobel Filter Kernel N

			   // Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	Mat Sx, Sy;
	Sx = Mat::zeros(3, 3, CV_32S);
	Sy = Mat::zeros(3, 3, CV_32S);
	//Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
	Sx.at<int>(0, 0) = -1; Sx.at<int>(1, 0) = -2; Sx.at<int>(2, 0) = -1;
	Sx.at<int>(0, 2) = 1; Sx.at<int>(1, 2) = 2; Sx.at<int>(2, 2) = 1;
	Sy.at<int>(0, 0) = -1; Sy.at<int>(0, 1) = -2; Sy.at<int>(0, 2) = -1;
	Sy.at<int>(2, 0) = 1; Sy.at<int>(2, 1) = 2; Sy.at<int>(2, 2) = 1;

	Mat output = Mat::zeros(row, col, input.type());

	int tempa, tempb;
	int sum_R, sum_G, sum_B;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float sumX_R = 0.0;
			float sumY_R = 0.0;
			float sumX_G = 0.0;
			float sumY_G= 0.0;
			float sumX_B = 0.0;
			float sumY_B = 0.0;

			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process
					// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy ) 
					// sobel filter with "mirroring" process:

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

					sumX_R += Sx.at<int>(a + 1, b + 1)*(float)input.at<C>(tempa, tempb)[0];
					sumY_R += Sy.at<int>(a + 1, b + 1)*(float)input.at<C>(tempa, tempb)[0];
					sumX_G += Sx.at<int>(a + 1, b + 1)*(float)input.at<C>(tempa, tempb)[1];
					sumY_G += Sy.at<int>(a + 1, b + 1)*(float)input.at<C>(tempa, tempb)[1];
					sumX_B += Sx.at<int>(a + 1, b + 1)*(float)input.at<C>(tempa, tempb)[2];
					sumY_B += Sy.at<int>(a + 1, b + 1)*(float)input.at<C>(tempa, tempb)[2];
				}
			}
			sum_R = sqrt(pow(sumX_R, 2.0) + pow(sumY_R, 2.0));
			sum_G = sqrt(pow(sumX_G, 2.0) + pow(sumY_G, 2.0));
			sum_B = sqrt(pow(sumX_B, 2.0) + pow(sumY_B, 2.0));
			//sum = abs(sumX) + abs(sumY);
			output.at<C>(i, j)[0] = (G)sum_R;
			output.at<C>(i, j)[1] = (G)sum_G;
			output.at<C>(i, j)[2] = (G)sum_B;
		}
	}
	return output;
}