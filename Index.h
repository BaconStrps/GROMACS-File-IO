#ifndef INDEX
#define INDEX
#include <fstream>

namespace gfile
{

	class Index
	{
	public:

		struct group
		{
			char* grname = nullptr;
			int* indstart = nullptr;
			int* indend = nullptr;
			int indices = 0;
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
		int size;

		int getnumlength(int num);

		void initIndex();
		group readGroup(char* hline, int size);

	};
}
#endif
