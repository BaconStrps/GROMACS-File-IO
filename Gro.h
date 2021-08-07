#include <string>
#include <fstream>
class Gro
{
	private:
		bool fflag;
		int numatoms;
		int titlesize;
		int linesize;
		std::string currentline;
		std::ifstream g;
		
		char** data;
		
		void dettitle(); // find title
		
	public:
		Gro(const char* fn);
		~Gro();
		
		char** getdata(int start, int end, const char* dat);
		
		int getresnum(); 	// return residue number
		char* getresname();	// return residue name
		char* getatomname();// return atom name
		int getatomnum();	// return atom number
		double getx();		// return x pos of current atom
		double gety();		// return y pos of current atom
		double getz();		// return z pos of current atom
		double getvx();		// return x velocity of current atom
		double getvy();		// return x velocity of current atom
		double getvz();		// return x velocity of current atom
		std::string getcline(); 	// return currentline
		void getnline();		// basically getline
		int getnatoms();	// return numatoms
		bool fail(); 		// return fail
		
		int seekatom(int n);	// seek to specified atom number, return -1 if fail

};

