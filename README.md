## Computer Vision ✏


### 1. Image stitching <br>
This code creates a blend stitched image from two input images. <br>



> **Note**
>
> Corresponding pixels of two images are given in advance.<br>Uses bilinear interpolation when inverse warping. <br>Change the value of 'alpha' (parameter of blend_stitching()) to change the ratio of I1 and I2.<br>Alpha must be float value between 0 and 1, and bigger alpha makes overlapping area closer to I1.



Stitched Result 👇

<img src="images/stitched.jpg" width="700"/>





### 2. Compute PDF and CDF from grayscale image

In order to compute PDF(Probability Density Function) and CDF(Cumulative Distribution Function), we need grayscale image. This code converts RGB input image into grayscale image. Then computes PDF and CDF from grayscale image. <br>
PDF and CDF are saved as .txt file after running the code. 



RGB to Grayscale result 👇<br>
    <img src="images/rgb2gray.png" alt="" width="500"/>





<center>PDF, CDF plot result 👇<br>
    <img src="images/pdfcdfplot.png" alt="" width="500"/></center>



### 3. Histogram stretching

This code creates higher contrast image by stretching histogram.

> **Note**
>
> Uses linear function for stretching.<br>To customize linear function, change x1, x2, y1, y2 values. These are parameters of linear_stretching() function.



Original grayscale image & stretched image 👇<br>
    <img src="images/histstretch.png" alt="" width="500"/>



PDF histogram 👇 (left: before stretching, right: after stretching)<br>
    <img src="images/histstretchplot.png" alt="" width="500"/>




### 4. Histogram equalization

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




### 5. Histogram matching

Histogram matching creates result image with reference image or reference histogram. Different reference, different result.

- **Grayscale**

  <img src="images/ref1.jpg" width="250"/> 👈 reference 1

  <img src="images/ref1result.png" width="500"/> 👈 Left: original / Right: Histogram matched result

  With bright, low contrast reference image, the intensity values of original image became similar to the reference.

  

  

   <img src="images/ref2.jpg" width="250"/>👈 reference 2

  <img src="images/ref2result.png" width="500"/> 👈 Left: original / Right: Histogram matched result



- **YUV**

  For color image, histogram matching is applied on Y channel. 

  <img src="images/matchingYUV.png" width="500"/>

  This is the outcome using reference 1.



