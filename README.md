## Computer Vision ✏


### 1. Image stitching <br>
This code creates a blend stitched image from two input images. <br>

Notice<br>

> Corresponding pixels of two images are given in advance<br>Uses bilinear interpolation when inverse warping <br>Change the value of 'alpha' (parameter of blend_stitching()) to change the ratio of I1 and I2<br>Alpha must be float value between 0 and 1, and bigger alpha makes overlapping area closer to I1.



**Stitched Result** 👇 <br>
<img src="https://github.com/EmilyCh0/ComputerVision/blob/master/image/stitching_result.png" alt="" width="700"/>

### 2. Convert RGB to Grayscale & compute PDF and CDF from grayscale image
This converts RGB image into grayscale image. Then computes PDF(Probability Density Function) and CDF(Cumulative Distribution Function) from grayscale image. <br>
PDF and CDF are saved as .txt file after running the code. 

**RGB to Grayscale Result** 👇<br>
<img src="https://github.com/EmilyCh0/ComputerVision/blob/master/result/rgb2gray.png" alt="" width="500"/>

**PDF, CDF plot Result** 👇<br>
<img src="https://github.com/EmilyCh0/ComputerVision/blob/master/result/pdf_cdf_plot.png" alt="" width="500"/>
