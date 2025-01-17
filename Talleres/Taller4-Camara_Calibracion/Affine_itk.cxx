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
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAffineTransform.h"
#include "itkResampleImageFilter.h"
#include "itkWindowedSincInterpolateImageFunction.h"

// Image type: 2-dimensional 1-byte rgb
const unsigned int Dim = 2;
typedef unsigned char                   TRGBResolution;
typedef itk::RGBPixel< TRGBResolution > TRGBPixel;
typedef itk::Image< TRGBPixel, Dim >    TColorImage;

// Types definition
typedef itk::ImageFileReader< TColorImage > TReader;
typedef itk::IdentityTransform<double, 2> TransformType;
typedef itk::ResampleImageFilter< TColorImage, TColorImage > ResampleImageFilterType;
typedef itk::ImageFileWriter< TColorImage > TWriter;

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0];
    std::cerr << " <InputFileName> ";
    std::cerr << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension = 2;
  using ScalarType = double;

  const char * inputFileName = argv[1];
  const char * outputFileName = "Transformado.png";
  const auto   defaultValue = static_cast<ScalarType>(2);

  using MatrixType = itk::Matrix<ScalarType, Dimension + 1, Dimension + 1>;
  MatrixType matrix;
  matrix[0][0] = std::cos(0.05);
  matrix[0][1] = std::sin(0.05);
  matrix[0][2] = 0.;

  matrix[1][0] = -matrix[0][1];
  matrix[1][1] = matrix[0][0];
  matrix[1][2] = 0.;

  matrix[2][0] = -10.;
  matrix[2][1] = -20.;
  matrix[2][2] = 1.;

  using PixelType = unsigned char;
  using ImageType = itk::Image<PixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  ImageType::ConstPointer input = reader->GetOutput();

  const ImageType::SizeType & size = input->GetLargestPossibleRegion().GetSize();

  using ResampleImageFilterType = itk::ResampleImageFilter<ImageType, ImageType>;
  ResampleImageFilterType::Pointer resample = ResampleImageFilterType::New();
  resample->SetInput(input);
  resample->SetReferenceImage(input);
  resample->UseReferenceImageOn();
  resample->SetSize(size);
  resample->SetDefaultPixelValue(defaultValue);

  constexpr unsigned int Radius = 3;
  using InterpolatorType = itk::WindowedSincInterpolateImageFunction<ImageType, Radius>;
  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  resample->SetInterpolator(interpolator);

  using TransformType = itk::AffineTransform<ScalarType, Dimension>;
  TransformType::Pointer transform = TransformType::New();

  // get transform parameters from MatrixType
  TransformType::ParametersType parameters(Dimension * Dimension + Dimension);
  for (unsigned int i = 0; i < Dimension; i++)
  {
    for (unsigned int j = 0; j < Dimension; j++)
    {
      parameters[i * Dimension + j] = matrix[i][j];
    }
  }
  for (unsigned int i = 0; i < Dimension; i++)
  {
    parameters[i + Dimension * Dimension] = matrix[i][Dimension];
  }
  transform->SetParameters(parameters);

  resample->SetTransform(transform);

  using WriterType = itk::ImageFileWriter<ImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(resample->GetOutput());
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject & error)
  {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// eof - 02_scale.cxx
