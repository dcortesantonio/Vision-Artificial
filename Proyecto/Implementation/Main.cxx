#include "opencv2/highgui/highgui.hpp"
// highgui - an interface to video and image capturing.

#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
// The header files for performing input and output.
 
using namespace cv;
// Namespace where all the C++ OpenCV functionality resides.

using namespace std;
// For input output operations.
 

int main(int argc, char** argv )
{
      // Get command line arguments
  if ( argc < 2 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  
  Mat src, img_gray, imgBlu,imgCanny,imgThreshold,bw,dist;

  vector< vector<Point> > contours; // List of contour points (External)
	vector<Vec4i> hierarchy;
  //ReAd the image
	src = imread( argv[1], IMREAD_COLOR);
  stringstream ss( argv[ 1 ] );
  string basename;
  getline( ss, basename, '.' );
  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  }
  
	//Convert image to gray
	cvtColor(src, img_gray, cv::COLOR_BGR2GRAY);

  //Apply Gaussian Blur to grayscaled image.
   GaussianBlur(img_gray,            
        imgBlu,                            
        cv::Size(5, 5), //Kernel size                      
        1.5);                                

    Canny(imgBlu,            
        imgCanny,                    
        100,                        // low threshold
        200);                        // high threshold

 // threshold(imgBlu, bw, 100, 255, THRESH_BINARY | THRESH_OTSU);
/*
  Mat M = Mat::ones(3, 3, CV_8U);
  dilate(bw, bw, M);



	//imwrite("BIa.png", bw);
	
	// Perform the distance transform algorithm
  distanceTransform(bw, dist, CV_DIST_L2, 5);

  // Normalize the distance image for range = {0.0, 1.0} so we can visualize and threshold it
  normalize(dist, dist, 0, 1, NORM_MINMAX);
  //imwrite("DTI.png", dist);

  // Threshold to obtain the peaks, this will be the markers for the foreground objects
  threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);

   // imwrite("the.png", dist);

	// Dilate a bit the dist image
  Mat kernel1 = Mat::ones(3, 3, CV_8U);
  dilate(dist, dist, kernel1);
  //imwrite("Peaks.png", dist);

  // Create the CV_8U version of the distance image, it is needed for findContours()
  Mat dist_8u;
  dist.convertTo(dist_8u, CV_8U);
	*/
    //imwrite( basename + "GaussianBlur.png", imgBlu );
    //imwrite( basename + "__AllBorders.png", imgCanny );   
    
    //Binary Thresholding
    threshold( imgBlu, 
        imgThreshold, 
        100,   // low threshold
        255,   // high threshold
        THRESH_BINARY);
      
    imwrite( basename + "_Binary.png", imgThreshold );

     //Find contours of the image.(External)
    findContours(imgThreshold, 
        contours, 
        hierarchy, 
        CV_RETR_EXTERNAL,
        CHAIN_APPROX_SIMPLE,
        Point(0, 0));

  //Bordes: where i'm going to draw the contour.
  //Mat Borders= Mat::zeros(imgThreshold.size(), CV_8UC3);

 Scalar color_contours( rand()&0, rand()&255, rand()&0 ); //Green
 //Draw the contour.
 for(int i = 0; i < contours.size(); i++) 
    drawContours(src, 
        contours, 
        i, 
        color_contours, 
        5, 
        8, 
        hierarchy,
        2, 
        Point());
	   
  cout<<"nUMERO->"<<contours.size();
  imwrite( "_Booooorder.png", src );

  

}