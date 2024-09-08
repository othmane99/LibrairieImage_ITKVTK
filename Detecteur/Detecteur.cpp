#include<iostream>
#include<itkImage.h>
#include<itkImageFileReader.h>
#include<itkCastImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkImageFileWriter.h>
#include<itkRescaleIntensityImageFilter.h>

int main(int argc, char* argv[])
{
	/*using UCharPixelType = unsigned char;
	using FloatPixelType = float;
	using UCharImageType = itk::Image<UCharPixelType, 2>;
	using FloatImageType = itk::Image<FloatPixelType, 2>;

	// image file reader

	using FileReaderType = itk::ImageFileReader<UCharImageType>;
	FileReaderType::Pointer reader = FileReaderType::New();
	reader->SetFileName(argv[1]);


	//  cast image uchar  float

	using UChar2FloatCastFilterType = itk::CastImageFilter < UCharImageType, FloatImageType>;
	UChar2FloatCastFilterType::Pointer uChar2FloatFilter = UChar2FloatCastFilterType::New();
	uChar2FloatFilter->SetInput(reader->GetOutput());


	//sobel detection
	using SobelFilterType = itk::SobelEdgeDetectionImageFilter<FloatImageType, FloatImageType>;
	SobelFilterType::Pointer sobelFilter = SobelFilterType::New();
	sobelFilter->SetInput(uChar2FloatFilter->GetOutput());



	//rescale filter output to cope with unsigned char

	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, UCharImageType>;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(sobelFilter->GetOutput());
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	//  image cast  float ucha
	using Float2UCharCastFilterType = itk::CastImageFilter<FloatImageType, UCharImageType>;
	Float2UCharCastFilterType::Pointer float2UCharFilter = Float2UCharCastFilterType::New();
	float2UCharFilter->SetInput(sobelFilter->GetOutput());


	// image writer
	using FileWriterType = itk::ImageFileWriter<UCharImageType>;
	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetInput(float2UCharFilter->GetOutput());
	writer->SetFileName("result.jpg");
	writer->Update();  */

	using UCharPixelType = unsigned char;
	using FloatPixelType = float;
	using UCharImageType = itk::Image<UCharPixelType, 2>;
	using FloatImageType = itk::Image<FloatPixelType, 2>;

	//Image file reader
	using FileReaderType = itk::ImageFileReader<UCharImageType>;
	FileReaderType::Pointer reader = FileReaderType::New();
	reader->SetFileName(argv[1]);

	//Cast image uchar -> float
	using UChar2FloatCastFilterType = itk::CastImageFilter<UCharImageType, FloatImageType>;
	UChar2FloatCastFilterType::Pointer uChar2FloatFilter = UChar2FloatCastFilterType::New();
	uChar2FloatFilter->SetInput(reader->GetOutput());

	//Sobel edge detection filter
	using SobelFilterType = itk::SobelEdgeDetectionImageFilter<FloatImageType, FloatImageType>;
	SobelFilterType::Pointer sobelFilter = SobelFilterType::New();
	sobelFilter->SetInput(uChar2FloatFilter->GetOutput());


	/*using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, UCharImageType>;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(sobelFilter->GetOutput());
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);*/

	//Image cast float -> uchar
	using Float2UCharCastFilterType = itk::CastImageFilter<FloatImageType, UCharImageType>;
	Float2UCharCastFilterType::Pointer float2UCharFilter = Float2UCharCastFilterType::New();
	float2UCharFilter->SetInput(sobelFilter->GetOutput());

	//Image file writer
	using FileWriterType = itk::ImageFileWriter<UCharImageType>;
	FileWriterType::Pointer writer = FileWriterType::New();
	//writer->SetInput(float2UCharFilter->GetOutput());
	writer->SetInput(float2UCharFilter->GetOutput());
	writer->SetFileName("result1.jpg");
	writer->Update();




	return 0;
}


