#include "Gro.h"
#include <iostream>
#include <string.h>

Gro::Gro(const char* fn)
{
	g.open(fn, std::ios_base::binary);
	
	data = nullptr;
	buf = nullptr;
	
	titlesize = 0;
	linesize = 0;
	fflag = false;
	
	dettitle();
}

Gro::~Gro()
{
	if (data)
	{
		for (int i = 0; i < datasize; i++)
			delete [] data[i];
		delete [] data;
	}
	
	if (buf)
	{
		delete [] buf;
	}
	
	g.close();
}

char** Gro::getdata(int start, int end, const char* dat)
{
	if (data)
	{
		for (int i = 0; i < datasize; i++)
			delete [] data[i];
		delete [] data;
	}
	
	if (buf)
	{
		delete [] buf;
	}
	
	int pos;
	int len;
	int offset;
	datasize = end - start + 1;
	buf = new char[datasize*linesize];
		
	g.seekg(((start-1) * linesize) + titlesize, g.beg);
	
	g.read(buf, datasize*linesize);
	
	if (g.gcount() != datasize*linesize) // safety just in case datasize would lead us past the end of file.
	{
		std::cout << "datasize before: " << datasize;
		datasize = g.gcount()/linesize;
		std::cout << "datasize after: " << datasize;
	}
	
	if (strcmp(dat, "rnum") == 0)
	{
		pos = 0;
		len = 5;
	}
	else if (strcmp(dat, "rnam") == 0)
	{
		pos = 5;
		len = 5;
	}
	else if (strcmp(dat, "anam") == 0)
	{
		pos = 10;
		len = 5;
	}
	else if (strcmp(dat, "anum") == 0)
	{
		pos = 15;
		len = 5;
	}
	else if (strcmp(dat, "x")  == 0)
	{
		pos = 20;
		len = 8; 
	}
	else if (strcmp(dat, "y")  == 0)
	{
		pos = 28;
		len = 8; 
	}
	else if (strcmp(dat, "z")  == 0)
	{
		pos = 36;
		len = 8; 
	}
	else if (strcmp(dat, "vx")  == 0)
	{
		pos = 44;
		len = 8; 
	}
	else if (strcmp(dat, "vy")  == 0)
	{
		pos = 52;
		len = 8; 
	}
	else if (strcmp(dat, "vz")  == 0)
	{
		pos = 60;
		len = 8; 
	}
	
	
	data = new char*[datasize];
	
	for (int i = 0; i < datasize; i++)
	{
		data[i] = new char[len+1];
	}
	
	
	
	for (int i = 0; i < datasize; i++)
	{
		offset = i*linesize + pos;
		memcpy(data[i], &buf[offset], len);
		data[i][len] = '\0';
		
	}
	
	
	return data;
}

void Gro::dettitle()
{
	if (g.fail())
	{
		fflag = true;
		return; // gro file failed to open, set fail flag.
	}
	
	std::getline(g, currentline);			// a .gro has 2 fixed lines at the beginning
	titlesize += currentline.length() + 1;
	std::getline(g, currentline);
	titlesize += currentline.length() + 1;
	
	numatoms = atoi(currentline.c_str());
	
	std::getline(g, currentline);
	linesize += currentline.length() + 1; 		// this gives us the length of an atom line.
	
}

bool Gro::fail()
{
	return fflag;
}
