#include "Topol.h"
#include <iostream>
#include <string.h>

Topol::Topol(const char* fn)
{
	t.open(fn, std::ios_base::binary);
	f = false;
	if (t.fail())
		f = true;
	
	ff = nullptr;
	nres = 0;
	rescount = 1;
	searchTopol();
}

Topol::~Topol()
{
	if (ff)
	{
		for (int i = 0; i < rescount; i++)
		{
			delete [] ff[i].resname;
		}
		delete [] ff;
	}
}

void Topol::searchTopol()
{
	int size, molpos;
	int linesize = 0;
	int resindex = 0;
	t.seekg(0, t.end);
	size = t.tellg();
	
	char* buf = new char[size];
	t.seekg(0, t.beg);
	t.read(buf, size);
	
	char* temp = strstr(buf, "[ molecules ]");
	molpos = temp - buf;
	
	for (int i = molpos+14;;i++)
	{
		if (buf[i] == ';')
		{
			for (int j = 0;;j++)
			{
				if (buf[i+j] == '\n')
				{
					i+=j+1;
					break;
				}
			}
		}
		if (buf[i] == '\n' && buf[i] != 0)
		{
			rescount++;
		}
		if (buf[i] == '[' || buf[i] == 0)
		{
			break;
		}
	}
	
	
	ff = new residue[rescount];
	
	
	for (int i = molpos + 14;;i++)
	{
		if (buf[i] == ';')
		{
			for (int j = 0;;j++)
			{
				if (buf[i+j] == '\n')
				{
					i+=j+1;
					break;
				}
			}
		}
		
		if (i >= size || buf[i] == '\0')
		{
			break;
		}
		
		if (isalnum(buf[i]))
		{
			ff[resindex++] = checkResidue(&buf[i]);
			for (int j = 0;;j++)
			{
				if (buf[i+j] == '\n' || i+j > size)
				{
					i+=j;
					break;
				}
			}
		}
		if (buf[i] == '[' || buf[i] == 0 || i > size)
		{
			break;
		}
		if (i > t.gcount())
		{
			break;
		}
	}
	
	
	delete [] buf;
}

Topol::residue Topol::checkResidue(char* buf)
{
	char c;
	residue temp;
	for (int i = 1;;i++)
	{
		if (buf[i] == '\n' || buf[i] == 0)
		{
			for (int j = 1;;j++)
			{
				if (isspace(buf[i-j])) // find the start of the molecule count
				{
					c = buf[i];
					buf[i] = '\0';
					temp.nmolc = atoi(&buf[i-j]);
					buf[i] = c;
					break;
				}
			}
			break;
		}
	}
	for (int j = 0;;j++)
	{
		if (isspace(buf[j])) // find the end of the residue name
		{
			temp.resname = new char[(&buf[j]-(buf)+1)];
			memcpy(temp.resname, buf, (&buf[j]-(buf)));
			temp.resname[&buf[j]-buf] == '\0';
			break;
		}
	}

return temp;
}

Topol::residue* Topol::getResidues(int& size)
{
	size = rescount;
	return ff; 
}

bool Topol::fail()
{
	return f;
}
