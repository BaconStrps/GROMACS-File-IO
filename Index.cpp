#include "Index.h"
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
		delete [] grs[i].indstart;
		delete [] grs[i].indend;
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
	buf = new char[size+1]; 
	
	index.seekg(0, index.beg);
	if (index.fail())
	{
		f = true;
		return;
	}
	index.read(buf, size);
	
	buf[size] = '\0';
	
	for (int i = 0; i < size; i++)
	{
		if (buf[i] == '\n' || i == 0)
			if (buf[i+1] == '[' || i == 0)
			{
				headpos[groupnum++] = (i == 0) ? i+1 : i+2;
			}
	}
	
	grs = new group[groupnum];
	
	for (int i = 0; i < groupnum; i++)
	{
		grs[i] = readGroup(&buf[headpos[i]], size - headpos[i]);
	}
	
	index.close();
	
	delete [] buf;
	
}

Index::group Index::readGroup(char* hline, int size)
{
	group temp;
	
	int* starts = new int[256];
	int* ends = new int[256]; // temp storage
	char num1[256];
	char num2[256];
	char tempnum[256];
	
	char *space, *newl, *head;
	
	memset(num1, 0, 256);
	memset(num2, 0, 256);
	memset(tempnum, 0, 256);
	
	size_t numswap = 0;
	int numlength;
	int n = strstr(hline, "]") - hline - 2;
	
	temp.grname = new char[n+1];
	memcpy(temp.grname, hline+1, n);
	temp.grname[n] = '\0';
	//std::cout << "GROUP: " << temp.grname << "A\n\n";
	
	for (int i = n; i < size; i++) //find start index
	{
		if (isdigit(hline[i]))
		{
			numlength = (strstr(&hline[i], " ") - &hline[i]);
			memcpy(num1, &hline[i], numlength);
			num1[numlength] = '\0';
			starts[numswap] = atoi(num1);
			numswap++;
			break;
		}
	}

	for (int i = n; i < size; i++)
	{
		if (isdigit(hline[i]))
		{
			head = strchr(&hline[i], '[');
			space = strchr(&hline[i], ' ');
			newl = strchr(&hline[i], '\n');
			if (space == nullptr && newl == nullptr && head == nullptr)
			{
				numlength = size - i;
			}
			else if (head < space && head != nullptr)
			{
				numlength = head - &hline[i];
			}
			else if (space > newl && newl != nullptr && space != nullptr)
			{
				numlength = newl - &hline[i];
			}
			else if (space != nullptr)
			{
				numlength = space - &hline[i];
			}
			strncpy(num1, &hline[i], numlength);
			num1[numlength] = '\0';
			
			if (numlength < 0)
				break;
			
			if (num1[0] != 0 && num2[0] != 0 && (getabsdif(num1, num2) > 1 || getabsdif(num2, tempnum) > 1))
			{
				if (getabsdif(tempnum, num2) > 1)
				{
					starts[numswap] = (atoi(tempnum) - atoi(num2) < 0) ? atoi(num2) : atoi(tempnum);
					ends[numswap-1] = (atoi(tempnum) - atoi(num2) > 0) ? atoi(num2) : atoi(tempnum);
				}
				else
				{				
					starts[numswap] = (atoi(num1) - atoi(num2) < 0) ? atoi(num2) : atoi(num1);
					ends[numswap-1] = (atoi(num1) - atoi(num2) > 0) ? atoi(num2) : atoi(num1);
				}
				numswap++;
			}
			
			for (int j = numlength; i+j < size; j++)
			{
				if (isdigit(hline[i+j]))
				{
					
					space = strchr(&hline[i+j], ' ');
					newl = strchr(&hline[i+j], '\n');
					
					if (space == nullptr && newl == nullptr && head == nullptr)
					{
						numlength = size - i;
					}
					else if (head < space && head != nullptr)
					{
						numlength = head - &hline[i];
					}
					else if (space > newl && newl != nullptr && space != nullptr)
					{
						numlength = newl - &hline[i+j];
					}
					else if (space != nullptr)
					{
						numlength = space - &hline[i+j];
					}
					if (numlength < 0)
						break;
					strncpy(num2, &hline[i+j], numlength);
					num2[numlength] = '\0';
					
					if (head < space && head != nullptr)
					{
						if (hline[i+j-2] == ']')
							memset(num2, 0, 256);
						i+=numlength;
						break;
					}
					i += j + numlength;
					break;
				}
			}
		}
		strcpy(tempnum, num1);
		if (hline[i] == '[')
			break;
		
	}
	ends[numswap-1] = (atoi(num1) - atoi(num2) < 0) ? atoi(num2) : atoi(num1);
	
	
	temp.indstart = starts;
	temp.indend = ends;
	temp.size = numswap;
	
	return temp;
}

int Index::getabsdif(char* a1, char* a2)
{
	int temp;
	temp = ((atoi(a1) - atoi(a2) > 0) ? atoi(a1) - atoi(a2) : atoi(a2) - atoi(a1));
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
