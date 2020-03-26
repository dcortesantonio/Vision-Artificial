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

  // Rescale image
  cv::Point2f src_p[4];
	cv::Point2f dst_p[4];

	float w  = (float)image.cols;
	float h  = (float)image.rows;
	float hw = w / 2.0f;
	float hh = h;

	// from points
	src_p[0] = cv::Point2f(0.0f, 0.0f);
	src_p[1] = cv::Point2f(   w, 0.0f);
	src_p[2] = cv::Point2f(   w,    h);
	src_p[3] = cv::Point2f(0.0f,    h);

	// to points
	dst_p[0] = cv::Point2f(  hw, 0.0f);
	dst_p[1] = cv::Point2f(   w,   hh);
	dst_p[2] = cv::Point2f(  hw,    h);
	dst_p[3] = cv::Point2f(0.0f,   hh);

  Mat dest_img, dest_img2;
  Mat warpMatrix = getPerspectiveTransform(src_p, dst_p); 
  /*
  warpMatrix=0;76
  warpMatrix.at<double>(0,0)=4.88;
  warpMatrix.at<double>(1,1)=2.74;
  warpMatrix.at<double>(2,2)=0;
*/
  warpPerspective(image, dest_img, warpMatrix, image.size(), INTER_LINEAR, BORDER_CONSTANT);

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "_distorsion.png", dest_img );


  return( 0 );
}

// eof - 02_scale.cxx
