#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
		return -1;
	}
	Mat image,out1,out2,DoG;
	image = imread(argv[1], IMREAD_COLOR); // Read the file
	if (image.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	GaussianBlur(image, out1, Size(21,21), 0, 0, BORDER_DEFAULT);
	GaussianBlur(image, out2, Size(7, 7), 0, 0, BORDER_DEFAULT);
	DoG = out1-out2;
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", DoG); // Show our image inside it.
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
