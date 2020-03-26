#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // Get command line arguments
  if ( argc < 3 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file number_of_bits" << std::endl;
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

  // Convert RGB image to grayscale
  Mat gray_image = image;
Mat src = image;
Mat bgr[3];   //destination array
split(src,bgr);//split source  

//Note: OpenCV uses BGR color order

Mat red = bgr[0];
Mat blue = bgr[1];
Mat green = bgr[2];


  // Quantize into desired number of bits creando table
  uchar table[ 256 ];
  int scale = pow( 2, 8 - atoi( argv[ 2 ] ) );
  for (int i = 0; i < 256; ++i)
    table[ i ] = ( uchar )( scale * ( i / scale ) );

  Mat resint_imgr = Mat::zeros( image.size( ), CV_8UC1 );
  MatIterator_< uchar > it, end;
  for( it = resint_imgr.begin< uchar >( ), end = resint_imgr.end< uchar >( ); it != end; ++it)
    *it = table[*it];


  Mat resint_imgb = Mat::zeros( image.size( ), CV_8UC1 );

  for( it = resint_imgb.begin< uchar >( ), end = resint_imgb.end< uchar >( ); it != end; ++it)
    *it = table[*it];

  Mat resint_imgg = Mat::zeros( image.size( ), CV_8UC1 );
  
  for( it = resint_imgg.begin< uchar >( ), end = resint_imgg.end< uchar >( ); it != end; ++it)
    *it = table[*it];

  // Rescale intensities to cover the full range
  Mat lookUpTable(1, 256, CV_8U);
  uchar* p = lookUpTable.data;
  for( int i = 0; i < 256; ++i)
    p[i] = table[i];
  LUT( red , lookUpTable, resint_imgr);


  p = lookUpTable.data;
  for( int i = 0; i < 256; ++i)
    p[i] = table[i];
  LUT( blue , lookUpTable, resint_imgb);


 p = lookUpTable.data;
  for( int i = 0; i < 256; ++i)
    p[i] = table[i];
  LUT( green , lookUpTable, resint_imgg);

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  
  imwrite( basename + "_rescaleIntRED.png", resint_imgr );

  imwrite( basename + "_rescaleIntBLUE.png", resint_imgb );

  imwrite( basename + "_rescaleIntGREEN.png", resint_imgg );



	Mat R = imread(basename +"_rescaleIntRED.png", 0);
    Mat G = imread( basename + "_rescaleIntGREEN.png", 0);
    Mat B = imread( basename + "_rescaleIntBLUE.png", 0);

    std::vector<cv::Mat> array_to_merge;

    array_to_merge.push_back(B);
    array_to_merge.push_back(G);
    array_to_merge.push_back(R);

    Mat color;

    merge(array_to_merge, color);

    imwrite("merged.png", color);

  return( 0 );
}

// eof - 02_rescale_intensity.cxx
