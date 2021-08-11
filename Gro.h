#include <string>
#include <fstream>
class Gro
{
	private:
		bool fflag;
		int numatoms;
		int titlesize;
		int linesize;
		int datasize;
		std::string currentline;
		std::ifstream g;
		char* buf;
		char** data;
		
		void dettitle(); // find title
		
	public:
		Gro(const char* fn);
		~Gro();
		
		char** getdata(int start, int end, const char* dat);
		
		bool fail(); 		// return fail
		
		int seekatom(int n);	// seek to specified atom number, return -1 if fail

};

