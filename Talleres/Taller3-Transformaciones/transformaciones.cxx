#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

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
  Mat image;
  image = imread( argv[1], 1 );

  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi

  Mat s(2,3,CV_64F);
  s=0;
  Mat r(2,3,CV_64F);
  r=0;
  Mat t(2,3,CV_64F);
  t=0;
  Mat M(2,3,CV_64F);
  M=0;

  s.at<double>(0,0) = 0.8;    
  s.at<double>(1,1) = 0.8;  

  r.at<double>(0,0) = 0.819;    
  r.at<double>(0,1) = -0.573;   
  r.at<double>(1,0) = 0.573;   
  r.at<double>(1,1) = 0.819; 

  t.at<double>(0,2) = -42;
  t.at<double>(0,0) = 1;
  t.at<double>(0,1) = 0;
  t.at<double>(1,0) = 0;
  t.at<double>(1,1) = 1;   
  t.at<double>(1,2) = 37; 

  M.at<double>(0,0) = 0.655;    
  M.at<double>(0,1) = -0.459;    
  M.at<double>(0,2) = -44.501;   
  M.at<double>(1,0) = 0.459;   
  M.at<double>(1,1) = 0.655;   
  M.at<double>(1,2) = 4.975;
   
 Mat dest1, dest2, dt, dr;
 Size size(image.cols,image.rows);

 warpAffine(image, dt, t, size);

  //imwrite( "Transladar.png", dt );

 warpAffine(dt, dr, r, size);

  //imwrite( "Rotacion.png", dr );
  warpAffine(dr, dest1, s, size);
  imwrite( "StepByStep.png", dest1 );

  warpAffine(image, dest2,M, size);
  imwrite( "MatrizCompuesta.png", dest2 );

  return( 0 );
}

// eof - 01_color_channels.cxx
