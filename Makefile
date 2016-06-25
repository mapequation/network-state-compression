
all: project compress

project: cpp/project.cpp 
	g++ -std=c++11 -o3 cpp/project.cpp -o project

compress: cpp/compress.cpp 
	g++ -std=c++11 -o3 cpp/compress.cpp cpp/Aggregator/plogp.cpp -o compress

clean:
	rm compress project