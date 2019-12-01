#include "iostream"
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "mpi.h"

#define OCTAVE_SIZE 4

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	vector<Mat> octave, DoG;
	Mat src, blur, diff;
	int myid, numprocs, namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);

	printf("Process %d running on %s \n", myid, processor_name);

	// Loads an image
	src = imread("knifecrab.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//normalize(src, src, 0, 255, NORM_MINMAX);

	for (int i=0; i<myid; i++)
	{
		pyrDown(src, src, Size(src.cols/2, src.rows/2));
	}
	printf("Process %d, Octave resolution: %d x %d\n", myid, src.cols, src.rows);

	//create the blur levels
	octave.push_back(src.clone());
	for(int i=0; i<OCTAVE_SIZE-1; i++)
	{
		GaussianBlur(src, blur, Size((12*(i+1))+1, (12*(i+1))+1), 0, 0, BORDER_DEFAULT);
		
		octave.push_back(blur.clone());
	}

	//create DoGs
	for(vector<Mat>::iterator iter = octave.begin()+1; iter != octave.end(); ++iter)
	{
		diff = *(iter-1) - *iter;
		DoG.push_back(diff.clone());
	}

	//Slave work
	if(myid!=0)
	{
		//send DoGs to root
		int count = 1;
		for(vector<Mat>::iterator iter=DoG.begin();iter!=DoG.end();++iter)
		{
			printf("sending DoG # %d from Process %d\n", count, myid);
			MPI_Send((*iter).data, src.rows*src.cols, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
			count++;
		}
	}

	//Master work
	if(myid==0)
	{//receiving each DoG
		//each octave
		for(int i=0; i<numprocs-1; i++)
		{	
			printf("Root waiting to receive DoGs from process %d\n", i+1);
			//each DoG:j in Octave:i
			for(int j=0;j<OCTAVE_SIZE-1;j++)
			{
				Mat dummy = Mat(src.rows/(2*(i+1)), src.cols/(2*(i+1)), CV_8UC1);
				MPI_Recv(dummy.data, 
					src.rows/(2*(i+1)) * src.cols/(2*(i+1)), 
					MPI_BYTE, 
					i+1, 
					0, 
					MPI_COMM_WORLD, 
					&status);
				DoG.push_back(dummy.clone());
				printf("Root received DoG # %d from Process %d \n", 
					j+1, 
					i+1);
			}
		}
		
		//write all the DoGs
		for(int i=0;i<numprocs;i++)
		{
			stringstream oct_num;
			oct_num << i;
			vector<Mat>::iterator iter = DoG.begin()+(i*(OCTAVE_SIZE-1));
			for(int j=0;j<OCTAVE_SIZE-1;j++)
			{
				stringstream DoG_num;
        		        DoG_num << j;
                		string save_name = "Oct" + oct_num.str() + "DoG" + DoG_num.str() + ".jpg";
                		imwrite(save_name, *iter);
				++iter;
			}
		}
	}

	MPI_Finalize();
}
