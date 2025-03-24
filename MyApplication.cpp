
#include "Utilities.h"


// Ground truth for pedestrian crossings.  Each row contains
// 1. the image number (PC?.jpg)
// 2. the coordinates of the line at the top of the pedestrian crossing (left column, left row, right column, right row)
// 3. the coordinates of the line at the bottom of the pedestrian crossing (left column, left row, right column, right row)
int pedestrian_crossing_ground_truth[][9] = {
	{ 10,0,132,503,113,0,177,503,148},
	{ 11,0,131,503,144,0,168,503,177},
	{ 12,0,154,503,164,0,206,503,213},
	{ 13,0,110,503,110,0,156,503,144},
	{ 14,0,95,503,104,0,124,503,128},
	{ 15,0,85,503,91,0,113,503,128},
	{ 16,0,65,503,173,0,79,503,215},
	{ 17,0,43,503,93,0,89,503,146},
	{ 18,0,122,503,117,0,169,503,176},
	{ 19,0,0,0,0,0,0,0,0}, //added by me (for image without ground truth)
	{ 20,0,157,503,131,0,223,503,184},
	{ 21,0,140,503,136,0,190,503,183},
	{ 22,0,114,503,97,0,140,503,123},
	{ 23,0,133,503,122,0,198,503,186},
	{ 24,0,107,503,93,0,146,503,118},
	{ 25,0,58,503,164,0,71,503,204},
	{ 26,0,71,503,131,0,106,503,199},
	{ 27,0,138,503,151,0,179,503,193},
	{ 28,0,0,0,0,0,0,0,0}, //added by me (for image without ground truth)
	{ 29,0,0,0,0,0,0,0,0} //added by me (for image without ground truth)
};

void MyApplication()
{
	for (int image_index = 10; (image_index <= 29); image_index++) 
	{
		char filename[200];
		sprintf(filename, "Media/PC%d.jpg", image_index);
		Mat original_image;
		original_image = imread(filename, -1);

		Mat smoothed_image; //smoothed image
		medianBlur(original_image, smoothed_image, 3); 
		


		imshow("Original Image - " + std::string(filename), original_image); 

		imshow("Smoothed Image - " + std::string(filename), smoothed_image); // Display the smoothed image in a different window


		
		
		
		Mat grayscale_image; //greyscaled image
		Mat binary_image; //binary image
		cvtColor(smoothed_image, grayscale_image, COLOR_BGR2GRAY);
		
		

		threshold(grayscale_image, binary_image, 182,255, 0); //binary thresholding
		imshow("Binary Thresholded Image - " + std::string(filename), binary_image);
		
		Mat morph_image; //image after performing open operation
		Mat structuring_element(5, 5, CV_8U, Scalar(1));
		morphologyEx(binary_image, morph_image, MORPH_OPEN, structuring_element);
		imshow("Morph - " + std::string(filename), morph_image); 
		
		Mat canny_edged_image; //canny edged image
		Canny(morph_image, canny_edged_image, 50, 150);
		imshow("Canny Edges - " + std::string(filename), canny_edged_image);
		
		


		vector<Vec2f> hough_lines;
		HoughLines(canny_edged_image, hough_lines, 1, PI / 180, 80);
		for (size_t i = 0; i < hough_lines.size(); i++) {
			
			
			float rho = hough_lines[i][0], theta = hough_lines[i][1];
			if (theta < 1.8 && theta > 1.2) { //to avoid having inclined lines
				Point pt1, pt2;
				double a = cos(theta), b = sin(theta);
				double x0 = a * rho, y0 = b * rho;
				pt1.x = cvRound(x0 + 1000 * (-b));
				pt1.y = cvRound(y0 + 1000 * (a));
				pt2.x = cvRound(x0 - 1000 * (-b));
				pt2.y = cvRound(y0 - 1000 * (a));
				line(original_image, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);  // Draw red lines (this represents what my approach found and NOT the ground truth)
			}
		}
		// Below is code for drawing ground truth
		for (int row = 0; row < 19 ; row++) {
			if (pedestrian_crossing_ground_truth[row][0] == image_index) {
				// Extract the ground truth coordinates for upper and lower pedestrian crossing lines
				Point upper_left(pedestrian_crossing_ground_truth[row][1], pedestrian_crossing_ground_truth[row][2]);
				Point upper_right(pedestrian_crossing_ground_truth[row][3], pedestrian_crossing_ground_truth[row][4]);
				Point lower_left(pedestrian_crossing_ground_truth[row][5], pedestrian_crossing_ground_truth[row][6]);
				Point lower_right(pedestrian_crossing_ground_truth[row][7], pedestrian_crossing_ground_truth[row][8]);

				// Draw the ground truth upper and lower lines in green
				line(original_image, upper_left, upper_right, Scalar(0, 255, 0), 2, LINE_AA);  // Green line for ground truth upper line
				line(original_image, lower_left, lower_right, Scalar(0, 255, 0), 2, LINE_AA);  // Green line for ground truth lower line

				break;  
			}
		}

		
		imshow("Hough lines - " + std::string(filename), original_image);
	
		
		char cha = cv::waitKey();


		cv::destroyAllWindows();
	}

}

