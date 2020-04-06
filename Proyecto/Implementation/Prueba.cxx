
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
/*
Mat src; 
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );
*/
/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
 Mat src;
  	src = imread( argv[1], IMREAD_COLOR);
//src = imread( "Level-1.1.jpg", IMREAD_COLOR);
  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  /// Convert image to gray and blur it
  Mat gray, blur_image, threshold_output;
  
	cvtColor(src, gray, COLOR_BGR2GRAY); // convert to grayscale

	blur(gray, blur_image, Size(3, 3)); // apply blur to grayscaled image

	threshold(blur_image, threshold_output, 127, 255, THRESH_BINARY_INV);
 stringstream ss( argv[ 1 ] );
  string basename;
  getline( ss, basename, '.' );
  imwrite( basename +"BinaryInv.jpg", threshold_output );
  vector< vector<Point> > contours; // list of contour points
	vector<Vec4i> hierarchy;

	// find contours
	//findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
   // create hull array for convex hull points

	vector< vector<Point> > hull(contours.size());

	for(int i = 0; i < contours.size(); i++)

	    convexHull(Mat(contours[i]), hull[i], false);
// create a blank image (black image)

	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);

	for(int i = 0; i < contours.size(); i++) 
  {

	    //Scalar color_contours = Scalar(0, 255, 0); // green - color for contours
      Scalar color_contours( rand()&0, rand()&255, rand()&0 );
	    //Scalar color = Scalar(255, 0, 0); // blue - color for convex hull
      //Scalar color( rand()&255, rand()&0, rand()&0 );
       
	    // draw ith contour
	    drawContours(drawing, contours, i, color_contours, 1, 8, vector<Vec4i>(), 2, Point());
	    // draw ith convex hull
	    //drawContours(drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point());

	}
  
  imwrite( basename + "Border.jpg", drawing );

  return(0);
}
