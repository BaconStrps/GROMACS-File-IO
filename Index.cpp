#include "Index.h"
#include <string.h>


namespace gfile
{

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

		char* buf = new char[size];
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

		if (endofgr < 0)
			return temp;
	
		int* starts = new int[256];
		int* ends = new int[256]; // temp storage

		int num1 = 0, num2 = 0, dif = 0;
	
		char* curpos = hline;
	
		size_t numswap = 0;
		size_t n = strstr(hline, "]") - hline - 3;
	
		temp.grname = new char[n+1];
		memcpy(temp.grname, hline+2, n);
		temp.grname[n] = '\0';
		curpos += n+2;
	
		while (1)
		{
			if (isdigit(*curpos))
			{
				starts[numswap] = atoi(curpos);
				numswap++;
				break;
			}
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


		temp.indstart = starts;
		temp.indend = ends;
		temp.indices = numswap;
	
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
			return i;
		}

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

}
