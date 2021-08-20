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

	index.seekg(0, index.end);
	size = index.tellg();
	index.seekg(0);
	
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
	
	index.close();

	delete [] grs;
}


void Index::initIndex()
{
	int headpos[128]; // id be surprised if someone had more than 128 unique groups

	memset(headpos, 0, 128*sizeof(int));

	char buf[size+1];
	buf[size] = 0;

	if (index.fail())
	{
		f = true;
		return;
	}

	index.read(buf, size);
	
	for (int i = 0; i < size; i++)
	{
		if (buf[i] == '[')
			headpos[groupnum++] = i;
	}
	
	grs = new group[groupnum];
	
	for (int i = 0; i < groupnum; i++)
	{
		grs[i] = readGroup(&buf[headpos[i]], size - headpos[i]);
	}
	
	
}

Index::group Index::readGroup(char* hline, int endofgr)
{
	group temp;
	
	int* starts = new int[256];
	int* ends = new int[256]; // temp storage

	int num1 = 0, num2 = 0, dif = 0;
	
	char* curpos = hline;
	
	size_t numswap = 0;
	int numlength;
	int n = strstr(hline, "]") - hline - 2;
	//printf("hlinestart: %p\nSize: %d\nEndofgr: %p\n", hline, endofgr, hline+endofgr);
	
	temp.grname = new char[n+1];
	memcpy(temp.grname, hline+1, n);
	temp.grname[n] = '\0';
	curpos += n+1;
	
	while (1)
	{
		if (isdigit(*curpos))
		{
			//printf("%c\n", *curpos);
			starts[numswap] = atoi(curpos);
			numswap++;
			break;
		}
		//printf("%c\n", *curpos);
		curpos++;
	}


	for (int i = 0; curpos < hline + endofgr; i++)
	{
		if (isdigit(*curpos))
		{
			num1 = atoi(curpos);

			curpos += getnumlength(num1);

			if (num2)
			{
				dif = num1 - num2;
				if (dif != 1 && dif != -1)
				{
					starts[numswap] = (num1 > num2) ? num1 : num2;
					ends[numswap-1] = (num1 > num2) ? num2 : num1;
					numswap++;
				}
			}

			for (int j = 0;;j++)
			{
				if (*curpos == '[')
					break;
				if (isdigit(*curpos))
				{
					num2 = atoi(curpos);
					curpos += getnumlength(num2);
					dif = num1 - num2;
					if (dif != 1 && dif != -1)
					{
						starts[numswap] = (num1 > num2) ? num1 : num2;
						ends[numswap-1] = (num1 > num2) ? num2 : num1;
						numswap++;
					}
					break;
				}
				curpos++;
			}
		}
		if (*curpos == 0 || *curpos == '[')
			break;
		curpos++;
	}

	if (num1 != 0 && num2 != 0)
	{
		ends[numswap-1] = (num1 > num2) ? num1 : num2;
	}
	else if (num1 != 0)
	{
		ends[numswap-1] = num1;
	}
	else
	{
		ends[numswap-1] = num2;
	}

	/*for (int i = n; i < size; i++)
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
		
	}*/ //banish this code

	temp.indstart = starts;
	temp.indend = ends;
	temp.size = numswap;
	
	return temp;
}

int Index::getnumlength(int num)
{
	int i;
	if (num == 0)
	{
		return 1;
	}
	else
	{
		for (i = 0; num != 0; i++)
			num/=10;

		//printf("i: %d\n", i);
		return i;
	}

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
