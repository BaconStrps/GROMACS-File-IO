#ifndef TOPOL
#define TOPOL

#include <fstream>


namespace gfile
{

	class Topol
	{

	public:

		struct residue
		{
			char* resname = nullptr;	//name of residue
			int nmolc = 0;		//molcules per residue
		};

		Topol(const char* fn);
		~Topol();

		void searchTopol();
		bool fail();
		residue* getResidues(int& size);

	private:
		residue checkResidue(char* buf, int size);
		int rescount;
		residue* ff;
		bool f;
		std::ifstream t;

	};
}

#endif

