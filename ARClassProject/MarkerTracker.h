#ifndef MARKER_TRACKER_H
#define MARKER_TRACKER_H

#include <opencv2/core.hpp>    // include OpenCV core headers

using Pose = float[16];
class Marker{
public:
	int code;
	Pose resultMatrix;
	void print_matrix() {
		const auto &mat = resultMatrix;
		const int kSize = 4;
		for (int r = 0; r < kSize; ++r) {
			for (int c = 0; c < kSize; ++c) {
				std::cout << mat[c + kSize * (c - 1)] << " ";
			}
			std::cout << std::endl;
		}
	}
};


// Define types
using Contour = std::vector<cv::Point>;
using ContourList = std::vector<Contour>;
using ContourHierarchy = std::vector<cv::Vec4i>;

const std::string kWinName1 = "Exercise 8 - Original Image";
const std::string kWinName2 = "Exercise 8 - Converted Image";
const std::string kWinName3 = "Exercise 8 - Stripe Image";
const std::string kWinName4 = "Exercise 8 - Marker";
const std::string kWindowNameStripe = "Exercise 2 - Stripe Image";
const std::string kWindowMarker = "Marker Image";


class MarkerTracker{
public:
	MarkerTracker(double kMarkerSideLength_) 
		:	thresh(100),
		bw_thresh(100),
		kMarkerSideLength(kMarkerSideLength_)
	{
		init();
	}
	MarkerTracker(double kMarkerSideLength_, int thresh_, int bw_thresh_) 
		:	thresh(thresh_),
		bw_thresh(bw_thresh_),
		kMarkerSideLength(kMarkerSideLength_)
	{
		init();
	}
	~MarkerTracker(){
		cleanup();
	}
	void findMarker( cv::Mat &img_bgr, std::vector<Marker> &markers );
//	void findMarker( cv::Mat &img_bgr, float resultMatrix[16] );
protected:
	void init( );
	void cleanup( );
	
	int subpixSampleSafe(const cv::Mat &pSrc, const cv::Point2f &p);
	bool checkContourCondition(const Contour &contour_approx, const cv::Mat &image_bgr, const int &kNumOfCorners);
	void drawContour(cv::Mat &image_bgr, const Contour &contour_approx);


	//camera settings
	const double kMarkerSideLength; // Marker side length [m]

	cv::Mat image_gray;
	cv::Mat image_gray_filtered;

	int thresh; // Threshold (gray to mono)
	int bw_thresh; // threshold for (gray maker to ID image)

};

#endif // MARKER_TRACKER_H
