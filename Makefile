CPPFLAGS=-g -pthread 
LIBS=-I .

all: server ob_exec

server: ob_server.cpp
	g++ $(CPPFLAGS) $(LIBS) -o server ob_server.cpp


ob_exec: order_book.cpp order_book.hpp
	g++ $(CPPFLAGS) $(LIBS) -o ob_exec order_book.cpp