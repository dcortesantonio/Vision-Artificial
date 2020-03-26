#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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
  Mat src, img_gray;
	src = imread( argv[1], IMREAD_COLOR);

  if ( !src.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  
	//SE PASA LA IMAGEN A ESCALA DE GRISES
	cvtColor(src, img_gray, cv::COLOR_BGR2GRAY);
	
	int totalPixels = img_gray.cols*img_gray.rows;
	
  int histSize = 256;

  float range[] = { 0, 256 } ;
  const float* histRange = { range };

 	cv::Mat hist;
  int channels[] = {0};

  //Se calcula el histograma
  calcHist( &img_gray, 1, channels, Mat(), hist, 1, &histSize, &histRange, true, false );

	//De aquí para abajo se mira cual humbral seleccionar
	float minClassVariance = 3.40e38;
	int tresh =0;
	//Wb -> peso fondo, Mb -> media fondo, Ob -> varianza fondo, Wf -> peso clase, Mf -> media clase, Of -> varianza clase
	double Wb = 0, Mb = 0, Ob = 0, Wf = 0, Mf = 0, Of = 0;
	//WCV -> Varianza entre clase y fondo
	double WCV = 0;
	//numWb -> numerador peso fondo, numMb -> numerador media fondo, numOb -> numerador varianza fondo
	float numWb = 0, numMb = 0, numOb = 0;
		//numWf -> numerador peso clase, numMf -> numerador media clase, numOf -> numerador varianza clase
	float numWf = 0, numMf = 0, numOf = 0;
	bool allClass = false;

	for(int i = 0; i < 256; i++){
			//Si es el primer humbral significa que todo es clase
			if(i == 0){
				Wb = 0;
				Mb = 0; 
				Ob = 0;
				allClass = true;
			}

			//Si no es el primer humbral saco todo normal
			if(!allClass){
				//Se encuentra el valor del numerador del peso y la media del fondo
				for(int j = 0; j < i; j++){
					numWb+= hist.at<float>(j);
					numMb+= j*hist.at<float>(j);
				}
				//Peso del fondo para el humbral i
				Wb = numWb/totalPixels;
				//Media del fondo para el humbral i
				if(numWb==0){
					Mb = 0;
				}else{
					Mb = numMb/numWb;
				}
				//Se encuentra el valor del numerador de la varianza del fondo
				for(int j = 0; j < i; j++){
						numOb+= ( ((j - Mb)*(j -Mb)) * hist.at<float>(j) );	
				}
				//Varianza dle fondo
				if(numWb==0){
					Ob = 0;
				}else{
					Ob = numOb/numWb;
				}
			}

	
			//Se encuentra el valor del numerador del peso y la media de la clase
			for(int k = i; k < 256 ; k++){
				numWf+= hist.at<float>(k);
				numMf+= k*hist.at<float>(k);
			}
			//Peso de la clase para el humbral i
			Wf = numWf/totalPixels;
			//Media de la clase para el humbral i
			if(numWf==0){
				Mf = 0;
			}else{
				Mf = numMf/numWf;
			}
			
			//Se encuentra el valor del numerador de la varianza de la clase
			for(int k = i; k < 256; k++){
					numOf+= ( ((k - Mf)*(k -Mf)) * hist.at<float>(k) );	
			}
			//Varianza de la clase para el humbral i
			if(numWf==0){
				Of = 0;
			}else{
				Of = numOf/numWf;
			}
			WCV = (Wb*Ob) + (Wf*Of);
			//Si el WCV del humbral i es menor que minClassVariance se cambia minClassVariance
			if(WCV <= minClassVariance){
				tresh = i;
				minClassVariance = WCV;
			}
			allClass = false;
			Wb = 0;
			Mb = 0; 
			Ob = 0;
			Wf = 0;
		  Mf = 0;
			Of = 0;
			numWb = 0;
			numMb = 0;
			numOb = 0;
			numWf = 0;
			numMf = 0;
			numOf = 0;
	}	

	cout<<"Se utiliza el humbral de: "<<tresh<<endl;
	Mat binary;
	Mat trunc;


	threshold(img_gray, binary, tresh, 255, CV_THRESH_BINARY);
	imwrite( "Binary.png", binary );

	threshold(img_gray, trunc, tresh, 255, CV_THRESH_TOZERO);
	imwrite( "ToZero.png", trunc );
	
  return( 0 );
}

// eof - 01_color_channels.cxx
