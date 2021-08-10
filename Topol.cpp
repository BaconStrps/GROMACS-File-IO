#include "Topol.h"
#include <

Topol::Topol(const char* fn)
{
	t.open(fn);
	if (t.fail())
		f = true;
		
	nres = 0;
	
	searchTopol();
}


void Topol::searchTopol()
{
	size_t size, molpos, incpos;
	string molecules, include;
	t.seekg(0, t.end);
	size = t.tellg();
	
	char* buf = new char[size];
	t.seekg(0, t.beg);
	t.read(buf, size);
	
	char* temp = strstr(buf, "[ molecules ]");
	molpos = temp - buf;

	molecules.assign(temp, (strstr(temp, "\n") - temp));
	
	temp = strstr(buf, "#include");
	incpos = temp - buf;
	
	include.assign(temp, (strstr(temp, "\n") - temp));
	
	t.seekg(molpos, t.beg);
	
	
	while (1)
	{
		getline(t, molecules);
		if (molecules.compare("") != -1)
			nres++;
		else
			break;
	}
	
	ff = new itp[nres];
	
	t.seekg(incpos, t.beg);
	
	for (int i = 0;;i++)
	{
		if (include[0] != '#')
			break;
		else
		{
			ff[i] = searchITP(include.substr(10, include.length()-11).c_str())
			getline(t, include);
		}
	}
	// somewhere here you need to initialize the itp struct member nmolc (number molecules)
	delete [] buf;
}


itp Topol::searchITP(const char* fn) //to do: write this function, initialize itp structs.
{
	ifstream i(fn);
	string prev, c;
	
	i.seekg(0, t.end);
	size = i.tellg();
	
	char* buf = new char[size];
	i.seekg(0, t.beg);
	i.read(buf, size);
	prev.assign(strstr(buf, ))
	
}


Topol::fail()
{
	return f;
}
