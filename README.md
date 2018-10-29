# Rock, Paper, Scissors

## Digital Images Process

F915  
<https://github.com/F915/Rock-Paper-Scissors>  
2018.05.05

## What is it

a small thing you can play Rock-Paper-Scissors with it and you never win.

## How to run

It depends on OpenCV 3(I built it in OpenCV3.4), so make sure that you have installed the OpenCV Environment correctly.

[OpenCV wiki](https://docs.opencv.org/3.4.3/df/d65/tutorial_table_of_content_introduction.html)

Now I think you have completed it, so just compile.

```g++ -std=c++11 main.cpp `pkg-config --cflags --libs opencv` ```

and run it

`./a.out`

Windows is in a similar way.

## Principle

Why it works? I don't know. But it just works, yet.

1. some preprocessing(eg. gauss blur)
1. it divide the contours of the hand, and calculate the square and length.
1. depend on contours, find the convex hull with its square and length.
1. Normalize the index by $$i=\frac{length_contour^2-length_hull^2}{square_contour-square_hull}$$
1. decide the answer by index.



## Update Logs

2018.05.05 Created project and tested algorithms.  
2018.05.06 Restructured codes.  
2018.05.07 Finished processFlame function.  
2018.05.08 Finished all functions.  
2018.10.29 Rewrite README and remove some redundancy.
