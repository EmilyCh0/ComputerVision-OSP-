## Computer Vision ✏


### 1. Image stitching <br>
This code creates a blend stitched image from two input images. <br>



> **Note**
>
> Corresponding pixels of two images are given in advance.<br>Uses bilinear interpolation when inverse warping. <br>Change the value of 'alpha' (parameter of blend_stitching()) to change the ratio of I1 and I2.<br>Alpha must be float value between 0 and 1, and bigger alpha makes overlapping area closer to I1.



<center>Stitched Result 👇</center> 

<img src="images/stitched.jpg" width="700"/>





### 2. Compute PDF and CDF from grayscale image

In order to compute PDF(Probability Density Function) and CDF(Cumulative Distribution Function), we need grayscale image. <br>This code converts RGB input image into grayscale image. Then computes PDF and CDF from grayscale image. <br>
PDF and CDF are saved as .txt file after running the code. 



<center>RGB to Grayscale result 👇<br>
    <img src="images/rgb2gray.png" alt="" width="500"/></center>




<center>PDF, CDF plot result 👇<br>
    <img src="images/pdfcdfplot.png" alt="" width="500"/></center>



### 3. Histogram stretching

This code creates higher contrast image by stretching histogram.

> **Note**
>
> Uses linear function for stretching.<br>To customize linear function, change x1, x2, y1, y2 values. These are parameters of linear_stretching() function.



<center>Original grayscale image & stretched image 👇<br>
    <img src="images/histstretch.png" alt="" width="500"/></center>



<center>PDF histogram 👇 (left: before stretching, right: after stretching)<br>
    <img src="images/histstretchplot.png" alt="" width="500"/></center>



### 4. Histogram equalization

This code creates higher contrast image by histogram equalization.<br>Histogram equalization is similar to histogram stretching in the way that it results to higher contrast outcome. However, unlike histogram stretching, histogram equalization is fully automatic. In other words, setting parameters for linear equation is not needed.



- **Grayscale**

<center>Grayscale - histogram equalizated result 👇 <br>
    <img src="images/histeq.png" alt="" width="500"/></center>

 <center>PDF histogram 👇<br>
    <img src="images/histeqplot.png" alt="" width="500"/></center>



- **RGB**

  What would be the result if you apply histogram equalization on each channel of RGB color image? Would it be higher contrast color image? 

  

<center>Histogram equalization on RGB channels 👇<br>
    <img src="images/histeqrgb.png" alt="" width="500"/></center>

​		No, It results to color distortion. 



- **YUV**

  To avoid color distortion, apply histogram equalization on Y channel. Y channel has intensity values of light on the image. By applying histogram equalization, you get higher contrast color image.

  

<center>Histogram equalization on YUV 👇<br>
    <img src="images/histeqyuv.png" alt="" width="500"/></center>



### 5. Histogram matching

