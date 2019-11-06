#include "iostream"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace std;
using namespace cv;
const char* window_name = "Pyramids Demo";
int main(int argc, char** argv)
{
	cout << "\n Zoom In-Out demo \n "
		"------------------  \n"
		" * [i] -> Zoom in   \n"
		" * [o] -> Zoom out  \n"
		" * [g] -> Gaussian Blur \n"
		" * [b] -> Mean Blur \n"
		" * [s] -> Save Current Image \n"
		" * [d] -> Current Image - Saved Image \n"
		" * [r] -> Reset Image \n"
		" * [ESC] -> Close program \n" << endl;
	const char* filename = argc >= 2 ? argv[1] : "messi5.jpg";
	// Loads an image
	Mat src = imread(samples::findFile(filename));
	Mat save;
	// Check if image is loaded fine
	if (src.empty()){
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default chicky_512.png] \n");
		return EXIT_FAILURE;
	}
	for (;;)
	{
		imshow(window_name, src);
		char c = (char)waitKey(0);
		if (c == 27)
		{
			break;
		}
		else if (c == 'i')
		{
			pyrUp(src, src, Size(src.cols * 2, src.rows * 2));
			printf("** Zoom In: Image x 2 \n");
		}
		else if (c == 'o')
		{
			pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
			printf("** Zoom Out: Image / 2 \n");
		}
		else if (c == 'g')
		{
			GaussianBlur(src, src, Size(9, 9), 0, 0, BORDER_DEFAULT);
			printf("** Gaussian Blur With 9x9 Kernel\n");
		}
		else if (c == 'b')
		{
			blur(src, src, Size(9, 9), Point(-1, -1), BORDER_DEFAULT);
			printf("** Mean Blur With 9x9 Kernel\n");
		}
		else if (c == 's')
		{
			src.copyTo(save);
			printf("** Image Stored\n");
		}
		else if (c == 'd')
		{
			if (!save.empty())
			{
				resize(save, save, Size(src.size().width, src.size().height));
				src = src - save;
			}
			printf("** Difference\n");
		}
		else if (c == 'r')
		{
			src = imread(samples::findFile(filename));
			printf("** Image Reset\n");
		}
	}
	return EXIT_SUCCESS;
}
