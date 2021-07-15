#include "hist_func.h"

// calculate tranfer function
void calc_trans(G *trans_func, float *CDF);
// histogram matching
void hist_matching(Mat &input, Mat &input_ch, G *trans_func_t, G *trans_func_g_inv);
// find inverse funtion
void inv(G *trans_func_g, G *trans_func_g_inv);

int main() {
	// read input image and reference image
	Mat input = imread("input.jpg", cv::IMREAD_COLOR);
	Mat ref = imread("ref.jpg", cv::IMREAD_COLOR);
	Mat input_YUV, ref_YUV;
	// convert RGB to YUV
	cvtColor(input, input_YUV, COLOR_RGB2YUV);
	cvtColor(ref, ref_YUV, COLOR_RGB2YUV);

	// split each channel(Y, U, V)
	Mat input_channels[3];
	split(input_YUV, input_channels);
	Mat input_Y = input_channels[0];
	// split each channel(Y, U, V)
	Mat ref_channels[3];
	split(ref_YUV, ref_channels);
	Mat ref_Y = ref_channels[0];

	// PDF txt files
	FILE *f_PDF;
	FILE *f_matched_PDF_YUV;
	// FILE *f_PDF_ref;
	// transfer functions 
	FILE *f_trans_func; //*f_trans_func_ref, *f_trans_func_ref_inv;

	fopen_s(&f_PDF, "PDF.txt", "w+");
	fopen_s(&f_matched_PDF_YUV, "matched_PDF_YUV.txt", "w+");
	//fopen_s(&f_PDF_ref, "PDF_ref.txt", "w+");
	fopen_s(&f_trans_func, "trans_func.txt", "w+");
	//fopen_s(&f_trans_func_ref, "trans_func_ref.txt", "w+");
	//fopen_s(&f_trans_func_ref_inv, "trans_func_ref_inv.txt", "w+");

	float **PDF = cal_PDF_RGB(input_YUV);	// PDF of Input image : [L][3]
	float *CDF = cal_CDF(input_Y);	// CDF of Input image Y channel: [L]
	float **PDF_ref = cal_PDF_RGB(ref_YUV);	// PDF of reference image : [L][3]
	float *CDF_ref = cal_CDF(ref_Y);	// CDF of reference image Y channel: [L]

	G trans_func[L] = { 0 };			// transfer function
	G trans_func_ref[L] = { 0 };		// transfer function of reference image
	G trans_func_ref_inv[L] = { 0 };	// inverse of trans_func_ref

	calc_trans(trans_func, CDF);		// transfer function of input
	calc_trans(trans_func_ref, CDF_ref);	// trasfer function of reference

	// find inverse function of reference
	inv(trans_func_ref, trans_func_ref_inv);

	// histogram matching
	hist_matching(input_Y, input_channels[0], trans_func, trans_func_ref_inv);
	// merge Y, U, V channel & convert YUV to RGB
	merge(input_channels, 3, input_YUV);	
	cvtColor(input_YUV, input_YUV, COLOR_YUV2RGB, 3);

	//merge(ref_channels, 3, ref_YUV);
	//cvtColor(input_YUV, ref_YUV, COLOR_YUV2RGB, 3);

	float **matched_PDF_YUV = cal_PDF_RGB(input_YUV);		

	for (int i = 0; i < L; i++) {
		// write PDF
		fprintf(f_PDF, "%d\t%f\n", i, PDF[i][0]);
		fprintf(f_PDF, "%d\t%f\n", i, PDF[i][1]);
		fprintf(f_PDF, "%d\t%f\n", i, PDF[i][2]);
		fprintf(f_matched_PDF_YUV, "%d\t%f\n", i, matched_PDF_YUV[i][0]);
		fprintf(f_matched_PDF_YUV, "%d\t%f\n", i, matched_PDF_YUV[i][1]);
		fprintf(f_matched_PDF_YUV, "%d\t%f\n", i, matched_PDF_YUV[i][2]);
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
	fclose(f_matched_PDF_YUV);
	fclose(f_trans_func);
	//fclose(f_trans_func_ref);
	//fclose(f_trans_func_ref_inv);
	////////////////////// Show each image ///////////////////////

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Matched", WINDOW_AUTOSIZE);
	imshow("Matched", input_YUV);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}
void calc_trans(G *trans_func, float *CDF) {
	// compute transfer function
	for (int i = 0; i < L; i++)
		trans_func[i] = (G)((L - 1) * CDF[i]);
}
// histogram matching using transfer function T() and G^-1()
void hist_matching(Mat &input, Mat &input_ch, G *trans_func_t, G *trans_func_g_inv){
	for (int i = 0;i < input.rows;i++) {
		for (int j = 0;j < input.cols;j++) {
			input_ch.at<G>(i, j) = trans_func_g_inv[trans_func_t[input.at<G>(i, j)]];
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
			trans_func_g_inv[i] = (int)(tmp / (right - left));
		}
	}
}

