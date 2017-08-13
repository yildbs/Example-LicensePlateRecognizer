/*
 * main.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: yildbs
 */

#include <iostream>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>


cv::Rect FindRect(std::vector<cv::Point> contour)
{
	cv::Rect outer;
	int top, left, bot, right;
	top = INT_MAX;
	left = INT_MAX;
	bot = 0;
	right = 0;
	for(auto& point : contour){
		if(point.x < left){
			left = point.x;
		}else if(point.x > right){
			right = point.x;
		}
		if(point.y < top){
			top = point.y;
		}else if(point.y > bot){
			bot = point.y;
		}
	}
	outer.x = left;
	outer.y = top;
	outer.width = right - left;
	outer.height = bot - top;
	return outer;
}

std::vector<cv::Rect>& FilterRects(cv::Size min, cv::Size max, std::vector<cv::Rect>& rects)
{
	std::vector<cv::Rect> filtered;
	for(auto& rect : rects){
		if(rect.width < min.width){
			continue;
		}
		if(rect.height < min.height){
			continue;
		}
		if(rect.width > max.width){
			continue;
		}
		if(rect.height > max.height){
			continue;
		}
		filtered.push_back(rect);
	}
	rects = filtered;
	return rects;
}


void FindSequencedRect(std::vector<cv::Rect>& rects)
{
	int num_rects = rects.size();
	for(int first=0;first<num_rects;first++){
		for(int second=first+1;second<num_rects;second++){

		}
	}
}

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
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	/// Find contours
	cv::findContours( post_proc, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

	/// Draw contours
	cv::Mat contour;
	image.copyTo(contour);

	printf("contours.size() : %d\n", contours.size());

	std::vector<cv::Rect> rects;

	for(auto& points : contours){
		auto rect = FindRect(points);
		rects.push_back(rect);
	}

	FilterRects(cv::Size(20, 20), cv::Size(100, 100), rects);
	printf("rects.size() : %d\n", rects.size());


	for(auto& rect : rects){
		cv::rectangle(contour, rect, cv::Scalar(255, 255, 255), 1, 8, 0);
	}


	cv::imshow("contour", contour);




	cv::waitKey(0);
}




