#include "hist_func.h"

void hist_eq_Color(Mat &input, Mat &equalized, G(*trans_func)[3], float **CDF);

int main() {
	// read input image
	Mat input = imread("input.jpg", cv::IMREAD_COLOR);
	Mat equalized_RGB = input.clone();

	// PDF or transfer function txt files
	FILE *f_equalized_PDF_RGB, *f_PDF_RGB;
	FILE *f_trans_func_eq_R, *f_trans_func_eq_G, *f_trans_func_eq_B;

	fopen_s(&f_PDF_RGB, "PDF_RGB.txt", "w+");
	fopen_s(&f_equalized_PDF_RGB, "equalized_PDF_RGB.txt", "w+");
	fopen_s(&f_trans_func_eq_R, "trans_func_eq_R.txt", "w+");
	fopen_s(&f_trans_func_eq_G, "trans_func_eq_G.txt", "w+");
	fopen_s(&f_trans_func_eq_B, "trans_func_eq_B.txt", "w+");

	float **PDF_RGB = cal_PDF_RGB(input);	// PDF of Input image(RGB) : [L][3]
	float **CDF_RGB = cal_CDF_RGB(input);	// CDF of Input image(RGB) : [L][3]

	G trans_func_eq_RGB[L][3] = { 0 };		// transfer function

	hist_eq_Color(input, equalized_RGB, trans_func_eq_RGB, CDF_RGB);	// histogram equalization on RGB image
				

	float **equalized_PDF_RGB = cal_PDF_RGB(equalized_RGB);				// equalized PDF (RGB)
										

	for (int i = 0; i < L; i++) {
		// write PDF
		// ...
		fprintf(f_PDF_RGB, "%d\t%f\n", i, PDF_RGB[i][0]);
		fprintf(f_PDF_RGB, "%d\t%f\n", i, PDF_RGB[i][1]);
		fprintf(f_PDF_RGB, "%d\t%f\n", i, PDF_RGB[i][2]);
		fprintf(f_equalized_PDF_RGB, "%d\t%f\n", i, equalized_PDF_RGB[i][0]);
		fprintf(f_equalized_PDF_RGB, "%d\t%f\n", i, equalized_PDF_RGB[i][1]);
		fprintf(f_equalized_PDF_RGB, "%d\t%f\n", i, equalized_PDF_RGB[i][2]);

		// write transfer functions
		// ...
		fprintf(f_trans_func_eq_R, "%d\t%d\n", i, trans_func_eq_RGB[i][0]);
		fprintf(f_trans_func_eq_G, "%d\t%d\n", i, trans_func_eq_RGB[i][1]);
		fprintf(f_trans_func_eq_B, "%d\t%d\n", i, trans_func_eq_RGB[i][2]);
	}

	// memory release
	free(PDF_RGB);
	free(CDF_RGB);
	fclose(f_PDF_RGB);
	fclose(f_equalized_PDF_RGB);
	fclose(f_trans_func_eq_R);
	fclose(f_trans_func_eq_G);
	fclose(f_trans_func_eq_B);

	////////////////////// Show each image ///////////////////////

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Equalized_RGB", WINDOW_AUTOSIZE);
	imshow("Equalized_RGB", equalized_RGB);

	//////////////////////////////////////////////////////////////

	waitKey(0);

	return 0;
}

// histogram equalization on 3 channel image
void hist_eq_Color(Mat &input, Mat &equalized, G(*trans_func)[3], float **CDF) {

	for (int i = 0;i < L;i++) {
		trans_func[i][0] = (G)((L - 1)*CDF[i][0]);	// transfer function channel R
		trans_func[i][1] = (G)((L - 1)*CDF[i][1]);	// transfer function channel G
		trans_func[i][2] = (G)((L - 1)*CDF[i][2]);	// transfer function channel B
	}
	for (int i = 0;i < input.rows;i++) {
		for (int j = 0;j < input.cols;j++) {
			// put transfered result to equalized outcome image on each channel
			equalized.at<Vec3b>(i, j)[0] = trans_func[input.at<Vec3b>(i, j)[0]][0];
			equalized.at<Vec3b>(i, j)[1] = trans_func[input.at<Vec3b>(i, j)[1]][1];
			equalized.at<Vec3b>(i, j)[2] = trans_func[input.at<Vec3b>(i, j)[2]][2];
		}
	}
}