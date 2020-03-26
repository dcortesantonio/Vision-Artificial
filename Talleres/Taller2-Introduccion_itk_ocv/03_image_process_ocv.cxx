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

  // Create color channel images
  // red channel
  Mat rImg = Mat::zeros( image.size( ), CV_8UC3 );
  // green channel
  Mat gImg = Mat::zeros( image.size( ), CV_8UC3 );
  // blue channel
  Mat bImg = Mat::zeros( image.size( ), CV_8UC3 );
  // composite image (RGB)
  Mat rgbImg = Mat::zeros( image.size( ), CV_8UC3 );
   
  // Fill color channel images
  MatIterator_< Vec3b > it, crIt, cgIt, cbIt, rgbIt, end, endr, endg, endb, endrgb;
  it = image.begin< Vec3b >( );
  crIt = rImg.begin< Vec3b >( );
  cgIt = gImg.begin< Vec3b >( );
  cbIt = bImg.begin< Vec3b >( );
  end = image.end< Vec3b >( );
  endr = rImg.end< Vec3b >( );
  endg = gImg.end< Vec3b >( );
  endb = bImg.end< Vec3b >( );
  for(  ; it != end, crIt != endr, cgIt != endg, cbIt != endb; ++it, ++crIt, ++cgIt, ++cbIt)
  {
  	(*crIt)[0] = 0;
  	(*crIt)[1] = 0;
  	(*crIt)[2] = (*it)[2];

  	(*cgIt)[0] = 0;
  	(*cgIt)[1] = (*it)[1];
  	(*cgIt)[2] = 0;
  	
  	(*cbIt)[0] = (*it)[0];
  	(*cbIt)[1] = 0;
  	(*cbIt)[2] = 0;

  } // rof

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  imwrite( basename + "_R.png", rImg );
  imwrite( basename + "_G.png", gImg );
  imwrite( basename + "_B.png", bImg );


  Mat sR;
  resize( rImg, sR, Size(), 0.75, 0.75, INTER_LINEAR);

  Mat sG;
  resize(gImg, sG, Size(), 0.5, 0.5, INTER_LINEAR);

  Mat sB;
  resize(bImg, sB, Size(), 0.25, 0.25, INTER_LINEAR);


  imwrite( basename + "_sR.png", sR );
  imwrite( basename + "_sG.png", sG );
  imwrite( basename + "_sB.png", sB );

  Mat ssR;
  resize( sR, ssR, Size(), 1.3333, 1.3333, INTER_LINEAR);

  Mat ssG;
  resize(sG, ssG, Size(), 2, 2, INTER_LINEAR);

  Mat ssB;
  resize( sB, ssB, Size(), 4, 4, INTER_LINEAR);


  imwrite( basename + "_ssR.png", ssR );
  imwrite( basename + "_ssG.png", ssG );
  imwrite( basename + "_ssB.png", ssB );


  // From color channel images, reconstruct the original color image
  crIt = ssR.begin< Vec3b >( );
  cgIt = ssG.begin< Vec3b >( );
  cbIt = ssB.begin< Vec3b >( );
  rgbIt = rgbImg.begin< Vec3b >( );
  endr = ssR.end< Vec3b >( );
  endg = ssG.end< Vec3b >( );
  endb = ssB.end< Vec3b >( );
  endrgb = rgbImg.end< Vec3b >( );
  for(  ; crIt != endr, cgIt != endg, cbIt != endb, rgbIt != endrgb; ++crIt, ++cgIt, ++cbIt, ++rgbIt)
  {
    (*rgbIt)[0] = (*cbIt)[0];  // BLUE [0]
    (*rgbIt)[1] = (*cgIt)[1]; // GREEN [1]
    (*rgbIt)[2] = (*crIt)[2];  //RED [2]
    
  } // rof
  imwrite( basename + "_rRGB.png", rgbImg );

    Mat diff;
    absdiff( image, rgbImg, diff); 
  imwrite( basename + "_diff.png", diff );


  return( 0 );
}

// eof - 01_color_channels.cxx
