#include <cmath>
#include <cstdlib>
#include <limits>
#include <iostream>
#include <string>
#include <sstream>

#include <itkImage.h>
#include <itkRGBPixel.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>


#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>

// Image type: 2-dimensional 1-byte rgb
const unsigned int Dim = 2;
typedef unsigned char                   TRGBResolution;
typedef itk::RGBPixel< TRGBResolution > TRGBPixel;
typedef itk::Image< TRGBPixel, Dim >    TColorImage;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::ImageRegionConstIteratorWithIndex< TColorImage > TIterator;
typedef itk::ImageRegionIteratorWithIndex< TColorImage > TColorIterator;
typedef itk::ImageFileWriter< TColorImage > TWriter;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::IdentityTransform<double, 2> TransformType;
typedef itk::ResampleImageFilter< TColorImage, TColorImage > ResampleImageFilterType;
typedef itk::ImageFileWriter< TColorImage > TWriter;

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  // Get command line arguments
  if( argc < 2 )
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
  TReader::Pointer reader = TReader::New( );
  reader->SetFileName( argv[ 1 ] );
  try
  {
    reader->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  TColorImage* img = reader->GetOutput( );

  // Create color channel images
  // red channel
  TColorImage::Pointer rImg = TColorImage::New( );
  rImg->SetSpacing( img->GetSpacing( ) );
  rImg->SetOrigin( img->GetOrigin( ) );
  rImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  rImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  rImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  rImg->Allocate( );

  // green channel
  TColorImage::Pointer gImg = TColorImage::New( );
  gImg->SetSpacing( img->GetSpacing( ) );
  gImg->SetOrigin( img->GetOrigin( ) );
  gImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  gImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  gImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  gImg->Allocate( );

  // blue channel
  TColorImage::Pointer bImg = TColorImage::New( );
  bImg->SetSpacing( img->GetSpacing( ) );
  bImg->SetOrigin( img->GetOrigin( ) );
  bImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  bImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  bImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  bImg->Allocate( );

  // composite image (RGB)
  TColorImage::Pointer rgbImg = TColorImage::New( );
  rgbImg->SetSpacing( img->GetSpacing( ) );
  rgbImg->SetOrigin( img->GetOrigin( ) );
  rgbImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  rgbImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  rgbImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  rgbImg->Allocate( );

  // composite image (diff)
  TColorImage::Pointer diffImg = TColorImage::New( );
  diffImg->SetSpacing( img->GetSpacing( ) );
  diffImg->SetOrigin( img->GetOrigin( ) );
  diffImg->SetLargestPossibleRegion( img->GetLargestPossibleRegion( ) );
  diffImg->SetRequestedRegion( img->GetRequestedRegion( ) );
  diffImg->SetBufferedRegion( img->GetBufferedRegion( ) );
  diffImg->Allocate( );

  // Initialize created images in black
  TRGBPixel black;
  black.SetRed( 0 );
  black.SetGreen( 0 );
  black.SetBlue( 0 );
  rImg->FillBuffer( black );
  gImg->FillBuffer( black );
  bImg->FillBuffer( black );
  rgbImg->FillBuffer( black );
  diffImg->FillBuffer( black );

  // Fill color channel images
  TIterator it( img, img->GetLargestPossibleRegion( ) );
  TColorIterator crIt( rImg, rImg->GetLargestPossibleRegion( ) );
  TColorIterator cgIt( gImg, gImg->GetLargestPossibleRegion( ) );
  TColorIterator cbIt( bImg, bImg->GetLargestPossibleRegion( ) );

  it.GoToBegin( );
  crIt.GoToBegin( );
  cgIt.GoToBegin( );
  cbIt.GoToBegin( );
  for( ; !it.IsAtEnd( ) && !crIt.IsAtEnd( ) && !cgIt.IsAtEnd( ) && !cbIt.IsAtEnd( ); ++it, ++crIt, ++cgIt, ++cbIt )
  {
    TRGBPixel value, pixel;
    pixel = it.Get( );
    value.SetRed( pixel.GetRed( ) );
    value.SetGreen( 0 );
    value.SetBlue( 0 );
    crIt.Set( value );

    value.SetRed( 0 );
    value.SetGreen( pixel.GetGreen( ) );
    value.SetBlue( 0 );
    cgIt.Set( value );

    value.SetRed( 0 );
    value.SetGreen( 0 );
    value.SetBlue( pixel.GetBlue( ) );
    cbIt.Set( value );

  } // rof
  
 

  // Write results
  std::stringstream ss( argv[ 1 ] );
  std::string basename;
  getline( ss, basename, '.' );

  TWriter::Pointer writer = TWriter::New( );
  writer->SetInput( rImg );
  writer->SetFileName( basename + "_R.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( gImg );
  writer->SetFileName( basename + "_G.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  writer->SetInput( bImg );
  writer->SetFileName( basename + "_B.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  

    // rImg gImg bImg
 TColorImage::SizeType inputSize = rImg->GetLargestPossibleRegion( ).GetSize( );

  // Calculate output size and scaling
  TColorImage::SizeType outputSizeR;
  outputSizeR[ 0 ] = inputSize[ 0 ] * 0.75;
  outputSizeR[ 1 ] = inputSize[ 1 ] * 0.75;
  TColorImage::SpacingType outputSpacingR;
  outputSpacingR[ 0 ] = 
    rImg->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSizeR[ 0 ] ) );
  outputSpacingR[ 1 ] = 
    rImg->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSizeR[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFilterR = ResampleImageFilterType::New( );
  resampleFilterR->SetTransform( TransformType::New( ) );
  resampleFilterR->SetInput( rImg );
  resampleFilterR->SetSize( outputSizeR );
  resampleFilterR->SetOutputSpacing( outputSpacingR );
  resampleFilterR->UpdateLargestPossibleRegion( );

  
  writer->SetInput( resampleFilterR->GetOutput( ) );
  writer->SetFileName( basename + "_sR.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

  //Scale Green
    inputSize = gImg->GetLargestPossibleRegion( ).GetSize( );

  // Calculate output size and scaling
  TColorImage::SizeType outputSizeG;
  outputSizeG[ 0 ] = inputSize[ 0 ] * 0.5;
  outputSizeG[ 1 ] = inputSize[ 1 ] * 0.5;
  TColorImage::SpacingType outputSpacingG;
  outputSpacingG[ 0 ] = 
    gImg->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSizeG[ 0 ] ) );
  outputSpacingG[ 1 ] = 
    gImg->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSizeG[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFilterG = ResampleImageFilterType::New( );
  resampleFilterG->SetTransform( TransformType::New( ) );
  resampleFilterG->SetInput( gImg );
  resampleFilterG->SetSize( outputSizeG );
  resampleFilterG->SetOutputSpacing( outputSpacingG );
  resampleFilterG->UpdateLargestPossibleRegion( );

  writer->SetInput( resampleFilterG->GetOutput( ) );
  writer->SetFileName( basename + "_sG.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
 
   //Scale Blue
    inputSize = bImg->GetLargestPossibleRegion( ).GetSize( );

  // Calculate output size and scaling
  TColorImage::SizeType outputSizeB;
  outputSizeB[ 0 ] = inputSize[ 0 ] * 0.25;
  outputSizeB[ 1 ] = inputSize[ 1 ] * 0.25;
  TColorImage::SpacingType outputSpacingB;
  outputSpacingB[ 0 ] = 
    bImg->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSize[ 0 ] ) / static_cast< double >( outputSizeB[ 0 ] ) );
  outputSpacingB[ 1 ] = 
    bImg->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSize[ 1 ] ) / static_cast< double >( outputSizeB[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFilterB = ResampleImageFilterType::New( );
  resampleFilterB->SetTransform( TransformType::New( ) );
  resampleFilterB->SetInput( bImg );
  resampleFilterB->SetSize( outputSizeB );
  resampleFilterB->SetOutputSpacing( outputSpacingB );
  resampleFilterB->UpdateLargestPossibleRegion( );

  
  writer->SetInput( resampleFilterB->GetOutput( ) );
  writer->SetFileName( basename + "_sB.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

    //Reescale red
    TColorImage::SizeType inputSizeR = resampleFilterR->GetSize( );

  // Calculate output size and scaling
  outputSizeR[ 0 ] = inputSizeR[ 0 ] / 0.75;
  outputSizeR[ 1 ] = inputSizeR[ 1 ] / 0.75;
 outputSpacingR[ 0 ] = 
    resampleFilterR->GetOutput( )->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSizeR[ 0 ] ) / static_cast< double >( outputSizeR[ 0 ] ) );
  outputSpacingR[ 1 ] = 
     resampleFilterR->GetOutput( )->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSizeR[ 1 ] ) / static_cast< double >( outputSizeR[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFiltersR = ResampleImageFilterType::New( );
  resampleFiltersR->SetTransform( TransformType::New( ) );
  resampleFiltersR->SetInput(resampleFilterR->GetOutput( ) );
  resampleFiltersR->SetSize( outputSizeR );
  resampleFiltersR->SetOutputSpacing( outputSpacingR );
  resampleFiltersR->UpdateLargestPossibleRegion( );

  writer->SetInput( resampleFiltersR->GetOutput( ) );
  writer->SetFileName( basename + "_ssR.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

   //Reescale green
       TColorImage::SizeType inputSizeG = resampleFilterG->GetSize( );

  // Calculate output size and scaling
  outputSizeG[ 0 ] = inputSizeG[ 0 ] / 0.5;
  outputSizeG[ 1 ] = inputSizeG[ 1 ] / 0.5;
 outputSpacingG[ 0 ] = 
    resampleFilterG->GetOutput( )->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSizeG[ 0 ] ) / static_cast< double >( outputSizeG[ 0 ] ) );
  outputSpacingG[ 1 ] = 
     resampleFilterG->GetOutput( )->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSizeG[ 1 ] ) / static_cast< double >( outputSizeG[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFiltersG = ResampleImageFilterType::New( );
  resampleFiltersG->SetTransform( TransformType::New( ) );
  resampleFiltersG->SetInput(resampleFilterG->GetOutput( ) );
  resampleFiltersG->SetSize( outputSizeG );
  resampleFiltersG->SetOutputSpacing( outputSpacingG );
  resampleFiltersG->UpdateLargestPossibleRegion( );

  writer->SetInput( resampleFiltersG->GetOutput( ) );
  writer->SetFileName( basename + "_ssG.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt

 //Reescale green
       TColorImage::SizeType inputSizeB = resampleFilterB->GetSize( );

  // Calculate output size and scaling
  outputSizeB[ 0 ] = inputSizeB[ 0 ] / 0.25;
  outputSizeB[ 1 ] = inputSizeB[ 1 ] / 0.25;
 outputSpacingB[ 0 ] = 
    resampleFilterB->GetOutput( )->GetSpacing( )[ 0 ] * ( static_cast< double >( inputSizeB[ 0 ] ) / static_cast< double >( outputSizeB[ 0 ] ) );
  outputSpacingB[ 1 ] = 
     resampleFilterB->GetOutput( )->GetSpacing( )[ 1 ] * ( static_cast< double >( inputSizeB[ 1 ] ) / static_cast< double >( outputSizeB[ 1 ] ) );
  
  // Rescale image
  ResampleImageFilterType::Pointer resampleFiltersB = ResampleImageFilterType::New( );
  resampleFiltersB->SetTransform( TransformType::New( ) );
  resampleFiltersB->SetInput(resampleFilterB->GetOutput( ) );
  resampleFiltersB->SetSize( outputSizeB );
  resampleFiltersB->SetOutputSpacing( outputSpacingB );
  resampleFiltersB->UpdateLargestPossibleRegion( );

  writer->SetInput( resampleFiltersB->GetOutput( ) );
  writer->SetFileName( basename + "_ssB.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yr
 
     // From color channel images, reconstruct the original color image
  TColorIterator rgbIt( rgbImg, rgbImg->GetLargestPossibleRegion( ) );
  
  TColorIterator diff( diffImg, diffImg->GetLargestPossibleRegion( ) );
   TColorIterator csrIt( resampleFiltersR->GetOutput( ),  resampleFiltersR->GetOutput( )->GetLargestPossibleRegion( ) );
  TColorIterator csgIt( resampleFiltersG->GetOutput( ),  resampleFiltersG->GetOutput( )->GetLargestPossibleRegion( )  );
  TColorIterator csbIt( resampleFiltersB->GetOutput( ),  resampleFiltersB->GetOutput( )->GetLargestPossibleRegion( ) );

  rgbIt.GoToBegin( );
  csrIt.GoToBegin( );
  it.GoToBegin( );
  csgIt.GoToBegin( );
  csbIt.GoToBegin( );
  for( ; !diff.IsAtEnd( ) && !it.IsAtEnd( ) &&  !rgbIt.IsAtEnd( ) && !csrIt.IsAtEnd( ) && !csgIt.IsAtEnd( ) && !csbIt.IsAtEnd( ); ++it, ++diff,  ++rgbIt, ++csrIt, ++csgIt, ++csbIt )
  {
    TRGBPixel value, pixel, pixeld, pixeldimg;
     pixeldimg = it.Get();

    value = csrIt.Get( );
    pixel.SetRed( value.GetRed( ) );
    pixeld.SetRed( pixeldimg.GetRed() - value.GetRed() );

    value = csgIt.Get( );
    pixel.SetGreen( value.GetGreen( ) );
    pixeld.SetGreen( pixeldimg.GetGreen() - value.GetGreen() );


    value = csbIt.Get( );
    pixel.SetBlue( value.GetBlue( ) );
    pixeld.SetBlue( pixeldimg.GetBlue() - value.GetBlue() );

    rgbIt.Set( pixel );
    diff.Set(  pixeld );

  } // rof

   writer->SetInput( rgbImg );
  writer->SetFileName( basename + "_rRGB.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  
   writer->SetInput( diffImg );
  writer->SetFileName( basename + "_diff.png" );
  try
  {
    writer->Update( );

  }
  catch( itk::ExceptionObject& err )
  {
    std::cerr << "Error: " << err << std::endl;
    return( 1 );

  } // yrt
  return( 0 );
}

// eof - 01_color_channels.cxx
