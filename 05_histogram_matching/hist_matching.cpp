#include "hist_func.h"

// calculate tranfer function
void calc_trans(G *trans_func, float *CDF);
// histogram matching
void hist_matching(Mat &input, Mat &matched, G *trans_func_t, G *trans_func_g_inv);
// find inverse function
void inv(G *trans_func_g, G *trans_func_g_inv);

int main() {
	// read input image and reference image
	Mat input = imread("input.jpg", cv::IMREAD_COLOR);
	Mat ref = imread("ref.jpg", cv::IMREAD_COLOR);
	Mat input_gray, ref_gray;
	// convert RGB to Grayscale
	cvtColor(input, input_gray, COLOR_RGB2GRAY);	
	cvtColor(ref, ref_gray, COLOR_RGB2GRAY);

	Mat matched = input_gray.clone();
	

	// PDF or transfer function txt files
	FILE *f_PDF;
	FILE *f_matched_PDF_gray;
	//FILE *f_PDF_ref;
	// transfer functions 
	FILE *f_trans_func; // *f_trans_func_ref, *f_trans_func_ref_inv;

	fopen_s(&f_PDF, "PDF.txt", "w+");
	fopen_s(&f_matched_PDF_gray, "matched_PDF_gray.txt", "w+");
	//fopen_s(&f_PDF_ref, "PDF_ref.txt", "w+");
	fopen_s(&f_trans_func, "trans_func.txt", "w+");
	//fopen_s(&f_trans_func_ref, "trans_func_ref.txt", "w+");
	//fopen_s(&f_trans_func_ref_inv, "trans_func_ref_inv.txt", "w+");

	float *PDF = cal_PDF(input_gray);	// PDF of Input image(Grayscale) : [L]
	float *CDF = cal_CDF(input_gray);	// CDF of Input image(Grayscale) : [L]
	float *PDF_ref = cal_PDF(ref_gray);	// PDF of reference image(Grayscale) : [L]
	float *CDF_ref = cal_CDF(ref_gray);	// CDF of reference image(Grayscale) : [L]

	G trans_func[L] = { 0 };			// transfer function
	G trans_func_ref[L] = { 0 };		// transfer function of reference image
	G trans_func_ref_inv[L] = { 0 };		// inverse of trans_func_eq_ref

	calc_trans(trans_func, CDF);		// transfer function of input
	calc_trans(trans_func_ref, CDF_ref);	// trasfer function of reference

	// find inverse function of reference
	inv(trans_func_ref, trans_func_ref_inv);
	// histogram matching
	hist_matching(input_gray, matched, trans_func, trans_func_ref_inv);

	float *matched_PDF_gray = cal_PDF(matched);		// matched PDF (grayscale)

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF, "%d\t%f\n", i, PDF[i]);
		fprintf(f_matched_PDF_gray, "%d\t%f\n", i, matched_PDF_gray[i]);
		//fprintf(f_PDF_ref, "%d\t%f\n", i, PDF_ref[i]);

		// write transfer functions for debuging
		fprintf(f_trans_func, "%d\t%d\n", i, trans_func[i]);
		//fprintf(f_trans_func_ref, "%d\t%d\n", i, trans_func_ref[i]);
		//fprintf(f_trans_func_ref_inv, "%d\t%d\n", i, trans_func_ref_inv[i]);
	}

	// memory release
	free(PDF);
	free(CDF);
	fclose(f_PDF);
	fclose(f_matched_PDF_gray);
	fclose(f_trans_func);
	//fclose(f_trans_func_ref);
	//fclose(f_trans_func_ref_inv);
	////////////////////// Show each image ///////////////////////

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("Matched", WINDOW_AUTOSIZE);
	imshow("Matched", matched);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}
void calc_trans(G *trans_func, float *CDF) {
	// compute transfer function
	for (int i = 0; i < L; i++)
		trans_func[i] = (G)((L - 1) * CDF[i]);
}
// histogram matching
void hist_matching(Mat &input, Mat &matched, G *trans_func_t, G *trans_func_g_inv) {
	for (int i = 0;i < input.rows;i++) {
		for (int j = 0;j < input.cols;j++) {
			matched.at<G>(i, j) = trans_func_g_inv[trans_func_t[input.at<G>(i, j)]];
		}
	}
}
// find inverse function
void inv(G *trans_func_g, G *trans_func_g_inv) {
	for (int i = 0;i < L;i++) {
		trans_func_g_inv[trans_func_g[i]] = i;
	}
	// fill in rest of the blanks
	for (int i = 1;i < L;i++) {
		if (trans_func_g_inv[i] == 0) {
			int left = i;	// nearest value(!=0) on left
			int right = i;	// nearest value(!=0) on right
			while (trans_func_g_inv[left] == 0) {
				if (left == 0) {
					trans_func_g_inv[i] = 0;
					break;
				}
				left--;
			}
			while (trans_func_g_inv[right] == 0) {
				if (right == 255) {
					trans_func_g_inv[i] = 255;
					break;
				}
				right++;
			}
			// find trans_func_g_inv[i] by linear equation
			float tmp = ((float)trans_func_g_inv[left] * (right - i) + (float)trans_func_g_inv[right] * (i - left));
			trans_func_g_inv[i] = (int)(tmp/(right-left));
		}
	}
}

