#include <opencv2/core/core_c.h>
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"   /* Hough transform  */

#include <iostream>
#include <string>
#include <vector>

#include "cvimage.h"
#include "trace.h"
#include "linedebug.h"

using namespace std;
using namespace Trace;

void doCanny(const string& input, const string& output, int canny1, int canny2)
{
	cout << "doCanny: [" << input << "] [" << output << "] (" << canny1 << ") (" << canny2 << ")" << endl;

	IplImage* imgInput  = cvLoadImage(input.c_str());
	IplImage* imgOutput = cvCreateImage(cvGetSize(imgInput), IPL_DEPTH_8U, 1);

	cvCanny(imgInput, imgOutput, canny1, canny2);

	cvSaveImage(output.c_str(), imgOutput);
}

void doHough(const string& input, const string& output, int canny1, int canny2, 
	int hough1, int hough2, int hough3)
{
	cout << "doHough: [" << input << "] [" << output << "] (" << canny1 << ") (" << canny2 << ")"
	     << " (" << hough1 << ") (" << hough2 << ") (" << hough3 << ")" << endl;
	
	HoughParams        houghParams(hough1, hough2, hough3);
	Trace::CannyParams cannyParams(canny1, canny2);
	Trace::TraceParams lineTraceParams(0,0,0);

	LineWeldParams lineWeldParams(0,0,0);
	ProcessingParams pp(cannyParams, lineTraceParams, lineWeldParams, houghParams);

	pp.scale = 1;
	pp.selected = 0;
	pp.doHough = true;

	IplImage* imgInput = cvLoadImage(input.c_str());
	CVImage38   screen = CVImage38 (cvGetSize(imgInput));

	houghDebug(imgInput, screen.image(), pp);
	
	cvSaveImage(output.c_str(), screen.image());
}

void doMy(const string& input, const string& output, int canny1, int canny2, 
	int my1, int my2, int my3)
{
	cout << "doMy: [" << input << "] [" << output << "] (" << canny1 << ") (" << canny2 << ")"
	     << " (" << my1 << ") (" << my2 << ") (" << my3 << ")" << endl;
	
	HoughParams        houghParams(0,0,0);
	Trace::CannyParams cannyParams(canny1, canny2);
	Trace::TraceParams lineTraceParams(my1, my2, double(100 - my3)/100.0);

	LineWeldParams lineWeldParams(0,0,0);
	ProcessingParams pp(cannyParams, lineTraceParams, lineWeldParams, houghParams);

	pp.scale = 1;
	pp.selected = 0;
	pp.drawLineFit = true;

	IplImage* imgInput = cvLoadImage(input.c_str());
	CVImage38   screen = CVImage38 (cvGetSize(imgInput));

	vector<Trace::Line> v;
	lineTraceDebug(v, imgInput, screen.image(), /*drawNormals=*/false, pp); 
	
	cvSaveImage(output.c_str(), screen.image());
}



int main(int argc, char** argv)
{
	if (argc < 4) {
		cout << "Usage: ./main input output algorithm params..." << endl;
		return 0;
	}

	string input  = argv[1];
	string output = argv[2];
	string alg    = argv[3];

	if (alg == "canny") {
		if (argc < 6) {
			cout << "Usage: ./main input output canny <canny1> <canny2>" << endl;
			return 0;
		}
		int canny1 = atoi(argv[4]);
		int canny2 = atoi(argv[5]);

		doCanny(input, output, canny1, canny2);
	} else if (alg == "hough") {
		if (argc < 9) {
			cout << "Usage: ./main input output hough <canny1> <canny2> <hough1> <hough2> <hough3>" << endl;
			return 0;
		}
		int canny1 = atoi(argv[4]);
		int canny2 = atoi(argv[5]);
		int hough1 = atoi(argv[6]);
		int hough2 = atoi(argv[7]);
		int hough3 = atoi(argv[8]);

		doHough(input, output, canny1, canny2, hough1, hough2, hough3);

	} else if (alg == "my") {
		if (argc < 9) {
			cout << "Usage: ./main input output my <canny1> <canny2> <my1> <my2> <my3>" << endl;
			return 0;
		}
		int canny1 = atoi(argv[4]);
		int canny2 = atoi(argv[5]);
		// FIX: Refactor and rename.
		int hough1 = atoi(argv[6]);
		int hough2 = atoi(argv[7]);
		int hough3 = atoi(argv[8]);

		doMy(input, output, canny1, canny2, hough1, hough2, hough3);

	} else {
		cout << "Unknown algorithm [" << alg << "]" << endl;
	}

	return 0;
}

