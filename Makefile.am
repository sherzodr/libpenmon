include_HEADERS=libpenmon.h
lib_LTLIBRARIES=libpenmon.la
libpenmon_la_SOURCES=libpenmon.cpp libpenmon.h

ACLOCAL_AMFLAGS=-I m4

example: example.cpp
	$(CXX) -oexample -L".libs/" -I. example.cpp -lpenmon	

example.cpp: all

realclean: clean
	-rm config.* aclocal.m4 example.exe Makefile libtool .Makefile*
	-rm -rf autom4te.cache .deps
