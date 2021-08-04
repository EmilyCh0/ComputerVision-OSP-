#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#define RATIO_THR 0.4

using namespace std;
using namespace cv;

double euclidDistance(Mat& vec1, Mat& vec2);
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors);
int secondNearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int nn);
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold);

template <typename T>
Mat cal_affine(vector<Point2f>& srcPoints, vector<Point2f>& dstPoints);
void blend_stitching(const Mat I1, const Mat I2, Mat &I_f, int diff_x, int diff_y, float alpha);

int main() {

	Mat input1 = imread("input1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input2 = imread("input2.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input1_gray, input2_gray;

	if (!input1.data || !input2.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	//resize(input1, input1, Size(input1.cols / 2, input1.rows / 2));
	//resize(input2, input2, Size(input2.cols / 2, input2.rows / 2));

	cvtColor(input1, input1_gray, CV_RGB2GRAY);
	cvtColor(input2, input2_gray, CV_RGB2GRAY);


	FeatureDetector* detector = new SiftFeatureDetector(
		0,		// nFeatures
		4,		// nOctaveLayers
		0.04,	// contrastThreshold
		10,		// edgeThreshold
		1.6		// sigma
	);

	DescriptorExtractor* extractor = new SiftDescriptorExtractor();

	// Create a image for displaying mathing keypoints
	Size size = input2.size();
	Size sz = Size(size.width + input1_gray.size().width, max(size.height, input1_gray.size().height));
	Mat matchingImage = Mat::zeros(sz, CV_8UC3);

	input1.copyTo(matchingImage(Rect(size.width, 0, input1_gray.size().width, input1_gray.size().height)));
	input2.copyTo(matchingImage(Rect(0, 0, size.width, size.height)));

	// Compute keypoints and descriptor from the source image in advance
	vector<KeyPoint> keypoints1;
	Mat descriptors1;

	detector->detect(input1_gray, keypoints1);
	extractor->compute(input1_gray, keypoints1, descriptors1);
	printf("input1 : %d keypoints are found.\n", (int)keypoints1.size());

	vector<KeyPoint> keypoints2;
	Mat descriptors2;

	// Detect keypoints
	detector->detect(input2_gray, keypoints2);
	extractor->compute(input2_gray, keypoints2, descriptors2);

	printf("input2 : %zd keypoints are found.\n", keypoints2.size());

	for (int i = 0; i < keypoints1.size(); i++) {
		KeyPoint kp = keypoints1[i];
		kp.pt.x += size.width;
		circle(matchingImage, kp.pt, cvRound(kp.size*0.25), Scalar(255, 255, 0), 1, 8, 0);
	}

	for (int i = 0; i < keypoints2.size(); i++) {
		KeyPoint kp = keypoints2[i];
		circle(matchingImage, kp.pt, cvRound(kp.size*0.25), Scalar(255, 255, 0), 1, 8, 0);
	}

	// Find nearest neighbor pairs
	vector<Point2f> srcPoints;
	vector<Point2f> dstPoints;
	bool crossCheck = true;
	bool ratio_threshold = true;
	findPairs(keypoints2, descriptors2, keypoints1, descriptors1, srcPoints, dstPoints, crossCheck, ratio_threshold);
	printf("%zd keypoints are matched.\n", srcPoints.size());
	
	
	input1.convertTo(input1, CV_32FC3, 1.0 / 255);
	input2.convertTo(input2, CV_32FC3, 1.0 / 255);
	// height(row), width(col) of each image
	const float input1_row = (float)input1.rows;
	const float input1_col = (float)input1.cols;
	const float input2_row = (float)input2.rows;
	const float input2_col = (float)input2.cols;

	// calculate affine Matrix A12, A21
	
	Mat A12 = cal_affine<float>(dstPoints, srcPoints);
	Mat A21 = cal_affine<float>(srcPoints, dstPoints);

	// compute corners (p1, p2, p3, p4) using A21
	// these points are needed to find boundary of I_F
	// p1: (0,0)
	// p2: (row, 0)
	// p3: (row, col)
	// p4: (0, col)
	Point2f p1(A21.at<float>(0) * 0 + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * 0 + A21.at<float>(5));
	Point2f p2(A21.at<float>(0) * 0 + A21.at<float>(1) * input2_row + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * input2_row + A21.at<float>(5));
	Point2f p3(A21.at<float>(0) * input2_col + A21.at<float>(1) * input2_row + A21.at<float>(2), A21.at<float>(3) * input2_col + A21.at<float>(4) * input2_row + A21.at<float>(5));
	Point2f p4(A21.at<float>(0) * input2_col + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * input2_col + A21.at<float>(4) * 0 + A21.at<float>(5));

	// compute boundary for merged image(I_f)
	// bound_u <= 0
	// bound_b >= I1_row-1
	// bound_l <= 0
	// bound_b >= I1_col-1
	int bound_u = (int)round(min(0.0f, min(p1.y, p4.y)));
	int bound_b = (int)round(max(input1_row - 1, max(p2.y, p3.y)));
	int bound_l = (int)round(min(0.0f, min(p1.x, p2.x)));
	int bound_r = (int)round(max(input1_col - 1, max(p3.x, p4.x)));

	Mat I_f(bound_b - bound_u + 1, bound_r - bound_l + 1, CV_32FC3, Scalar(0));
	
	// inverse warping with NN
	for (int i = bound_u; i <= bound_b; i++) {
		for (int j = bound_l; j <= bound_r; j++) {
			float x = A12.at<float>(0) * j + A12.at<float>(1) * i + A12.at<float>(2) - bound_l;
			float y = A12.at<float>(3) * j + A12.at<float>(4) * i + A12.at<float>(5) - bound_u;
			
			float y1 = floor(y);
			float y2 = ceil(y);
			float x1 = floor(x);
			float x2 = ceil(x);

			//float mu = y - y1;
			//float lambda = x - x1;

			int nnx = (x - x1 < 1 - x + x1) ? (int)x1 : x2;
			int nny = (y - y1 < 1 - y + y1) ? (int)y1 : y2;

			if (x1 >= 0 && x2 < input2_col && y1 >= 0 && y2 < input2_row) {
				//I_f.at<Vec3f>(i - bound_u, j - bound_l) = lambda * (mu * input2.at<Vec3f>(y2, x2) + (1 - mu) * input2.at<Vec3f>(y1, x2)) + (1 - lambda) *(mu * input2.at<Vec3f>(y2, x1) + (1 - mu) * input2.at<Vec3f>(y1, x1));
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = input2.at<Vec3f>(nny, nnx);
			}
				
		}
	}
	

	blend_stitching(input1, input2, I_f, bound_l, bound_u, 0.5);
	I_f.convertTo(I_f, CV_8UC3, 255.0);

	//namedWindow("before");
	//imshow("before", I_f);
	// Display mathing image
	namedWindow("Matching");
	imshow("Matching", I_f);

	waitKey(0);

	return 0;
}

/**
* Calculate euclid distance
*/
double euclidDistance(Mat& vec1, Mat& vec2) {
	double sum = 0.0;
	int dim = vec1.cols;
	for (int i = 0; i < dim; i++) {
		//sum += (vec1.at<uchar>(0, i) - vec2.at<uchar>(0, i)) * (vec1.at<uchar>(0, i) - vec2.at<uchar>(0, i));		// assertion error
		sum += (vec1.at<float>(0, i) - vec2.at<float>(0, i)) * (vec1.at<float>(0, i) - vec2.at<float>(0, i));
	}

	return sqrt(sum);
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		Mat v = descriptors.row(i);		// each row of descriptor
										//
										//	Fill the code
		double dist = euclidDistance(vec, v);
		if (dist < minDist) {
			neighbor = i;
			minDist = dist;
		}
		//
	}
	return neighbor;
}
int secondNearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int nn) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		Mat v = descriptors.row(i);		// each row of descriptor

										//
										//	Fill the code
		double dist = euclidDistance(vec, v);
		if (dist < minDist&&dist&&i != nn) {
			neighbor = i;
			minDist = dist;
		}
		//
	}
	return neighbor;
}

/**
* Find pairs of points with the smallest distace between them
*/
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold) {
	for (int i = 0; i < descriptors1.rows; i++) {

		KeyPoint pt1 = keypoints1[i];
		Mat desc1 = descriptors1.row(i);

		int nn = nearestNeighbor(desc1, keypoints2, descriptors2);

		// Refine matching points using ratio_based thresholding
		if (ratio_threshold) {
			//
			//	Fill the code	
			int nn2 = secondNearestNeighbor(desc1, keypoints2, descriptors2, nn);
			float dist1 = euclidDistance(desc1, descriptors2.row(nn));
			float dist2 = euclidDistance(desc1, descriptors2.row(nn2));

			if ((dist1 / dist2) > 0.8) {
				continue;
			}
			//
		}

		// Refine matching points using cross-checking
		if (crossCheck) {
			//
			//	Fill the code
			KeyPoint tmp = keypoints2[nn];
			Mat desc2 = descriptors2.row(nn);
			int nn2 = nearestNeighbor(desc2, keypoints1, descriptors1);
			if (nn2 != i) {
				continue;
			}
			//
		}

		KeyPoint pt2 = keypoints2[nn];
		srcPoints.push_back(pt1.pt);
		dstPoints.push_back(pt2.pt);
	}
}

template <typename T>
Mat cal_affine(vector<Point2f>& srcPoints, vector<Point2f>& dstPoints) {

	Mat M(2 * (int)srcPoints.size(), 6, CV_32F, Scalar(0));
	Mat b(2 * (int)srcPoints.size(), 1, CV_32F);

	Mat M_trans, temp, affineM;

	// initialize matrix
	for (int i = 0; i < (int)srcPoints.size(); i++) {
		Point2f pt1 = srcPoints[i];
		Point2f pt2 = dstPoints[i];
		
		// fill out M(2Nx6 matrix) with corresponding pixels of I1
		M.at<T>(2 * i, 0) = pt1.x;		M.at<T>(2 * i, 1) = pt1.y;		M.at<T>(2 * i, 2) = 1;
		M.at<T>(2 * i + 1, 3) = pt1.x;		M.at<T>(2 * i + 1, 4) = pt1.y;		M.at<T>(2 * i + 1, 5) = 1;
		// fill out b(2Nx1 matrix) with correponding pixels of I2
		b.at<T>(2 * i) = pt2.x;		b.at<T>(2 * i + 1) = pt2.y;
	}

	// (M^T * M)^(−1) * M^T * b ( * : Matrix multiplication)
	transpose(M, M_trans);
	invert(M_trans * M, temp);
	affineM = temp * M_trans * b;

	return affineM;
}

void blend_stitching(const Mat I1, const Mat I2, Mat &I_f, int bound_l, int bound_u, float alpha) {

	int col = I_f.cols;
	int row = I_f.rows;

	// I2 is already in I_f by inverse warping
	// don't have to check area where only I2 is valid
	// so this loop check through I1 to find whether it is only I1 valid area or both valid area
	for (int i = 0; i < I1.rows; i++) {
		for (int j = 0; j < I1.cols; j++) {
			// validation of I2
			bool cond_I2 = I_f.at<Vec3f>(i - bound_u, j - bound_l) != Vec3f(0, 0, 0) ? true : false;
			if (cond_I2)  // both valid area
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = alpha * I1.at<Vec3f>(i, j) + (1 - alpha) * I_f.at<Vec3f>(i - bound_u, j - bound_l);
			else   // only I1 valid area
				I_f.at<Vec3f>(i - bound_u, j - bound_l) = I1.at<Vec3f>(i, j);

		}
	}
}
