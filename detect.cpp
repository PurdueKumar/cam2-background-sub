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
	namedWindow("person detection" , CV_WINDOW_AUTOSIZE);
	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());


	for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})){


		if(entry.path().extension() != ".png" && entry.path().extension() != ".jpg") {

			continue;

		}

		frame = imread(entry.path().string());
		if(frame.empty()) {

			cerr << "ERROR: Unable to open image: " << entry.path() << endl;
			exit(1);
		}

		while (true) {
     
        	vector<Rect> found, found_filtered;
        	hog.detectMultiScale(frame, found, 0, Size(8,8), Size(32,32), 1.05, 2);
        	size_t i, j;
        	for (i=0; i<found.size(); i++) 
        	{
            	Rect r = found[i];
            	for (j=0; j<found.size(); j++) 
                	if (j!=i && (r & found[j]) == r)
                    	break;
            	if (j== found.size())
                	found_filtered.push_back(r);
        	}
 
        	for (i=0; i<found_filtered.size(); i++) 
        	{
            	Rect r = found_filtered[i];
            	r.x += cvRound(r.width*0.1);
		    	r.width = cvRound(r.width*0.8);
		    	r.y += cvRound(r.height*0.07);
		    	r.height = cvRound(r.height*0.8);
		    	rectangle(frame, r.tl(), r.br(), Scalar(0,255,0), 3);        
        }
 
        	imshow("opencv", frame);
        	string buffer = "detect_orig/" + entry.path().stem().string() + "_od.jpg";
        	imwrite(buffer, frame);


        	if (waitKey(10)>=0)
            	break;
    	}
	}

	return 0;

}
