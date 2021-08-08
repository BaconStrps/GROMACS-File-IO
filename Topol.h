#include <fstream>

class Topol
{
	private:
		struct itp
		{
			char* resname;
			char* fn;
			int natoms;
			int nmolc;
			int index[2]; //start, end
		};
		
		itp* ff;
		bool f();
		
		int nres;
		ifstream t;
		
		
	public:
		
		Topol(const char* fn);
		
		void searchTopol();
		itp searchITP(const char* fn);
		bool fail();
		
};
