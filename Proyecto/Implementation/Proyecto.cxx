#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Coin.h"


std::string getBaseName(  std::string s);
cv::Mat generateCirles(cv::Mat markers, std::vector<std::vector<cv::Point> > contours);
std::vector<cv::Vec3b> generateColors(  std::vector<std::vector<cv::Point> > contours);
double getValueImage( std::vector<Coin *> m_Coins, double m_Area, cv::Mat src);
double getTotalValue(std::vector<Coin *> m_Coins,  std::vector<std::vector<cv::Point> > contours, cv::Mat src);

int main(int argc, char** argv )
{
  
  // Get command line arguments
  if ( argc < 2 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
    return( -1 );

  } 

  // Review name of the image.
  std::cout << "-------------------------" << std::endl;
  std::cout << " Read image: "<<argv[ 1 ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  cv::Mat src, re;
  src = cv::imread( argv[1], 1 );
  std::string basename = getBaseName( argv[ 1 ]);

  //Resize image 
  double scale = float(1500)/src.size().width;
  resize(src, src, cv::Size(0, 0), scale, scale);
 
  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } 
  
  //Information of the Coins.
  std::vector<Coin *> m_Coins;
  m_Coins.push_back(new Coin( 346, 331, 200));
  m_Coins.push_back(new Coin( 263, 211.6, 100));
  m_Coins.push_back(new Coin( 211.6, 5, 50));
  double m_Total;

  cv::Mat bw;
  cvtColor(src, bw, cv::COLOR_BGR2GRAY);
  threshold(bw, bw, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::Mat M = cv::Mat::ones(3, 3, CV_8U);
	dilate(bw, bw, M);
	
	// Perform the distance transform algorithm
  cv::Mat dist;
  distanceTransform(bw, dist, CV_DIST_L2, 5);

  // Normalize the distance image for range = {0.0, 1.0} so we can visualize and threshold it
  normalize(dist, dist, 0, 1, cv::NORM_MINMAX);

  // Threshold to obtain the peaks, this will be the markers for the foreground objects
  threshold(dist, dist, 0.4, 1.0, cv::THRESH_BINARY);
    
	// Dilate a bit the dist image
  cv::Mat kernel1 = cv::Mat::ones(3, 3, CV_8U);
  dilate(dist, dist, kernel1);

  // Create the CV_8U version of the distance image, it is needed for findContours()
  cv::Mat dist_8u;
  dist.convertTo(dist_8u, CV_8U);
	
  // Find total markers
  std::vector<std::vector<cv::Point> > contours;
  cv::findContours(dist_8u, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE );

  // Create the marker image for the watershed algorithm
  // Draw the foreground markers
  cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32S);
  for (size_t i = 0; i < contours.size(); i++)
  {
      drawContours(markers, contours, static_cast<int>(i), cv::Scalar(static_cast<int>(i)+1), -1);
  }


  // Draw the background marker
  circle(markers, cv::Point(5,5), 1, cv::Scalar(255), -1);

  // Perform the watershed algorithm
  watershed(src, markers);
	
  cv::Mat mark;
  markers.convertTo(mark, CV_8U);
  bitwise_not(mark, mark);

  cv::Mat dst = generateCirles(markers, contours);

  //Get values of coins.
  m_Total = getTotalValue(m_Coins, contours, src);
	std::cout<<"Hay "<<contours.size()<<" monedas"<<", para un total de :"<<m_Total<<"$ "<<std::endl;

 

  // Visualize the final images
  imwrite(basename+"FinalResult.png", dst); // Circles
  imwrite(basename+"FinalValue.png", src);  // Values and final result

  return 0;

}


std::string getBaseName(  std::string s)
{
  
  std::stringstream ss( s );
  std::string basename;
  getline( ss, basename, '.' );
         
  return basename;
  
}

cv::Mat generateCirles(cv::Mat markers, std::vector<std::vector<cv::Point> > contours)
{
  
   // Generate random colors
  std::vector<cv::Vec3b> colors = generateColors(contours);

  // Create the result image
  cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
  // Fill labeled objects with random colors
  for (int i = 0; i < markers.rows; i++)
  {
      for (int j = 0; j < markers.cols; j++)
      {
          int index = markers.at<int>(i,j);
          if (index > 0 && index <= static_cast<int>(contours.size()))
          {
              dst.at<cv::Vec3b>(i,j) = colors[index-1];
          }
      }
  }
  return dst;
  
}
std::vector<cv::Vec3b> generateColors(  std::vector<std::vector<cv::Point> > contours)
{
   std::vector<cv::Vec3b> colors;
  for (size_t i = 0; i < contours.size(); i++)
  {
      int b = cv::theRNG().uniform(0, 256);
      int g = cv::theRNG().uniform(0, 256);
      int r = cv::theRNG().uniform(0, 256);
      colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
  }
         
  return colors;
  
}

double getValueImage( std::vector<Coin *> m_Coins, double m_Area)
{
  for(int i =0; i<3; i++)
     if( m_Coins[i]->isTheCoin(m_Area))
          return m_Coins[i]->getValue();
         
  return 0;
  
}


double getTotalValue(std::vector<Coin *> m_Coins,  std::vector<std::vector<cv::Point> > contours, cv::Mat src)
{
  double m_Total=0;
   for (unsigned int i = 0;  i < contours.size();  i++)
  {
   
    double equiDiameter = sqrt( (4 * cv::contourArea(contours[i])) / M_PI);
    double value = getValueImage(m_Coins, equiDiameter);
    m_Total+=value;
    char str[200];
    sprintf(str,"Value: %0.0lf$", value);
    putText(src, str ,contours[i][0], cv::FONT_HERSHEY_DUPLEX, 0.9, cv::Scalar(0,255,0), 2);
  }

  char str[200];
  sprintf(str,"TOTAL: %0.0lf",m_Total);
  putText(src, str ,cv::Point((src.cols/2)-170,(4*src.rows/5)), cv::FONT_HERSHEY_DUPLEX, 02, cv::Scalar(0,255,0), 2);
         
  return m_Total;
  
}

