#include "Gro.h"
#include <string.h>

Gro::Gro(const char* fn)
{
	g.open(fn);
	
	data = nullptr;
	
	titlesize = 0;
	linesize = 0;
	fflag = false;
	
	dettitle();
}

Gro::~Gro()
{
	if (data != nullptr)
	{
		for (size_t i = 0; i < sizeof(data); i++)
			delete [] data[i];
		delete [] data;
	}
}

char** Gro::getdata(int start, int end, const char* dat)
{
	int pos;
	int len;
	if (data != nullptr)
	{
		for (size_t i = 0; i < sizeof(data); i++)
			delete [] &data[i];
		delete [] data;
	}
		
	g.seekg(((start-1) * linesize) + titlesize, g.beg);
	
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
	
	data = new char*[end - start];
	
	for (int i = 0; i < end - start; i++)
	{
		data[i] = new char[len+1];
	}
	
	for (int i = 0; i < end; i++)
	{
		getline(g, currentline);
		memcpy(data, &currentline[pos], len);
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

int Gro::seekatom(int n)
{
	if (g.fail() || n > numatoms)
		return -1;
	else
	{
		g.seekg(((n-1) * linesize) + titlesize, g.beg);
		getline(g, currentline);
		return 0;
	}
	
}

std::string Gro::getcline()
{
	return currentline;
}

int Gro::getnatoms()
{
	return numatoms;
}

bool Gro::fail()
{
	return fflag;
}
int Gro::getresnum()
{
	return atoi(currentline.substr(0,5).c_str());
}
char* Gro::getresname()
{
	char* s = nullptr;
	memcpy(s, &currentline[5], 5);
	s[5] = '\0';
	
	return s;
}
char* Gro::getatomname()
{
	char* s = nullptr;
	memcpy(s, &currentline[10], 5);
	s[5] = '\0';
	
	return s;
}
int Gro::getatomnum()
{
	char s[6];
	memcpy(s, &currentline[15], 5);
	s[5] = '\0';
	return atoi(s);
	
}
double Gro::getx()
{
	char s[9];
	memcpy(s, &currentline[20], 8);
	s[8] = '\0';
	return atof(s);
}
double Gro::gety()
{
	char s[9];
	memcpy(s, &currentline[28], 8);
	s[8] = '\0';
	return atof(s);
}
double Gro::getz()
{
	char s[9];
	memcpy(s, &currentline[36], 8);
	s[8] = '\0';
	return atof(s);
}
double Gro::getvx()
{
	if (currentline.length() < 36)
	{
		return 0; // no velocities to check
	}
	char s[9];
	memcpy(s, &currentline[44], 8);
	s[8] = '\0';
	return atof(s);
}
double Gro::getvy()
{
	if (currentline.length() < 36)
	{
		return 0; // no velocities to check
	}
	char s[9];
	memcpy(s, &currentline[52], 8);
	s[8] = '\0';
	return atof(s);
}
double Gro::getvz()
{
	if (currentline.length() < 36)
	{
		return 0; // no velocities to check
	}
	char s[9];
	memcpy(s, &currentline[60], 8);
	s[8] = '\0';
	return atof(s);
}
