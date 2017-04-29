#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace boost::filesystem;
using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

	if(argc < 2 || argc > 4) {

		cerr << "ERROR: invalid input arguements\n";
		exit(1);
	}

	path p = argv[1];

	if(!is_directory(p)) {

		cerr << "ERROR: cannot find directory " << p <<endl;
		exit(1);
	}

	Mat frame;
	Mat fgMask;
	//Mat bgImg;
	Ptr<BackgroundSubtractorMOG2> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2();
	pMOG2->setDetectShadows(false);

	if(argc >= 3) {

		int threshold = atoi(argv[2]);
		pMOG2->setVarThreshold(threshold);

	}

	if(argc == 4) {

		int history = atoi(argv[3]);
		pMOG2->setHistory(history);

	}

	//namedWindow("Frame");
	//namedWindow("FG Mask MOG 2");

	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})){


		if(entry.path().extension() != ".png" && entry.path().extension() != ".jpg") {

			continue;

		}

		frame = imread(entry.path().string());
		if(frame.empty()) {

			cerr << "ERROR: Unable to open image: " << entry.path() << endl;
			exit(1);

		}

		string buffer;
		pMOG2->apply(frame, fgMask);
		//cout << entry.path().string()<<endl;
		buffer = "bgsub_hist300_thres16/" + entry.path().stem().string() + "_bgsub_300_16.jpg";
		//imshow("Frame" , frame);
		//imshow("FG Mask MOG 2" , fgMask);
		imwrite(buffer, fgMask);
	}

	//pMOG2->getBackgroundImage(bgImg);
	//imwrite("bgsubImgages/image1.jpg", bgImg);
	//imwrite("fgMask.jpg" , fgMask);
	return 0;

} 
