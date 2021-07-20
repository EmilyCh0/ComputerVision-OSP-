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

Mat unsharpmask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k);
Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);
Mat meanfilter(const Mat input, int n, const char* opt);

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
	output = unsharpmask(input, 1, 1, 1, "mirroring", 0.5); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Unsharp Masking", WINDOW_AUTOSIZE);
	imshow("Unsharp Masking", output);

	waitKey(0);

	return 0;
}


Mat unsharpmask(const Mat input, int n, float sigmaT, float sigmaS, const char* opt, float k) {

	int row = input.rows;
	int col = input.cols;

	Mat output = Mat::zeros(row, col, input.type());
	Mat L = gaussianfilter(input, n, sigmaT, sigmaS, opt);
	//Mat L = meanfilter(input, n, opt);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			output.at<C>(i, j)[0] = (input.at<C>(i, j)[0] - k*L.at<C>(i, j)[0]) / (1 - k);
			output.at<C>(i, j)[1] = (input.at<C>(i, j)[1] - k*L.at<C>(i, j)[1]) / (1 - k);
			output.at<C>(i, j)[2] = (input.at<C>(i, j)[2] - k*L.at<C>(i, j)[2]) / (1 - k);
		}
	}
	return output;
}

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	float denom;
	float kernelvalue;

	// Initialiazing Kernel Matrix 
	kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);

	denom = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<float>(a + n, b + n) = value1;
			denom += value1;
		}
	}
	//normalization
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			kernel.at<float>(a + n, b + n) /= denom;
		}
	}

	Mat output = Mat::zeros(row, col, input.type());


	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-paddle")) {
				float sumR = 0.0;
				float sumG = 0.0;
				float sumB = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Gaussian filter with Zero-paddle boundary process:
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
							sumR += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(i + a, j + b)[0]);
							sumG += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(i + a, j + b)[1]);
							sumB += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(i + a, j + b)[2]);
						}
					}
				}
				output.at<C>(i, j)[0] = (G)sumR;
				output.at<C>(i, j)[1] = (G)sumG;
				output.at<C>(i, j)[2] = (G)sumB;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sumR = 0.0;
				float sumG = 0.0;
				float sumB = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Gaussian filter with "mirroring" process:
						if (i + a > row - 1) {  //mirroring for the border pixels
							tempa = i - a;	// bottom border
						}
						else if (i + a < 0) {
							tempa = -(i + a);	// top border
						}
						else {
							tempa = i + a;
						}
						if (j + b > col - 1) {
							tempb = j - b;	// right border
						}
						else if (j + b < 0) {
							tempb = -(j + b);	// left border
						}
						else {
							tempb = j + b;
						}
						sumR += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(tempa, tempb)[0]);
						sumG += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(tempa, tempb)[1]);
						sumB += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(tempa, tempb)[2]);
					}
				}
				output.at<C>(i, j)[0] = (G)sumR;
				output.at<C>(i, j)[1] = (G)sumG;
				output.at<C>(i, j)[2] = (G)sumB;
			}


			else if (!strcmp(opt, "adjustkernel")) {
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Gaussian filter with "adjustkernel" process:
						float sum1R = 0.0;
						float sum1G = 0.0;
						float sum1B = 0.0;
						float sum2 = 0.0;
						for (int a = -n; a <= n; a++) { // for each kernel window
							for (int b = -n; b <= n; b++) {
								if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
									sum1R += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(i + a, j + b)[0]);
									sum1G += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(i + a, j + b)[1]);
									sum1B += kernel.at<float>(a + n, b + n)*(float)(input.at<C>(i + a, j + b)[2]);
									sum2 += kernel.at<float>(a + n, b + n);
								}
							}
						}
						output.at<C>(i, j)[0] = (G)(sum1R / sum2);
						output.at<C>(i, j)[1] = (G)(sum1G / sum2);
						output.at<C>(i, j)[2] = (G)(sum1B / sum2);
					}
				}
			}
		}
	}
	return output;
}

Mat meanfilter(const Mat input, int n, const char* opt) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	// Initialiazing Kernel Matrix 
	kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size);
	float kernelvalue = kernel.at<float>(0, 0);

	Mat output = Mat::zeros(row, col, input.type());


	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-paddle")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
							sum1_r += kernelvalue*(float)(input.at<C>(i + a, j + b)[0]);
							sum1_g += kernelvalue*(float)(input.at<C>(i + a, j + b)[1]);
							sum1_b += kernelvalue*(float)(input.at<C>(i + a, j + b)[2]);
						}
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_r;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_b;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

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
						sum1_r += kernelvalue*(float)(input.at<C>(tempa, tempb)[0]);
						sum1_g += kernelvalue*(float)(input.at<C>(tempa, tempb)[1]);
						sum1_b += kernelvalue*(float)(input.at<C>(tempa, tempb)[2]);
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_r;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_b;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				float sum2 = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							sum1_r += kernelvalue*(float)(input.at<C>(i + a, j + b)[0]);
							sum1_g += kernelvalue*(float)(input.at<C>(i + a, j + b)[1]);
							sum1_b += kernelvalue*(float)(input.at<C>(i + a, j + b)[2]);
							sum2 += kernelvalue;
						}
					}
				}
				output.at<C>(i, j)[0] = (G)(sum1_r / sum2);
				output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
				output.at<C>(i, j)[2] = (G)(sum1_b / sum2);
			}
		}
	}
	return output;
}