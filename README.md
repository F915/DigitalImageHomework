<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=default"></script>

# Rock, Paper, Scissors

F915  
<https://github.com/F915/Rock-Paper-Scissors>  
2018.05.05

## What is it

a small thing you can play Rock-Paper-Scissors with it and you never win.

## How to run

1. It depends on [OpenCV 3.4](https://docs.opencv.org/3.4.3/df/d65/tutorial_table_of_content_introduction.html)(I built it in OpenCV 3.4), so make sure that you have installed the OpenCV Environment correctly.

1. Download this [code](https://raw.githubusercontent.com/F915/Rock-Paper-Scissors/master/main.cpp).
`wget https://raw.githubusercontent.com/F915/Rock-Paper-Scissors/master/main.cpp`

1. Now I think you have completed it, so just compile.
```g++ -std=c++11 main.cpp `pkg-config --cflags --libs opencv` ```

4. run it
`./a.out`

Windows is in a similar way.

## Principle

Why it works? I don't know. But it just works, yet.

1. some preprocessing(eg. gauss blur)
1. it divide the contours of the hand, and calculate the \\(square_c\\) and \\(length_c\\).
1. depend on contours, find the convex hull with its \\(square_h\\) and \\(length_h\\).
1. Normalize the index by $$i=\frac{length_c^2-length_h^2}{square_c-square_h}$$ 
1. decide the answer by \\(i\\) index.



## Update Logs

2018.05.05 Created project and tested algorithms.  
2018.05.06 Restructured codes.  
2018.05.07 Finished processFlame function.  
2018.05.08 Finished all functions.  
2018.10.29 Rewrite README and remove some redundancy.
