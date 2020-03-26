#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv )
{
  // Get command line arguments
  //hI
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
  Mat imageOriginal, image;
  imageOriginal = imread( argv[1], 1 );
  image = imageOriginal;
  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  } 
   Mat img_dst;

  std::stringstream ss( argv[ 1 ] );
  std::string basename = "Gauss Nivel-";
  std::string basename2 = "Laplace Nivel-";
  char n= 51;
  int size, i , f ,c;
  Mat rgbImg;
  Mat diffImg;
  Mat niv1Gauss;
  for( int im = 0; im < 3; im++)
  {
    GaussianBlur(image, img_dst, Size(5, 5), 0);
    std::string img_name = basename+n;
    n--;
    size = img_dst.size().width;
    i = 1, f=0, c=1; 
    rgbImg = Mat::zeros( img_dst.size( )/2, CV_8UC3 );
    rgbImg = Scalar(100,100,100);
    MatIterator_< Vec3b > it, rgbIt, end, endrgb;
    it = img_dst.begin< Vec3b >( );
    rgbIt = rgbImg.begin< Vec3b >( );
    end = img_dst.end< Vec3b >( );
    endrgb = rgbImg.end< Vec3b >( );
    for(  ; it!=end ,rgbIt != endrgb; ++it)
    {
      
      if( i % size==0)
      {
          c=1;
          f+=1;
      
      }
      if( c%2==1 && f%2==1)
      {
        (*rgbIt)[0] = (*it)[0];
        (*rgbIt)[1] = (*it)[1];
        (*rgbIt)[2] = (*it)[2];
      ++rgbIt;
      }
      i++;
      c++;
    }
    
    image = rgbImg;
     imwrite( img_name + ".png", rgbImg );
  }
  niv1Gauss = image;
  image = imageOriginal;
  n= 53;
   for( int im = 0; im < 3; im++)
  {
    std::string img_name = basename+n;
    i = 1, f=0, c=1; 
    rgbImg = Mat::zeros( image.size( )*2, CV_8UC3 );
    rgbImg = Scalar(100,100,100);
    MatIterator_< Vec3b > it, rgbIt, end, endrgb;
    it = image.begin< Vec3b >( );
    rgbIt = rgbImg.begin< Vec3b >( );
    end = image.end< Vec3b >( );
    endrgb = rgbImg.end< Vec3b >( );

    size = rgbImg.size().width;
    for(  ; it!=end ,rgbIt != endrgb;  ++rgbIt)
    {
      
      if( i % size==0)
      {
        c=1;
        f+=1;
         
      
      }
      if( c%2==1 && f%2==1)
      {
       (*rgbIt)[0] = (*it)[0];
        (*rgbIt)[1] = (*it)[1];
        (*rgbIt)[2] = (*it)[2];
      ++it;
      }
      i++;
      c++;
    }

    GaussianBlur(rgbImg, img_dst, Size(5, 5), 0);
    image = rgbImg;
    imwrite( img_name + ".png", rgbImg );
    n++;
  }
  //Laplaciana
  n=49;
  for (int j = 0; j < 7 ; j++){
	std::string img_name2 = basename2+n;
	if(j==0){
		imwrite( img_name2 + ".png", niv1Gauss );
		n++;
	}else{
		std::string img_name2 = basename2+n;
		i = 1, f=0, c=1; 
		rgbImg = Mat::ones( niv1Gauss.size( )*2, CV_8UC3 );
		//rgbImg = Scalar(100,100,100);
		MatIterator_< Vec3b > it, rgbIt, end, endrgb;
		it = niv1Gauss.begin< Vec3b >( ); 
		rgbIt = rgbImg.begin< Vec3b >( );
		end = niv1Gauss.end< Vec3b >( );
		endrgb = rgbImg.end< Vec3b >( );

		size = rgbImg.size().width;
		for(  ; it!=end ,rgbIt != endrgb;  ++rgbIt)
		{
		  
		  if( i % size==0)
		  {
		    c=1;
		    f+=1;
		     
		  
		  }
		  if( c%2==1 && f%2==1)
		  {
		   (*rgbIt)[0] = (*it)[0];
		    (*rgbIt)[1] = (*it)[1];
		    (*rgbIt)[2] = (*it)[2];
		  ++it;
		  }
		  i++;
		  c++;
		} 
		GaussianBlur(rgbImg, img_dst, Size(5, 5), 0);
		absdiff(img_dst,rgbImg,diffImg);
		imwrite( img_name2 + ".png", diffImg );
		niv1Gauss = rgbImg;
		n++;

	 }	
  }

  return( 0 );
}

// eof - 02_scale.cxx
