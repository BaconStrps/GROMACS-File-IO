#include <string>
#include <fstream>
#ifndef GFILE_GRO
#define GFILE_GRO

class Gro
{
	private:
		bool fflag;
		int numatoms;
		int titlesize;
		size_t linesize;
		size_t datasize;
		std::string currentline;
		std::ifstream g;
		char* buf;
		char** data;
		
		void dettitle(); // find title
		
	public:
		Gro(const char* fn);
		~Gro();

		int getnumatoms() { return numatoms; };
		
		char** getdata(long long start, long long end, const char* dat);
		
		bool fail(); 		// return fail

};


#endif

