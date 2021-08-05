#include "Gro.h"
#include <string.h>

Gro::Gro(const char* fn)
{
	g.open(fn);
	
	titlesize = 0;
	linesize = 0;
	fflag = false;
	
	dettitle();
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

bool Gro::seekatom(int n)
{
	g.seekg(((n-1) * linesize)+ titlesize, g.beg);
	getline(g, currentline);
	if (g.fail())
		return 1;
	else
	return 0;
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
	char* s;
	memcpy(s, &currentline[5], 5);
	s[5] = '\0';
	
	return s;
}
char* Gro::getatomname()
{
	char* s;
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
