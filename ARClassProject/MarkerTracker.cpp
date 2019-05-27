#include <iostream>
#include <iomanip>

#include <opencv2/core.hpp>    // include OpenCV core headers
#include <opencv2/imgproc.hpp> // include image processing headers
#include <opencv2/highgui.hpp> // include GUI-related headers


#define _USE_MATH_DEFINES
#include <math.h>

#include "MarkerTracker.h"
#include "PoseEstimation.h"

//////////// Turn this on for debug
// #define MARKER_TRACKER_DEBUG
////////////

void trackbarHandler( int pos, void* slider_value ) {
	*( (int*)slider_value ) = pos;
}

void bw_trackbarHandler(int pos, void* slider_value ) {
	*( (int*)slider_value ) = pos;
}

int MarkerTracker::subpixSampleSafe ( const cv::Mat &pSrc, const cv::Point2f &p )
{
	int x = int( floorf ( p.x ) );
	int y = int( floorf ( p.y ) );

	if ( x < 0 || x >= pSrc.cols  - 1 ||
		 y < 0 || y >= pSrc.rows - 1 )
		return 127;

	int dx = int ( 256 * ( p.x - floorf ( p.x ) ) );
	int dy = int ( 256 * ( p.y - floorf ( p.y ) ) );

	unsigned char* i = ( unsigned char* ) ( ( pSrc.data + y * pSrc.step ) + x );
	int a = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	i += pSrc.step;
	int b = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
	return a + ( ( dy * ( b - a) ) >> 8 );
}


// Draw a contour
void MarkerTracker::drawContour(cv::Mat &image_bgr, const Contour &contour_approx) {
	const bool kIsLineClosed = true;
	const cv::Scalar kEdgeColor(rand() & 255, rand() & 255, rand() & 255);
	const int kThickness = 4;
	cv::polylines(image_bgr, contour_approx, kIsLineClosed, kEdgeColor, kThickness);
}


//  A function to check if a contour is a good candidate
bool MarkerTracker::checkContourCondition(const Contour &contour_approx, const cv::Mat &image_bgr, const int &kNumOfCorners) {

	// Get a bounding rectangle of the approximated contour
	const cv::Rect bounding_rectangle = cv::boundingRect(contour_approx);
	const int marker_size = bounding_rectangle.area();

	// Filter bad contours
	const int kImageSize = image_bgr.rows*image_bgr.cols;
	const int kMarkerSizeMin = (int)(kImageSize*0.01);
	const int kMarkerSizeMax = (int)(kImageSize*0.99);
	const bool is_contour_valid = (marker_size > kMarkerSizeMin)
		&& (marker_size < kMarkerSizeMax)
		&& contour_approx.size() == kNumOfCorners
		&& cv::isContourConvex(contour_approx);

	return is_contour_valid;
}

void MarkerTracker::init()
{
	std::cout << "Startup\n";
#ifdef MARKER_TRACKER_DEBUG
	cv::namedWindow(kWinName1, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(kWinName2, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kWinName3, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kWinName4, 0 );
#endif
	cv::resizeWindow("Marker", 120, 120 );

	int max = 255;
	int slider_value = 100;
	cv::createTrackbar( "Threshold", kWinName2, &slider_value, 255, trackbarHandler, &slider_value);

	int bw_sileder_value = bw_thresh;
	cv::createTrackbar( "BW Threshold", kWinName2, &slider_value, 255, bw_trackbarHandler, &bw_sileder_value);
	
}

void MarkerTracker::cleanup() 
{
#ifdef MARKER_TRACKER_DEBUG
	cv::destroyWindow (kWinName1);
	cv::destroyWindow (kWinName2);
	cv::destroyWindow (kWinName3);
	cv::destroyWindow (kWinName4);
#endif // MARKER_TRACKER_DEBUG
	std::cout << "Finished\n";
}

void MarkerTracker::findMarker( cv::Mat &image_bgr, std::vector<Marker> &markers )
{
	const int image_height = image_bgr.rows;
	const int image_width = image_bgr.cols;
	bool isFirstStripe = true;
	bool isFirstMarker = true;

	cv::cvtColor(image_bgr, image_gray, cv::COLOR_BGR2GRAY);
	cv::threshold(image_gray, image_gray_filtered, thresh, 255, cv::THRESH_BINARY);

	// Find contours
	ContourList contours;
	ContourList contours_approx;
	ContourHierarchy hierarchy;
	cv::findContours(image_gray_filtered, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	for each (auto contour in contours)
	{
		// Approximate a contour
		const auto kEpsilon = 0.05*cv::arcLength(contour, true);
		Contour contour_approx;
		cv::approxPolyDP(contour, contour_approx, kEpsilon, true);

		// Get a bounding rectangle of the approximated contour
		const cv::Rect kRectangle = cv::boundingRect(contour_approx);
		const int kMarkerSize = kRectangle.area();


		const int kNumOfCorners = 4;
		const bool is_valid = checkContourCondition(contour_approx, image_bgr, kNumOfCorners);

		if (is_valid == false)
			continue;
		
#ifdef MARKER_TRACKER_DEBUG
		// Draw lines
		const bool isClosed = true;
		const cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
		const int thickness = 4;
		cv::polylines(image_bgr, contour_approx, isClosed, color, thickness);
#endif // MARKER_TRACKER_DEBUG

		cv::Vec4f line_parameters[4];

		const cv::Scalar kCIRCLE_COLOR(CV_RGB(0, 255, 0));
		for (int i = 0; i < kNumOfCorners; ++i)
		{
			const int kNumOfEdgePoints = 7;
#ifdef MARKER_TRACKER_DEBUG
			// Draw corner points
			const int kCircleSize = 5;
			cv::circle(image_bgr, contour_approx[i], kCircleSize, kCIRCLE_COLOR, cv::FILLED);
#endif // MARKER_TRACKER_DEBUG

			const cv::Point2d kEdgeDirectionVec((contour_approx[(i + 1) % kNumOfCorners] - contour_approx[i]) / (double)kNumOfEdgePoints);
			const double kEdgeDirectionVecNorm = cv::norm(kEdgeDirectionVec);

			// Added in Exercise 2 - Start *****************************************************************
			int stripe_length = (int)(0.8*kEdgeDirectionVecNorm);
			if (stripe_length < 5)
				stripe_length = 5;
			//make stripeLength odd (because of the shift in nStop)
			stripe_length |= 1;

			// Loop start/end index e.g. stripeLength = 5 --> from -2 to 2
			const int kStop = stripe_length >> 1;
			const int kStart = -kStop;

			const int kStripeWidth = 3;
			const cv::Size kStripeSize(kStripeWidth, stripe_length);
			//normalize vectors
			const cv::Point2f kStripeVecX(kEdgeDirectionVec / kEdgeDirectionVecNorm);
			const cv::Point2f kStripeVecY(-kStripeVecX.y, kStripeVecX.x);

			cv::Mat stripe_image(kStripeSize, CV_8UC1);
			// Added in Exercise 2 - End *******************************************************************

			// Array for edge point centers
			cv::Point2f edge_points[kNumOfEdgePoints - 1];
			for (int j = 1; j < kNumOfEdgePoints; ++j)
			{
				const cv::Point2d edge_point_d = cv::Point2d(contour_approx[i]) + (double)j*kEdgeDirectionVec;
				const cv::Point edge_point(edge_point_d);
				
#ifdef MARKER_TRACKER_DEBUG
				// Draw line delimeters
				cv::circle(image_bgr, edge_point, 2, CV_RGB(0, 0, 255), -1);
#endif // MARKER_TRACKER_DEBUG

				// Added in Exercise 2 - Start *****************************************************************
				
				// Construct a srtipe image
				for (int m = -1; m <= 1; ++m)
				{
					for (int n = kStart; n <= kStop; ++n)
					{
						cv::Point2f subpixel;
						subpixel.x = (float)((double)edge_point.x + ((double)m * kStripeVecX.x) + ((double)n * kStripeVecY.x));
						subpixel.y = (float)((double)edge_point.y + ((double)m * kStripeVecX.y) + ((double)n * kStripeVecY.y));
						
#ifdef MARKER_TRACKER_DEBUG
						cv::Point subpixel_int(subpixel);
						if (isFirstStripe)
							cv::circle(image_bgr, subpixel_int, 1, CV_RGB(255, 0, 255), -1);
						else
							cv::circle(image_bgr, subpixel_int, 1, CV_RGB(0, 255, 255), -1);
#endif // MARKER_TRACKER_DEBUG

						// Fetch subpixel value
						const int kSubpixelValue = subpixSampleSafe(image_gray, subpixel);

						const int kStripeX = m + 1; //add 1 to shift to 0..2
						const int kStripeY = n + (stripe_length >> 1); //add stripelenght>>1 to shift to 0..stripeLength
						stripe_image.at<uchar>(kStripeY, kStripeX) = (uchar)kSubpixelValue;
						///							*(iplStripe->imageData + h * iplStripe->widthStep  + w) =  pixel; //set pointer to correct position and safe subpixel intensity
					}
				}

				//use sobel operator on stripe
				// ( -1 , -2, -1 )
				// (  0 ,  0,  0 )
				// (  1 ,  2,  1 )
				std::vector<double> sobelValues(stripe_length - 2);
				///					double* sobelValues = new double[stripeLength-2];
				for (int n = 1; n < (stripe_length - 1); n++)
				{
					unsigned char* stripePtr = &(stripe_image.at<uchar>(n - 1, 0));
					///						unsigned char* stripePtr = ( unsigned char* )( iplStripe->imageData + (n-1) * iplStripe->widthStep );
					double r1 = -(double)stripePtr[0] - 2.0 * stripePtr[1] - (double)stripePtr[2];

					stripePtr += 2 * stripe_image.step;
					///						stripePtr += 2*iplStripe->widthStep;
					double r3 = (double)stripePtr[0] + 2.0 * stripePtr[1] + (double)stripePtr[2];
					sobelValues[n - 1] = -(r1 + r3);
				}

				double maxVal = -1;
				int maxIndex = 0;
				for (int n = 0; n < stripe_length - 2; ++n)
				{
					if (sobelValues[n] > maxVal)
					{
						maxVal = sobelValues[n];
						maxIndex = n;
					}
				}

				double y0, y1, y2; // y0 .. y1 .. y2
				y0 = (maxIndex <= 0) ? 0 : sobelValues[maxIndex - 1];
				y1 = sobelValues[maxIndex];
				y2 = (maxIndex >= stripe_length - 3) ? 0 : sobelValues[maxIndex + 1];
				//formula for calculating the x-coordinate of the vertex of a parabola, given 3 points with equal distances 
				//(xv means the x value of the vertex, d the distance between the points): 
				//xv = x1 + (d / 2) * (y2 - y0)/(2*y1 - y0 - y2)
				double pos = (y2 - y0) / (4 * y1 - 2 * y0 - 2 * y2); //d = 1 because of the normalization and x1 will be added later

				// This would be a valid check, too
				//if (std::isinf(pos)) {
				//	// value is infinity
				//	continue;
				//}

				if (pos != pos) {
					// value is not a number
					continue;
				}

				cv::Point2f edgeCenter; //exact point with subpixel accuracy
				int maxIndexShift = maxIndex - (stripe_length >> 1);

				//shift the original edgepoint accordingly
				edgeCenter.x = (float)((double)edge_point.x + (((double)maxIndexShift + pos) * kStripeVecY.x));
				edgeCenter.y = (float)((double)edge_point.y + (((double)maxIndexShift + pos) * kStripeVecY.y));

				// Added in Exercise 3 - Start *****************************************************************
#ifdef MARKER_TRACKER_DEBUG
				cv::Point p_tmp;
				p_tmp.x = (int)edgeCenter.x;
				p_tmp.y = (int)edgeCenter.y;
				cv::circle(image_bgr, p_tmp, 1, CV_RGB(0, 0, 255), -1);
#endif // MARKER_TRACKER_DEBUG

				edge_points[j - 1].x = edgeCenter.x;
				edge_points[j - 1].y = edgeCenter.y;
				// Added in Exercise 3 - End *******************************************************************

				if (isFirstStripe)
				{
#ifdef MARKER_TRACKER_DEBUG
					cv::Mat iplTmp;
					cv::resize(stripe_image, iplTmp, cv::Size(100, 300), 0.0, 0.0, cv::INTER_NEAREST);
					cv::imshow(kWindowNameStripe, iplTmp);//iplStripe );
#endif // MARKER_TRACKER_DEBUG
					isFirstStripe = false;
				}

			} // end of loop over edge points of one edge
// Added in Exercise 2 - End *******************************************************************

// Added in Exercise 3 - Start *****************************************************************
			// we now have an array of exact edge centers stored in "points"
			cv::Mat mat(cv::Size(1, 6), CV_32FC2, edge_points);
			cv::fitLine(mat, line_parameters[i], cv::DIST_L2, 0, 0.01, 0.01);
			// cvFitLine stores the calculated line in lineParams in the following way:

			cv::Point p1;
			p1.x = (int)line_parameters[i][2] - (int)(50.0*line_parameters[i][0]);
			p1.y = (int)line_parameters[i][3] - (int)(50.0*line_parameters[i][1]);
			cv::Point p2;
			p2.x = (int)line_parameters[i][2] + (int)(50.0*line_parameters[i][0]);
			p2.y = (int)line_parameters[i][3] + (int)(50.0*line_parameters[i][1]);
			
#ifdef MARKER_TRACKER_DEBUG
			const cv::Scalar kLineColor = CV_RGB(0, 255, 255);
			const int kLineThickness = 1;
			cv::line(image_bgr, p1, p2, kLineColor, kLineThickness, 8, 0);
#endif // MARKER_TRACKER_DEBUG
			// Added in Exercise 3 - End *******************************************************************

		} // end of loop over the 4 edges


// Added in Exercise 3 - Start *******************************************************************
			// so far we stored the exact line parameters and show the lines in the image
			// now we have to calculate the exact corners
		cv::Point2f corners[kNumOfCorners];

		for (int i = 0; i < kNumOfCorners; ++i)
		{
			int j = (i + 1) % 4;
			double x0, x1, y0, y1, u0, u1, v0, v1;
			x0 = line_parameters[i][2]; y0 = line_parameters[i][3];
			x1 = line_parameters[j][2]; y1 = line_parameters[j][3];

			u0 = line_parameters[i][0]; v0 = line_parameters[i][1];
			u1 = line_parameters[j][0]; v1 = line_parameters[j][1];

			// (x|y) = p + s * vec
			// s = Ds / D (see cramer's rule)
			// (x|y) = p + (Ds / D) * vec
			// (x|y) = (p * D / D) + (Ds * vec / D)
			// (x|y) = (p * D + Ds * vec) / D
			// (x|y) = a / c;
			double a = x1 * u0*v1 - y1 * u0*u1 - x0 * u1*v0 + y0 * u0*u1;
			double b = -x0 * v0*v1 + y0 * u0*v1 + x1 * v0*v1 - y1 * v0*u1;
			double c = v1 * u0 - v0 * u1;

			if (fabs(c) < 0.001) //lines parallel?
			{
				std::cout << "lines parallel" << std::endl;
				continue;
			}

			a /= c;
			b /= c;

			//exact corner
			corners[i].x = (float)a;
			corners[i].y = (float)b;
#ifdef MARKER_TRACKER_DEBUG
			cv::Point p;
			p.x = (int)corners[i].x;
			p.y = (int)corners[i].y;

			cv::circle(image_bgr, p, 5, CV_RGB(255, 255, 0), -1);
#endif // MARKER_TRACKER_DEBUG
		} //finished the calculation of the exact corners

		cv::Point2f targetCorners[kNumOfCorners];
		targetCorners[0].x = -0.5; targetCorners[0].y = -0.5;
		targetCorners[1].x = 5.5; targetCorners[1].y = -0.5;
		targetCorners[2].x = 5.5; targetCorners[2].y = 5.5;
		targetCorners[3].x = -0.5; targetCorners[3].y = 5.5;

		//create and calculate the matrix of perspective transform
		cv::Mat projMat(cv::Size(3, 3), CV_32FC1);
		projMat = cv::getPerspectiveTransform(corners, targetCorners);
		///			cv::warpPerspectiveQMatrix ( corners, targetCorners, projMat);

					//create image for the marker
		//			markerSize.width  = 6;
		//			markerSize.height = 6;
		cv::Mat iplMarker(cv::Size(6, 6), CV_8UC1);

		//change the perspective in the marker image using the previously calculated matrix
		cv::warpPerspective(image_gray, iplMarker, projMat, cv::Size(6, 6));
		

		const int bw_thresh = 55;
		cv::threshold(iplMarker, iplMarker, bw_thresh, 255, cv::THRESH_BINARY);
		//now we have a B/W image of a supposed Marker



		// check if border is black
		int code = 0;
		for (int i = 0; i < 6; ++i)
		{
			//int pixel1 = ((unsigned char*)(iplMarker->imageData + 0*iplMarker->widthStep + i))[0]; //top
			//int pixel2 = ((unsigned char*)(iplMarker->imageData + 5*iplMarker->widthStep + i))[0]; //bottom
			//int pixel3 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep))[0]; //left
			//int pixel4 = ((unsigned char*)(iplMarker->imageData + i*iplMarker->widthStep + 5))[0]; //right
			int pixel1 = iplMarker.at<uchar>(0, i);
			int pixel2 = iplMarker.at<uchar>(5, i);
			int pixel3 = iplMarker.at<uchar>(i, 0);
			int pixel4 = iplMarker.at<uchar>(i, 5);
			if ((pixel1 > 0) || (pixel2 > 0) || (pixel3 > 0) || (pixel4 > 0))
			{
				code = -1;
				break;
			}
		}

		if (code < 0) {
			continue;
		}

		//copy the BW values into cP
		int cP[4][4];
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				cP[i][j] = iplMarker.at<uchar>(i + 1, j + 1);
				cP[i][j] = (cP[i][j] == 0) ? 1 : 0; //if black then 1 else 0
			}
		}

		//save the ID of the marker
		int codes[4];
		codes[0] = codes[1] = codes[2] = codes[3] = 0;
		for (int i = 0; i < 16; i++)
		{
			int row = i >> 2;
			int col = i % 4;

			codes[0] <<= 1;
			codes[0] |= cP[row][col]; // 0‹

			codes[1] <<= 1;
			codes[1] |= cP[3 - col][row]; // 90‹

			codes[2] <<= 1;
			codes[2] |= cP[3 - row][3 - col]; // 180‹

			codes[3] <<= 1;
			codes[3] |= cP[col][3 - row]; // 270‹
		}

		if ((codes[0] == 0) || (codes[0] == 0xffff)) {
			continue;
		}

		//account for symmetry
		code = codes[0];
		int angle = 0;
		for (int i = 1; i < 4; ++i)
		{
			if (codes[i] < code)
			{
				code = codes[i];
				angle = i;
			}
		}


		if (isFirstMarker)
		{
#ifdef MARKER_TRACKER_DEBUG
			cv::Mat iplTmp;
			cv::resize(iplMarker, iplTmp, cv::Size(100, 100), 0.0, 0.0, cv::INTER_NEAREST);
			cv::imshow(kWindowMarker, iplTmp);
#endif // MARKER_TRACKER_DEBUG
			isFirstMarker = false;
		}

		// Added in Exercise 4 - Start *****************************************************************
		//correct the order of the corners
		if (angle != 0)
		{
			cv::Point2f corrected_corners[4];
			for (int i = 0; i < 4; i++)	corrected_corners[(i + angle) % 4] = corners[i];
			for (int i = 0; i < 4; i++)	corners[i] = corrected_corners[i];
		}

		// transfer screen coords to camera coords
		for (int i = 0; i < 4; i++)
		{
			corners[i].x -= image_width/2; //here you have to use your own camera resolution (x) * 0.5
			corners[i].y = -corners[i].y + image_height/2; //here you have to use your own camera resolution (y) * 0.5
		}
		Marker marker;
		marker.code = code;
		estimateSquarePose(marker.resultMatrix, (cv::Point2f*)corners, (float) kMarkerSideLength);
		markers.push_back(marker);
		
#ifdef MARKER_TRACKER_DEBUG
		printf("Found: %04x\n", code);
		//this part is only for printing
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				std::cout << std::setw(6);
				std::cout << std::setprecision(4);
				std::cout << marker.resultMatrix[4 * i + j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
		float x, y, z;
		x = marker.resultMatrix[3];
		y = marker.resultMatrix[7];
		z = marker.resultMatrix[11];
		std::cout << "length: " << sqrt(x*x + y * y + z * z) << "\n";
		std::cout << "\n";
#endif // MARKER_TRACKER_DEBUG
		// Added in Exercise 5 - End *****************************************************************

	} // end of loop over countour candidates

#ifdef MARKER_TRACKER_DEBUG
	cv::imshow(kWinName1, image_bgr);
	cv::imshow(kWinName2, image_gray_filtered);
	int key = cv::waitKey(10);
#endif //MARKER_TRACKER_DEBUG
	

	//int key = cv::waitKey(-1);
	//if (key == 27) exit(0);

//	glutPostRedisplay();
}
