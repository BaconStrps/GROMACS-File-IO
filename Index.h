#ifndef INDEX
#define INDEX
#include <fstream>
class Index
{
	public:
		
		struct group
		{
			char* grname;
			int start;
			int end;
		};
	
		Index(const char* fn);
		~Index();
		
		int getGroupnum();
		
		bool fail();
		
		group* getGroups(int& size);
		
		
	private:
		bool f;
		std::ifstream index;
		group* grs; // group pointer for array
		
		int groupnum;
		
		void initIndex();
		group readGroup(char* hline);
	
};

#endif
