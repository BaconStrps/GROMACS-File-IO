#include <fstream>

class Topol
{	
		
	public:
		
		struct residue
		{
			char* resname;	//name of residue
			int nmolc;		//molcules per residue
		};
		
		Topol(const char* fn);
		~Topol();
		
		void searchTopol();
		bool fail();
		residue* getResidues(int& size);
		
	private:
		residue checkResidue(char* buf);
		int rescount;
		residue* ff;
		bool f;
		
		int nres;
		std::ifstream t;
		
};
