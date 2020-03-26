#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // Get command line arguments
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " image_file " << std::endl;
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
  }

  std::cout << "Image input size: " << image.size() << std::endl;
  Mat dest_img,distCoeffs;
  Mat cameraMatrix(3,3, CV_64F);
  cameraMatrix =0;
  //cameraMatrix.at<double>(0,0) 
  undistort(image,dest_img,cameraMatrix,distCoeffs);
  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "_distorsion.png", dest_img );


  return( 0 );
}

// eof - 02_scale.cxx
