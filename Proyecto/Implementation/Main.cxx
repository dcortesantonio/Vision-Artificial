#include "opencv2/highgui/highgui.hpp"
// highgui - an interface to video and image capturing.

#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
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
  
    Mat src, img_gray, imgBlu,imgCanny;
	src = imread( argv[1], IMREAD_COLOR);
//src = imread( "Level-1.1.jpg", IMREAD_COLOR);
  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  
	//SE PASA LA IMAGEN A ESCALA DE GRISES
	cvtColor(src, img_gray, cv::COLOR_BGR2GRAY);
   GaussianBlur(img_gray,            // input image
        imgBlu,                            // output image
        cv::Size(5, 5),                        // smoothing window width and height in pixels
        1.5);                                // sigma value, determines how much the image will be blurred

    Canny(imgBlu,            // input image
        imgCanny,                    // output image
        100,                        // low threshold
        200);                        // high threshold
        imwrite( "imgBlu.png", imgBlu );
        imwrite( "Borders.png", imgCanny );   
}