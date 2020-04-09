#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Coin.h"

using namespace cv;
using namespace std;

double getValueImage( vector<Coin *> m_Coins, double m_Area);

int main(int argc, char** argv )
{
  
  // Get command line arguments
  if ( argc < 2 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
    return( -1 );

  } 

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  Mat src;
  src = imread( argv[1], 1 );
  stringstream ss( argv[ 1 ] );
  string basename;
  getline( ss, basename, '.' );

  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  
  vector<Coin *> m_Coins;
  
  m_Coins.push_back(new Coin( 47000, 39000, 200));
  m_Coins.push_back(new Coin( 26000, 16000, 100));
  m_Coins.push_back(new Coin( 15000, 7000, 50));
  double m_Total;

  Mat bw;
  cvtColor(src, bw, COLOR_BGR2GRAY);
  threshold(bw, bw, 0, 255, THRESH_BINARY | THRESH_OTSU);
	Mat M = Mat::ones(3, 3, CV_8U);
	dilate(bw, bw, M);
	
	// Perform the distance transform algorithm
  Mat dist;
  distanceTransform(bw, dist, CV_DIST_L2, 5);

  // Normalize the distance image for range = {0.0, 1.0} so we can visualize and threshold it
  normalize(dist, dist, 0, 1, NORM_MINMAX);
  

  // Threshold to obtain the peaks, this will be the markers for the foreground objects
  threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);
    
	// Dilate a bit the dist image
  Mat kernel1 = Mat::ones(3, 3, CV_8U);
  dilate(dist, dist, kernel1);

  // Create the CV_8U version of the distance image, it is needed for findContours()
  Mat dist_8u;
  dist.convertTo(dist_8u, CV_8U);
	
  // Find total markers
  vector<vector<Point> > contours;
  findContours(dist_8u, contours, RETR_TREE, CHAIN_APPROX_NONE );

  // Create the marker image for the watershed algorithm
  Mat markers = Mat::zeros(dist.size(), CV_32S);
  // Draw the foreground markers
  for (size_t i = 0; i < contours.size(); i++)
  {
      drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
  }
  // Draw the background marker
  circle(markers, Point(5,5), 1, Scalar(255), -1);

  // Perform the watershed algorithm
  watershed(src, markers);
	
  Mat mark;
  markers.convertTo(mark, CV_8U);
  bitwise_not(mark, mark);

  // Generate random colors
  vector<Vec3b> colors;
  for (size_t i = 0; i < contours.size(); i++)
  {
      int b = theRNG().uniform(0, 256);
      int g = theRNG().uniform(0, 256);
      int r = theRNG().uniform(0, 256);
      colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
  }

  // Create the result image
  Mat dst = Mat::zeros(markers.size(), CV_8UC3);
  // Fill labeled objects with random colors
  for (int i = 0; i < markers.rows; i++)
  {
      for (int j = 0; j < markers.cols; j++)
      {
          int index = markers.at<int>(i,j);
          if (index > 0 && index <= static_cast<int>(contours.size()))
          {
              dst.at<Vec3b>(i,j) = colors[index-1];
          }
      }
  }
	
 

  // Visualize the final image
  imwrite(basename+"FinalResult.png", dst);
   

  for (unsigned int i = 0;  i < contours.size();  i++)
  {
    //std::cout << "# of contour points: " << contours[i].size() << std::endl;
    //std::cout << " Area: " << contourArea(contours[i])<< std::endl;
    double value = getValueImage(m_Coins, contourArea(contours[i]));
    m_Total+=value;
    char str[200];
    sprintf(str,"Value : %0.0lf$",value);
    putText(src, str ,contours[i][0], FONT_HERSHEY_DUPLEX, 0.8, Scalar(0,255,0), 2);
  }
  
  char str[200];
  sprintf(str,"TOTAL: %0.0lf",m_Total);
  putText(src, str ,Point((src.cols/2)-150,1000), FONT_HERSHEY_DUPLEX, 02, Scalar(0,255,0), 2);

	cout<<"Hay "<<contours.size()<<" monedas";
  cout<<", para un total de :"<<m_Total<<"$ "<<endl;
  imwrite(basename+"FinalValue.png", src);
  return 0;

}


double getValueImage( vector<Coin *> m_Coins, double m_Area)
{
  for(int i =0; i<3; i++)
     if( m_Coins[i]->isTheCoin(m_Area))
          return m_Coins[i]->getValue();
         
  return 0;
  
}
