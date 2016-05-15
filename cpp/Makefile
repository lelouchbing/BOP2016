main: get_res.o http_util.o
	g++ get_res.o http_util.o -lrestclient-cpp -o main

get_res.o: get_res.cpp node.h
	g++ -c get_res.cpp

http_util.o: http_util.cpp http_util.h node.h
	g++ -c http_util.cpp 
