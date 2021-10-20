#include "Topol.h"
#include <iostream>
#include <string.h>


namespace gfile
{
	Topol::Topol(const char* fn)
	{
		t.open(fn, std::ios_base::binary);
		f = false;
		if (t.fail())
			f = true;

		ff = nullptr;
		rescount = 1;
		searchTopol();
	}

	Topol::~Topol()
	{
		if (ff)
		{
			for (int i = 0; i < rescount; i++)
			{
				delete[] ff[i].resname;
			}
			delete[] ff;
		}
	}

	void Topol::searchTopol()
	{
		size_t size = 0, molpos = 0;
		int linesize = 0;
		int resindex = 0;
		t.seekg(0, t.end);
		size = t.tellg();
		size++;

		char* buf = new char[size];
		t.seekg(0, t.beg);
		t.read(buf, size);
		buf[size - 1] = '\0';

		char* temp = strstr(buf, "[ molecules ]");
		molpos = temp - buf;

		for (size_t i = molpos + 14; i < size; i++)
		{
			if (buf[i] == ';')
			{
				for (size_t j = 0; j < size - i; j++)
				{
					if (buf[i + j] == '\n')
					{
						i += j + 1;
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


		for (size_t i = molpos + 14; i < size; i++)
		{
			if (buf[i] == ';')
			{
				for (size_t j = 0; j < size - i; j++)
				{
					if (buf[i + j] == '\n')
					{
						i += j + 1;
						break;
					}
				}
			}

			if (isalnum(buf[i]))
			{
				ff[resindex++] = checkResidue(&buf[i], size - i);
				for (size_t j = 0; j < size - i; j++)
				{
					if (buf[i + j] == '\n' || j + 1 == size - i)
					{
						i += j;
						break;
					}
				}
			}
			if (buf[i] == '[')
			{
				break;
			}
		}

		delete[] buf;
	}

	Topol::residue Topol::checkResidue(char* buf, int size)
	{
		char c;
		size_t strl;
		residue temp;
		for (int i = 1; i < size; i++)
		{
			if (buf[i] == '\n' || buf[i] == 0)
			{
				for (int j = 1;; j++)
				{
					if (isspace(buf[i - j])) // find the start of the molecule count
					{
						c = buf[i];
						buf[i] = '\0';
						temp.nmolc = atoi(&buf[i - j]);
						buf[i] = c;
						break;
					}
				}
				break;
			}
		}
		for (int i = 0; i < size; i++)
		{
			if (isspace(buf[i])) // find the end of the residue name
			{
				strl = &buf[i] - buf;
				temp.resname = new char[strl + 1];
				memcpy(temp.resname, buf, strl);
				temp.resname[strl] = '\0';
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

}
