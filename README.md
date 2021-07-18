## Computer Vision ✏

<br>



1. [Image Stitching](#-1-image-stitching)

2. [PDF and CDF](#---2-compute-pdf-and-cdf-from-grayscale-image)

3. [Histogram Stretching](#-3-histogram-stretching)

4. [Histogram Equalization](#-4-histogram-equalization)

5. [Histogram Matching](#-5-histogram-matching)

6. [Mean Filter](#-6-mean-filter)

7. [Gaussian Filter](#-7-gaussian-filter)

8. [Sobel Filter](#-8-sobel-filter)

9. [Laplacian Filter](#-9-laplacian-filter)

10. [Unsharp Masking](#-10-unsharp-masking)

11. [Salt and Pepper Noise (generation/removal)](#-11-salt-and-pepper-noise-generationremoval)

12. [Gaussian Noise (generation/removal)](#-12-gaussian-noise-generationremoval)

13. [Adaptive Thresholding](#-13-adaptive-thresholding)

14. [K means Clustering](#-14-k-means-clustering)

    [Mean Shift](#--mean-shift)

15. 

### 📌 1. Image stitching 

This code creates a blend stitched image from two input images. <br>



> **Note**
>
> Corresponding pixels of two images are given in advance.<br>Uses bilinear interpolation when inverse warping. <br>Change the value of 'alpha' (parameter of blend_stitching()) to change the ratio of I1 and I2.<br>Alpha must be float value between 0 and 1, and bigger alpha makes overlapping area closer to I1.



Stitched Result 👇

<img src="images/stitched.jpg" width="700"/>





### <mark style='background-color: #d0b8ff'>  </mark>📌 2. Compute PDF and CDF from grayscale image

In order to compute PDF(Probability Density Function) and CDF(Cumulative Distribution Function), we need grayscale image. This code converts RGB input image into grayscale image. Then computes PDF and CDF from grayscale image. <br>
PDF and CDF are saved as .txt file after running the code. 



RGB to Grayscale result 👇<br>
    <img src="images/rgb2gray.png" alt="" width="500"/>





<center>PDF, CDF plot result 👇<br>
    <img src="images/pdfcdfplot.png" alt="" width="500"/></center>




### 📌 3. Histogram stretching

This code creates higher contrast image by stretching histogram.

> **Note**
>
> Uses linear function for stretching.<br>To customize linear function, change x1, x2, y1, y2 values. These are parameters of linear_stretching() function.



Original grayscale image & stretched image 👇<br>
    <img src="images/histstretch.png" alt="" width="500"/>



PDF histogram 👇 (left: before stretching, right: after stretching)<br>
    <img src="images/histstretchplot.png" alt="" width="500"/>




### 📌 4. Histogram equalization

This code creates higher contrast image by histogram equalization.<br>Histogram equalization is similar to histogram stretching in the way that it results to higher contrast outcome. However, unlike histogram stretching, histogram equalization is fully automatic. In other words, setting parameters for linear equation is not needed.



- **Grayscale**

  Grayscale - histogram equalized result 👇 <br>
      <img src="images/histeq.png" alt="" width="500"/>

  PDF histogram 👇<br>
      <img src="images/histeqplot.png" alt="" width="500"/>




- **RGB**

  What would be the result if you apply histogram equalization on each channel of RGB color image? Would it be higher contrast color image? No, It results to color distortion. 

  
  
  Histogram equalization on RGB channels 👇<br>
      <img src="images/histeqrgb.png" alt="" width="500"/>
  
  

- **YUV**

  To avoid color distortion, apply histogram equalization on Y channel. Y channel has intensity values of light on the image. By applying histogram equalization, you get higher contrast color image.

  
  
  Histogram equalization on YUV 👇<br>
      <img src="images/histeqyuv.png" alt="" width="500"/>




### 📌 5. Histogram matching

Histogram matching creates result image with reference image or reference histogram. Different reference, different result.

- **Grayscale**

  <img src="images/ref1.jpg" width="250"/> 👈 reference 1

  <img src="images/ref1result.png" width="500"/> 👈 Histogram matched result

  With bright, low contrast reference image, the intensity values of original image became similar to the reference.

  

  

   <img src="images/ref2.jpg" width="250"/>👈 reference 2

  <img src="images/ref2result.png" width="500"/> 👈 Histogram matched result



- **YUV**

  For color image, histogram matching is applied on Y channel. 

  <img src="images/matchingYUV.png" width="500"/>

  This is the outcome using reference 1.



### 📌 6. Mean filter

This code generate blurry image by applying mean filter. Adjusting boundary procedure precedes mean filtering. <br>

> **Note**
>
> There are three boundary processes in this code. Zero padding, Mirroring, Adjust kernel.<br>Change last parameter of meanfilter() function. Choose from [ zero-paddle / mirroring / adjustkernel ]



- Grayscale

  <img src="images/meangray.png" width="100%"/>



- RGB

  <img src="./images/meanrgb.png " width="100%"/>

Zero padding output images have dark boundary. This is because zero padding fill padding area with '0' resulting smaller mean value around boundary. 





### 📌 7. Gaussian filter

Unlike mean filtering, Gaussian filtering consider spatial distance. Resulting more natural output image.<br>

Another noticeable characteristic of Gaussian filtering is that it is separable. This is important because output is completely same but run time is shorter.

> Note<br>
>
> Change boundary process by adjusting parameter of gaussianfilter() function. <br>Choose from [ zero-paddle / mirroring / adjustkernel ].<br>
>
> There are 4 cpp codes, Gaussian filtering on grayscale image, Gaussian filtering on RGB image, Gaussian filtering on grayscale image with separable manner and Gaussian filtering on RGB image with separable manner.





<img src="./images/gaussgray.png" width="100%"/>



<img src="./images/meangauss.png" width="500"/>

With small kernel size, mean filtering result and Gaussian filtering result seems similar. However with bigger kernel size, you can see that Gaussian filtering is much natural than mean filtering.



### 📌 8. Sobel filter

Sobel filtering extracts edge of the image. <br>Finding edge can be done by finding rapid change of the neighboring pixels.





- Grayscale 

<img src="./images/sobelgray.png" width="500"/>

- RGB

<img src="./images/sobelrgb.png" width="500"/>





### 📌 9. Laplacian filter

Laplacian filtering is also high-pass filter that extracts edge of the image. <br>Finding edge can be done by finding rapid change of the neighboring pixels.

<img src="./images/laplacian.png" width="500"/>



### 📌 10. Unsharp masking

Unsharp masking create more vivid image by removing low frequency.<br>

Find low frequency by applying low-pass filtering such as mean filter or Gaussian filter. Then, by subtracting low-pass output, only high frequency is left on the image, resulting vivid image. 

> Note<br>
>
> Change parameter of unsharpmask() function. <br>Choose boundary process from [ zero-paddle / mirroring / adjustkernel ].<br>Last parameter is k value which decides strength of unsharp masking. 

- Grayscale

  <img src="./images/unsharpgray.png" width="500"/>

  ☝ Gaussian filter, n=1, sigma=1, boundary process=mirroring, k=0.5

  

- RGB

  <img src="./images/unsharprgb.png" width="500"/>

  ☝ Gaussian filter, n=1, sigma=1, boundary process=mirroring, k=0.5

You can see that the result of unsharp masking on RGB channel is quite different from expected result. This probably happened because we did not consider intensity difference of each channel. 



### 📌 11. Salt and pepper noise (Generation/Removal)

This code is to generate salt-and-pepper noise on random pixels of the image, then remove salt-and-pepper noise. Noise are randomly scattered on image. Salt is white(255) noise pixels and pepper is black(0) noise pixels. <br>Removing salt-and-pepper is done by median filtering.



> **Note**
>
> To change density of noise, change parameters of Add_salt_pepper_Noise() function.<br>2nd parameter is density of salt noise, 3rd parameter is density of pepper noise. Both value must be value between 0~1.<br>By default, both ps and pp is 0.1.<br>To change kernel size, change window_radius value on line 49.<br>Choose boundary process. Choose from 'zero-padding','mirroring','adjustkernel' on last parameter.



- **Noise generation**

<img src="./images/saltpepper.png" width="500"/>

👆 Salt and pepper noise generation on grayscale image and rgb color image.

Salt and pepper noise on color image is not white and black because I applied noise independently on each channel. To generate white and black noise on color image, try modifying code to add salt and pepper noise on same pixel on all three channel.



- **Noise removal - median filtering**

<img src="./images/saltpepperdenoise.png" width="100%"/>

👆 Noise image and Denoised images with different boundary processes



<img src="./images/saltpepperrgb.png" width="100%"/>

👆 Noise image and Denoised images with different boundary processes



### 📌 12. Gaussian noise (Generation/Removal)

This code generate Gaussian noise on input image. <br>Then removes Gaussian noise by Gaussian filtering and Bilateral filtering.<br>Bilateral filtering considers not only the distance but also intensity difference or color difference between neighboring pixels. <br>This prevents blurry edge of Gaussian filtering.

> **Note**
>
> To change density of noise, change parameter of Add_Gaussian_Noise() function.<br>To change kernel size, change second parameter of Gaussian filter function on line 53, 54.<br>To change kernel size, change second parameter of bilateral filter function on line 53, 54.<br>You can also decide boundary process by changing last parameter. 
> Choose one from 'zero-padding', 'mirroring', 'adjustkernel' on last parameter of Gaussian filter function.



- **Noise generation**

  <img src="./images/gaussnoisegray.png" width="500"/>

  <img src="./images/gaussnoisergb.png" width="500"/>

- **Noise removal - Gaussian filtering**

  kernel_size=7, sigma_t=10, sigma_s=10

  <img src="./images/gaussnoisegauss.png" width="100%"/>

  <img src="./images/gaussnoisegauss2.png" width="100%"/>

- **Noise removal - Bilateral filtering**

  <img src="./images/bilateral.png" width="100%"/>



### Image Segmentation

Image segmentation is dividing an image into non-overlapping region with similar information.

### 📌 13. Adaptive thresholding

Adaptive thresholding compute local threshold of each pixel. Using local threshold prevents contamination on shadow area that happens on global thresholding. 

> **Note**
>
> This code uses uniform mean filter and zero-padding.<br>
>
> To change kernel size, change second parameter of adaptive_thres() function.<br>
> To change bnumber(constant value multiplied to sum), change last parameter of adaptive_thres() function.



👇 Grayscale image and the output. 

<img src="./images/adaptivethres.png" width="500"/>

👇 n = 2, bnumber = 0.7 	 **/**  	n = 2, bnumber = 0.9 👆

<img src="./images/adaptivethres2.png" width="250"/>



### 📌 14. K means clustering

If we know representative intensity, we can label all the pixels. If we know all the label of each pixel, we can find representative intensity with average. However the problem is that we don’t know both of them. The goal of K means clustering is to find k number of cluster centers by following process.

1. Randomly initialize cluster centers.
2. Classify points by finding which cluster center is the closest. 
3. Update cluster center with points that are classified to the specific cluster. 
4. With updated cluster center, classify points again. 

By repeating these procedures above, you get the final result. 

> **Notes**
>
> This code uses kmeans function from opencv library.<br>
>
> Choose the number of clusters by changing clusterCount value. Default value is 10.<br>Choose the number of k means process by changing attemps value. Default value is 5.



- Grayscale

  <img src="./images/kmeangray.png" width="500"/>



- RGB

  <img src="./images/kmeanrgb.png" width="750"/>



K means clustering also has weaknesses. If random cluster center is badly selected at first, result can have problems. Also, k-mean clustering is sensitive to outliers and the k should be defined manually. In addition it results to spherical clusters which can be different from expected result, because it calculate distance by Euclidean.



### 📌 + Mean shift

Mean shift algorithm seeks modes or local maxima of density in the feature space. K had to be manually defined in K-means clustering but by mean shift doesn't. With random seeds, first calculate mean of a search window. Then shift search window and repeat these procedures. When the mean value doesn’t change anymore, that point is the mode. No matter how many random seeds, final mean value always converge to modes. 

> **Note**
>
> There is opencv library function of mean shift clustering but it doesn't work well. <br>
>
> So I used a program provided by "EDISON". So, there is no cpp code file for mean shift.

<img src="./images/meanshift.png" width="750"/>

You can change input parameters on the left of this program.



















































