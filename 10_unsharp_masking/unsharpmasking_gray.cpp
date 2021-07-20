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
	Mat output, input_gray;

	cvtColor(input, input_gray, COLOR_RGB2GRAY);

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);
	output = unsharpmask(input_gray,1,1,1,"mirroring", 0.5); //Boundary process: zero-paddle, mirroring, adjustkernel

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
			output.at<G>(i, j) = (input.at<G>(i, j) - k*L.at<G>(i, j))/(1-k);
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
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))));
			float value2 = exp(-(pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<float>(a + n, b + n) = value1*value2;
			denom += value1*value2;
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
				float sum = 0.0;
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Gaussian filter with Zero-paddle boundary process:
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
							sum += kernel.at<float>(a + n, b + n)*(float)(input.at<G>(i + a, j + b));

						}
					}
				}
				output.at<G>(i, j) = (G)sum;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum = 0.0;
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
						sum += kernel.at<float>(a + n, b + n)*(float)(input.at<G>(tempa, tempb));
					}
				}
				output.at<G>(i, j) = (G)sum;
			}


			else if (!strcmp(opt, "adjustkernel")) {
				for (int a = -n; a <= n; a++) {
					for (int b = -n; b <= n; b++) {
						// Gaussian filter with "adjustkernel" process:
						float sum1 = 0.0;
						float sum2 = 0.0;
						for (int a = -n; a <= n; a++) { // for each kernel window
							for (int b = -n; b <= n; b++) {
								if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
									sum1 += kernel.at<float>(a + n, b + n)*(float)(input.at<G>(i + a, j + b));
									sum2 += kernel.at<float>(a + n, b + n);
								}
							}
						}
						output.at<G>(i, j) = (G)(sum1 / sum2);
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
	float kernelvalue = kernel.at<float>(0, 0);  // To simplify, as the filter is uniform. All elements of the kernel value are same.

	Mat output = Mat::zeros(row, col, input.type());


	for (int i = 0; i < row; i++) { //for each pixel in the output
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-paddle")) {
				float sum1 = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
							sum1 += kernelvalue*(float)(input.at<G>(i + a, j + b));
						}
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1 = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

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
						sum1 += kernelvalue*(float)(input.at<G>(tempa, tempb));
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1 = 0.0;
				float sum2 = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							sum1 += kernelvalue*(float)(input.at<G>(i + a, j + b));
							sum2 += kernelvalue;
						}
					}
				}
				output.at<G>(i, j) = (G)(sum1 / sum2);
			}
		}
	}
	return output;
}