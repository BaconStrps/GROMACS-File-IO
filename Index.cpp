#include "Index.h"
#include <iostream>
#include <string.h>
Index::Index(const char* fn)
{
	index.open(fn, std::ios_base::binary);
	
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
	// Doesn't need to be null terminated we'll read as if it was a file and split it accordingly.
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
	
	std::cout << groupnum << '\n';
	
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
	std::cout << n << '\n';
	temp.grname = new char[n+1];
	memcpy(temp.grname, hline+1, n);
	temp.grname[n] = '\0';
	
	for (int i = n;;i++)
	{
		if (isalnum(hline[i]))
		{
			for (int j = 0;;j++)
			{
				if (!isalnum(hline[i+j]))
				{
					c = hline[i+j];
					hline[i+j] = '\0';
					temp.start = atoi(&hline[i]);
					hline[i+j] = c;
					break;
				}
			}
			break;
		}
	}
	
	for (int i = n;;i++)
	{
		if (hline[i] == 0)
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
