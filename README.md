## Computer Vision ✏

### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 1. Image stitching  </mark> <br>

This code creates a blend stitched image from two input images. <br>



> **Note**
>
> Corresponding pixels of two images are given in advance.<br>Uses bilinear interpolation when inverse warping. <br>Change the value of 'alpha' (parameter of blend_stitching()) to change the ratio of I1 and I2.<br>Alpha must be float value between 0 and 1, and bigger alpha makes overlapping area closer to I1.



Stitched Result 👇

<img src="images/stitched.jpg" width="700"/>





### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 2. Compute PDF and CDF from grayscale image  </mark>

In order to compute PDF(Probability Density Function) and CDF(Cumulative Distribution Function), we need grayscale image. This code converts RGB input image into grayscale image. Then computes PDF and CDF from grayscale image. <br>
PDF and CDF are saved as .txt file after running the code. 



RGB to Grayscale result 👇<br>
    <img src="images/rgb2gray.png" alt="" width="500"/>





<center>PDF, CDF plot result 👇<br>
    <img src="images/pdfcdfplot.png" alt="" width="500"/></center>



### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 3. Histogram stretching  </mark>

This code creates higher contrast image by stretching histogram.

> **Note**
>
> Uses linear function for stretching.<br>To customize linear function, change x1, x2, y1, y2 values. These are parameters of linear_stretching() function.



Original grayscale image & stretched image 👇<br>
    <img src="images/histstretch.png" alt="" width="500"/>



PDF histogram 👇 (left: before stretching, right: after stretching)<br>
    <img src="images/histstretchplot.png" alt="" width="500"/>




### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 4. Histogram equalization  </mark>

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




### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 5. Histogram matching  </mark>

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



### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 6. Mean filter  </mark>

This code generate blurry image by applying mean filter. Adjusting boundary procedure precedes mean filtering. <br>

> **Note**
>
> There are three boundary processes in this code. Zero padding, Mirroring, Adjust kernel.<br>Change last parameter of meanfilter() function. Choose from [ zero-paddle / mirroring / adjustkernel ]



- Grayscale

  <img src="images/meangray.png" width="100%"/>



- RGB

  <img src="./images/meanrgb.png " width="100%"/>

Zero padding output images have dark boundary. This is because zero padding fill padding area with '0' resulting smaller mean value around boundary. 





### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 7. Gaussian filter  </mark>

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



### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 8. Sobel filter  </mark>

Sobel filtering extracts edge of the image. <br>Finding edge can be done by finding rapid change of the neighboring pixels.





- Grayscale 

<img src="./images/sobelgray.png" width="500"/>

- RGB

<img src="./images/sobelrgb.png" width="500"/>





### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 9. Laplacian filter  </mark>

Laplacian filtering is also high-pass filter that extracts edge of the image. <br>Finding edge can be done by finding rapid change of the neighboring pixels.

<img src="./images/laplacian.png" width="500"/>



### <mark style='background-color: #d0b8ff'>  </mark><mark style='background-color: #f5f0ff'> 10. Unsharp masking  </mark>

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























