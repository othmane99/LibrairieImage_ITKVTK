#include<iostream>
#include<itkImage.h>
#include<itkImageFileReader.h>
#include<itkCastImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkImageFileWriter.h>
#include<itkImageRegionIterator.h>
#include<itkImageRegionIteratorWithIndex.h>
#include<itkNeighborhood.h>
#include<itkNeighborhoodInnerProduct.h>
#include<itkImageRegionIterator.h>


int main(int argc, char* argv[])
{
	using CharPixelType = char;
	using UCharPixelType = unsigned char;
	using FloatPixelType = float;
	using UCharImageType = itk::Image<UCharPixelType, 2>;
	using FloatImageType = itk::Image<FloatPixelType, 2>;


	//Allocation d'une nouvelle image de type UCharImageType

	UCharImageType::Pointer image = UCharImageType::New();
	//UCharImageType::IndexType start{ 0,0 };
	//UCharImageType::SizeType{ 512,512 };
	//UCharImageType::RegionType region(start, size;
	//image->SetRegions(region);
	image->SetRegions({ 215,215 });
	image->Allocate();

	// Fil image with zero 

	image->FillBuffer(0);

	//Display a white horizontal line in the middle of the  image 
	UCharImageType::RegionType region{ image->GetLargestPossibleRegion() };
	UCharImageType::SizeType size{ region.GetSize() };
	UCharImageType::SizeValueType width{ size[0] }, height{ size[1] };
	UCharImageType::IndexType index;
	index[1] = height / 2;
	for (index[0] = 0; index[0] < width; ++index[0])
	{
		image->SetPixel(index, 255);
	}

	//  draw a white disk
	const UCharImageType::SizeValueType radius{ width / 4 };
	
	for (index[0] = 0; index[0] < width; ++index[0])
		for (index[1] = 0; index[1] < height; ++index[1])
		{
			double dx{ (double)index[0] - width / 2 }, dy{ (double)index[1] - height / 2 };
			if (dx * dx + dy * dy < radius * radius)
				image->SetPixel(index, 255);
			else
				image->SetPixel(index, 0);
		}

	using FileWriterType = itk::ImageFileWriter<UCharImageType>;
	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetInput(image);
	writer->SetFileName("disk.jpg");
	writer->Update();


	// compute  disk negative image  with itterator
	itk::ImageRegionIterator<UCharImageType> it(image, image->GetLargestPossibleRegion());
	it.GoToBegin();
	while (!it.IsAtEnd())
	{
		it.Set(255 - it.Get());
		++it;
	}


	writer->SetInput(image);
	writer->SetFileName("disk_it.jpg");
	writer->Update();



	// draw white disque with an iterator with index 
	itk::ImageRegionIteratorWithIndex<UCharImageType> itIndex(image, image->GetLargestPossibleRegion());
	itIndex.GoToBegin();
	while (!itIndex.IsAtEnd())
	{
		UCharImageType::IndexType  index{ itIndex.GetIndex() };
		double dx{ (double)index[0] - width / 2 }, dy{ (double)index[1] - height / 2 };
		if (dx * dx + dy * dy < radius * radius)
			itIndex.Set(255);
		else
			itIndex.Set(0);
		++itIndex;
	}


	writer->SetInput(image);
	writer->SetFileName("disk_itIndex.jpg");
	writer->Update();


	//n*n mean filter 
	UCharImageType::Pointer output = UCharImageType::New();
	output->SetRegions(image->GetLargestPossibleRegion());
	output->Allocate();

	const UCharImageType::SizeType N{ 1,1 };
	itk::NeighborhoodIterator<UCharImageType> itNeighbor(N, image, image->GetLargestPossibleRegion());
	itk::ImageRegionIterator<UCharImageType> itOutput(output, output->GetLargestPossibleRegion());
	itNeighbor.GoToBegin();
	itOutput.GoToBegin();
	while (!itNeighbor.IsAtEnd())
	{
		int sum{};
		for (int i{}; i < itNeighbor.Size(); ++i)
		{
			sum += itNeighbor.GetPixel(i);
		}
		itOutput.Set(sum / itNeighbor.Size());
		++itNeighbor;
		++itOutput;
	}

	writer->SetInput(output);
	writer->SetFileName("mean.jpg");
	writer->Update();


	//  compute sobel  edge detector

	FloatImageType::Pointer tmp = FloatImageType::New();
	tmp->SetRegions(image->GetLargestPossibleRegion());
	tmp->Allocate();
	itk::ImageRegionIterator <FloatImageType> itTmp(tmp, tmp->GetLargestPossibleRegion());



	using SobelOperatorType = itk::SobelOperator<CharPixelType, 2>;
	SobelOperatorType sobelOperator;
	//itk::SobelOperator<CharPixelType, 2> sobelOperator;
	sobelOperator.SetDirection(0);
	sobelOperator.CreateToRadius(N);

	for (int i{}; i < sobelOperator.Size(); ++i)
		std::cout << i << " -> " << (int)sobelOperator.GetElement(i) << '\n';

	itk::NeighborhoodInnerProduct<UCharImageType, CharPixelType, FloatPixelType> innerProduct;

	itNeighbor.GoToBegin();
	itTmp.GoToBegin();
	while (!itNeighbor.IsAtEnd())
	{
		itTmp.Set(innerProduct.Compute(itNeighbor, sobelOperator));
		++itNeighbor;
		++itTmp;
	}

	//compute min and max of tmp image 
	FloatPixelType minTmp{}, maxTmp{};
	itTmp.GoToBegin();
	while (!itTmp.IsAtEnd())
	{
		if (itTmp.Get() < minTmp) minTmp = itTmp.Get();
		else if (itTmp.Get() > maxTmp) maxTmp = itTmp.Get();
		++itTmp;
	}


	//  convert float image to unsigned char output image 
	itTmp.GoToBegin();
	itOutput.GoToBegin();

	while (!itTmp.IsAtEnd())
	{

		itOutput.Set(255 * (itTmp.Get() - minTmp) / (maxTmp - minTmp));
		++itTmp;
		++itOutput;
	}



	writer->SetInput(output);
	writer->SetFileName("sobel.png");
	writer->Update();

	return 0;
}