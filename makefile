all: opencv boost

opencv: testopencv.cpp

	g++ testopencv.cpp `pkg-config --libs opencv` -o test

boost: testboost.cpp

	g++ -std=c++11 testboost.cpp -lboost_system -lboost_filesystem -o btest

bgsub: bgsub.cpp

	g++ -g -std=c++11 bgsub.cpp -lboost_system -lboost_filesystem `pkg-config --libs opencv` -o bgsub

detect: detect.cpp

	g++ -g -std=c++11 detect.cpp -lboost_system -lboost_filesystem `pkg-config --libs opencv` -o detect


