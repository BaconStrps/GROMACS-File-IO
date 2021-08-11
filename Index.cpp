#include "Index.h"
#include <iostream>
#include <string.h>
Index::Index(const char* fn)
{
	index.open(fn, std::ios_base::binary);
	
	f = false;
	
	if (index.fail())
	{
		f = true;
		index.close();
	}
	
	groupnum = 0;
	
	initIndex();
}

Index::~Index()
{	
	index.close();
	for (int i = 0; i < groupnum; i++)
	{
		delete [] grs[i].grname;
	}
	
	delete [] grs;
}


void Index::initIndex()
{
	int size; 
	int headpos[128]; // id be surprised if someone had more than 128 unique groups
	char* buf;
	
	index.seekg(0, index.end);
	size = index.tellg();
	buf = new char[size]; 
	
	index.seekg(0, index.beg);
	
	if (index.fail())
	{
		f = true;
		return;
	}
	
	index.read(buf, size);
	
	for (int i = 0; i < size; i++)
	{
		if (buf[i] == '\n' || i == 0)
			if (buf[i+1] == '[' || i == 0)
			{
				headpos[groupnum++] = i+1;
			}
	}
	
	grs = new group[groupnum];
	
	for (int i = 0; i < groupnum; i++)
	{
		grs[i] = readGroup(&buf[headpos[i]]);
	}
	
	delete [] buf;
	
}

Index::group Index::readGroup(char* hline)
{
	group temp;
	char c;
	int n = strstr(hline, "]") - hline - 1;
	temp.grname = new char[n+1];
	memcpy(temp.grname, hline+1, n);
	temp.grname[n] = '\0';
	
	// start
	
	for (int i = n;;i++)
	{
		if (isalnum(hline[i]))
		{
			for (int j = 0;;j++)
			{
				if (!isalnum(hline[i+j]))
				{
					hline[i+j] = '\0';
					temp.start = atoi(&hline[i]);
					break;
				}
			}
			n += i;
			break;
		}
	}
	
	// end
	
	for (int i = n;;i++)
	{
		if (hline[i] == 0) // E O F
		{
			for (int j = 0;;j++)
			{
				if (hline[i-j] == ' ' && isalnum(hline[i-j+1]))
				{
					temp.end = atoi(&hline[i-j]);
					break;
				}
			}
			break;
		}
		
		if (hline[i] == '\n')
			if (hline[i+1] == '[')
			{
				for (int j = 0;;j++)
				{
					if ((hline[i-j] == ' ' || hline[i-j] == '\n') && isalnum(hline[i-j+1]))
					{
						hline[i] = '\0';
						temp.end = atoi(&hline[i-j]);
						break;
					}
				}
				
				break; 
			}
	}
	
	return temp;
}

Index::group* Index::getGroups(int& size)
{
	size = groupnum;
	return grs;
}

bool Index::fail()
{
	return f;
}

int Index::getGroupnum()
{
	return groupnum;
}
