/*
 * main.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: yildbs
 */

#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

int main()
{

	std::cout << "Hello " << std::endl;

	cv::Mat image = cv::imread("/home/yildbs/Data/LicensePlate/image_1.jpg");
	cv::imshow("origin", image);

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

//	cv::Mat binary;
//	cv::threshold(gray, binary, )

	cv::Mat blurred1, blurred2;
	cv::GaussianBlur(image, blurred1, cv::Size(3, 3), 0);
	cv::GaussianBlur(image, blurred2, cv::Size(27, 27), 0);
	cv::imshow("blurred1", blurred1);
	cv::imshow("blurred2", blurred2);

	cv::Mat dog = blurred1 - blurred2;
	cv::cvtColor(dog, dog, cv::COLOR_RGB2GRAY);
	cv::imshow("DoG", dog);
	std::cout << "dog.channels() : " << dog.channels() << std::endl;

	cv::Mat binary;
	cv::threshold(dog, binary, 10, 255, cv::THRESH_BINARY);
	cv::imshow("binary", binary);

	cv::Mat adaptive;
	cv::adaptiveThreshold(dog, adaptive, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 0);
	cv::imshow("adaptive", adaptive);

	cv::Mat post_proc = adaptive;

	cv::dilate(post_proc, post_proc, cv::Mat());
	cv::erode(post_proc, post_proc, cv::Mat());
	cv::erode(post_proc, post_proc, cv::Mat());
	cv::dilate(post_proc, post_proc, cv::Mat());
	cv::imshow("post_proc", post_proc);


	//TODO
	cv::Mat contours;
	cv::findContours( post_proc, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// Draw contours
	Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
	{
	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	}



	cv::waitKey(0);
}




