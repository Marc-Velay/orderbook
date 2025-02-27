CPPFLAGS=-g -pthread 
LIBS=-I -lboost

server: ob_server.cpp
	g++ $(CPPFLAGS) $(LIBS) -o server ob_server.cpp