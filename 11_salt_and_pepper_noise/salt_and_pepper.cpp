#include <opencv2/opencv.hpp>
#include <stdio.h>

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

Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp);
Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt);
Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char *opt);

int main() {

	Mat input = imread("lena.jpg", IMREAD_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, COLOR_RGB2GRAY);	// convert RGB to Grayscale

	// Add noise to original image
	Mat noise_Gray = Add_salt_pepper_Noise(input_gray, 0.1f, 0.1f);
	Mat noise_RGB = Add_salt_pepper_Noise(input, 0.1f, 0.1f);

	// Denoise, using median filter
	int window_radius = 2;
	Mat Denoised_Gray = Salt_pepper_noise_removal_Gray(noise_Gray, window_radius, "adjustkernel");
	Mat Denoised_RGB = Salt_pepper_noise_removal_RGB(noise_RGB, window_radius, "adjustkernel");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Impulse Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Impulse Noise (Grayscale)", noise_Gray);

	namedWindow("Impulse Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Impulse Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp)
{
	Mat output = input.clone();
	RNG rng;
	// amount of pepper noise
	int amount1 = (int)(output.rows * output.cols * pp);
	// amount of salt noise
	int amount2 = (int)(output.rows * output.cols * ps);

	int x, y;

	// Grayscale image
	if (output.channels() == 1) {
		// add pepper noise on random pixels
		for (int counter = 0; counter < amount1; ++counter)
			output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 0;
		// add salt noise on random pixels
		for (int counter = 0; counter < amount2; ++counter)
			output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 255;
	}
	// Color image	
	else if (output.channels() == 3) {
		for (int counter = 0; counter < amount1; ++counter) {
			// add pepper noise on random pixels on each channel
			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[0] = 0;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[1] = 0;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[2] = 0;
		}

		for (int counter = 0; counter < amount2; ++counter) {
			// add salt noise on random pixels on each channel
			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[0] = 255;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[1] = 255;

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[2] = 255;
		}
	}

	return output;
}

Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int median;		// index of median value
	int tempa;
	int tempb;

	// initialize median filter kernel
	Mat kernel = Mat::zeros(kernel_size * kernel_size, 1, input.type());

	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!strcmp(opt, "zero-padding")) {
				//Median filter with "zero-padding" boundary process:
				kernel.setTo(Scalar::all(0));
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) { 
							kernel.at<G>((x+n)*kernel_size+(y+n), 0) = input.at<G>(i + x, j + y);
						}
					}
				}
				median = (kernel_size*kernel_size + 1) / 2 - 1;
			}
			else if (!strcmp(opt, "mirroring")) {
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						// Median filter with "mirroring" boundary process:
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
								kernel.at<G>((a + n)*kernel_size + (b + n), 0) = input.at<G>(tempa, tempb);
							}
						}
						median = (kernel_size*kernel_size + 1) / 2 - 1;
					}
				}
			}
			else if (!strcmp(opt, "adjustkernel")) {
				kernel.setTo(Scalar::all(0));
				int cnt = 0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						// Median filter with "adjustkernel" boundary process:
						
						if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) {
							kernel.at<G>((x + n)*kernel_size + (y + n), 0) = input.at<G>(i + x, j + y);
							cnt++;
						}
						median = ((cnt + 1) / 2)+kernel_size*kernel_size-cnt-1;
					}
				}
			}

			// Sort the kernels in ascending order
			sort(kernel, kernel, SORT_EVERY_COLUMN + SORT_ASCENDING);
			
			output.at<G>(i, j) = kernel.at<G>(median, 0);
			
			}
		}

		return output;
	}
	
	Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char *opt) {

		int row = input.rows;
		int col = input.cols;
		int kernel_size = (2 * n + 1);
		int median;		// index of median value
		int channel = input.channels();
		int tempa;
		int tempb;

		// initialize ( (TypeX with 3 channel) - (TypeX with 1 channel) = 16 )
		// ex) CV_8UC3 - CV_8U = 16
		Mat kernel = Mat::zeros(kernel_size * kernel_size, channel, input.type() - 16);

		Mat output = Mat::zeros(row, col, input.type());

		// convolution
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {

				if (!strcmp(opt, "zero-padding")) {
					kernel.setTo(Scalar::all(0));
					for (int a = -n; a <= n; a++) { // for each kernel window
						for (int b = -n; b <= n; b++) {
							if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
								kernel.at<C>((a + n)*kernel_size + (b + n), 0)[0] = input.at<C>(i + a, j + b)[0];
								kernel.at<C>((a + n)*kernel_size + (b + n), 0)[1] = input.at<C>(i + a, j + b)[1];
								kernel.at<C>((a + n)*kernel_size + (b + n), 0)[2] = input.at<C>(i + a, j + b)[2];
							}
						}
					}
					median = (kernel_size*kernel_size + 1) / 2 - 1;
				}

				else if (!strcmp(opt, "mirroring")) {

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
							kernel.at<C>((a + n)*kernel_size + (b + n), 0)[0] = input.at<C>(tempa, tempb)[0];
							kernel.at<C>((a + n)*kernel_size + (b + n), 0)[1] = input.at<C>(tempa, tempb)[1];
							kernel.at<C>((a + n)*kernel_size + (b + n), 0)[2] = input.at<C>(tempa, tempb)[2];
						}
					}
					median = (kernel_size*kernel_size + 1) / 2 - 1;
				}
				else if (!strcmp(opt, "adjustkernel")) {
					kernel.setTo(Scalar::all(0));
					int cnt = 0;
					for (int x = -n; x <= n; x++) { // for each kernel window
						for (int y = -n; y <= n; y++) {
							// Median filter with "adjustkernel" boundary process:
							
							if ((i + x <= row - 1) && (i + x >= 0) && (j + y <= col - 1) && (j + y >= 0)) { //if the pixel is not a border pixel
								kernel.at<C>((x + n)*kernel_size + (y + n), 0)[0] = input.at<C>(i + x, j + y)[0];
								kernel.at<C>((x + n)*kernel_size + (y + n), 0)[1] = input.at<C>(i + x, j + y)[1];
								kernel.at<C>((x + n)*kernel_size + (y + n), 0)[2] = input.at<C>(i + x, j + y)[2];
								cnt++;
							}
							median = ((cnt + 1) / 2) + kernel_size*kernel_size - cnt - 1;
						}
					}
				}

				// Sort the kernels in ascending order
				//sort(kernel, kernel, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
				sort(kernel, kernel, SORT_EVERY_COLUMN + SORT_ASCENDING);
				output.at<C>(i, j)[0] = kernel.at<C>(median, 0)[0];
				output.at<C>(i, j)[1] = kernel.at<C>(median, 0)[1];
				output.at<C>(i, j)[2] = kernel.at<C>(median, 0)[2];
				}
			}

			return output;
		}

	