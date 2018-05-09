#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <cmath>

#define SIZE 182
#define START 0
#define CLOCKWS 1

#define SELECTED_CELL 4
#define CELL_EDGE 45
#define CELL_EDGE_NUM 4
#define CELL_NUM 16
#define DIMENSION 256

cv::Mat MakeLBPatterns(cv::Mat mImage)
{
	cv::Mat cmNewImage(SIZE-2, SIZE-2, CV_8UC1);

	char nFilter[9] = { pow(2, (START + 7) % 8), pow(2, (START + 6) % 8), pow(2, (START + 5) % 8),
		pow(2, (START) % 8), 0, pow(2, (START + 4) % 8), pow(2, (START + 1) % 8), pow(2, (START + 2) % 8), pow(2, (START + 3) % 8) };

	cv::Mat cmFilter = cv::Mat(3, 3, CV_8UC1, nFilter);

	std::cout << cmFilter << std::endl;


	for (int nROW = 1; nROW < SIZE - 1; nROW++)
	{
		for (int nCOL = 1; nCOL < SIZE - 1; nCOL++)
		{


			if (nROW == 100 && nCOL == 100)
			{
				for (int i = nROW - 1; i < nROW + 2; i++)
				{
					for (int j = nCOL - 1; j < nCOL + 2; j++)
					{
						printf("%d, ", mImage.at<uchar>(i, j));
					}
					printf("\n");
				}
			}




			cv::Mat cmTEMP;
			cmTEMP = cv::Mat::zeros(3, 3, CV_8UC1);


			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (mImage.at<uchar>(nROW - 1 + i, nCOL - 1 + j) < mImage.at<uchar>(nROW, nCOL))
					{

						cmTEMP.at<uchar>(i, j) = 0;
					}
					else
					{
						cmTEMP.at<uchar>(i, j) = 1;
					}
				}
			}
			
			
			int nLBPtmp = cmTEMP.dot(cmFilter);

			if (nROW == 100 && nCOL == 100)
			{
				std::cout << cmTEMP << std::endl;
				std::cout << nLBPtmp << std::endl;
			}
			


			cmNewImage.at<uchar>(nROW - 1, nCOL - 1) = nLBPtmp;




		}
	}


	return cmNewImage;

}

void CalcCombination(int n, int r, int *arr, int sz, std::vector< std::vector<int> > *vComb, int *nCounter)
{
	for (int i = n; i >= r; i--)
	{
		arr[r - 1] = i;

		if (r > 1)
		{
			CalcCombination(i - 1, r - 1, arr, sz, vComb, nCounter);
		}
		else
		{
			//(*vComb)[*nCounter].resize(sz);

			for (int i = 0; i < sz; i++)
			{
				//std::cout << arr[i] << " ";
				(*vComb)[*nCounter].push_back(arr[i]);
			}
			(*nCounter)++;
			//std::cout << std::endl;

		}
	}
}

std::vector< std::vector<int> > MakeFeatures(cv::Mat mLBPimg, std::vector< std::vector<int> > vComb, int nCombCNT)
{
	std::vector< std::vector<int> > vFeat;
	vFeat.resize(nCombCNT);
	for (int i = 0; i < nCombCNT; i++)
	{
		vFeat[i].resize(256);
	}
	
	for (int i = 0; i < nCombCNT; i++)
	{
		for (int j = 0; j < SELECTED_CELL; j++)
		{
			int nROW = (vComb[i][j]-1) / CELL_EDGE_NUM;
			int nCOL = (vComb[i][j]-1) % CELL_EDGE_NUM;

			for (int nR = nROW * CELL_EDGE; nR < (nROW + 1) * CELL_EDGE; nR++)
			{
				for (int nC = nCOL * CELL_EDGE; nC < (nCOL + 1) *CELL_EDGE; nC++)
				{
					vFeat[i][mLBPimg.at<uchar>(nR,nC)]++;
				}
			}
		}
	}

	return vFeat;

}






void main()
{
	cv::Mat mImage;
	mImage = cv::imread("image.jpg", 0);
	cv::Mat mLBP = MakeLBPatterns(mImage);
	cv::imwrite("LBP.jpg", mLBP);

	std::vector< std::vector<int> > vComb;
	vComb.resize(2000);
	int nCounter = 0;
	int arr[16] = { 0, };
	CalcCombination(16, 4, arr, 4, &vComb, &nCounter);
	vComb.resize(nCounter);

	std::vector< std::vector<int> > vFeat;
	vFeat = MakeFeatures(mLBP, vComb, nCounter);

	printf("CHECK!\n");
	system("pause");
}




