#ifndef GMX_TRR_FIO
#define GMX_TRR_FIO
constexpr long DIM = 3;
constexpr long MAGICNUM = 1993; // this may be changed at any point, refer to GROMACS source code in trrio.cpp and change this if needed
#include <cstring>
#include <fstream>
#include <memory>


namespace gfile
{


    class Trr
    {
        enum class iotype { INT, REAL, CHAR, UCHAR, INTARR, REALARR, CHARARR, RVECARR };
    public:
        struct trr_header // ngl this is ripped from the GROMACS source code. Go support GROMACS!
        {
            bool    bDouble;
            int     ir_size;
            int     e_size;
            int     box_size;
            int     vir_size;
            int     pres_size;
            int     top_size;
            int     sym_size;
            int     x_size;
            int     v_size;
            int     f_size;

            int     natoms;
            int     step;
            int     nre;
            double  t;
            double  lambda;
            int     fep_state;
        };

    public:
        // Trr trr;
        Trr(const char*);
        ~Trr();

        bool getPrecision() { return file.Double; }; // true if double, false if float
        int getnatoms() { return header.natoms; };
        template<typename T>
        T* getBox() { return *reinterpret_cast<T**>(&box); }
        template<typename T>
        T* getX() { return *reinterpret_cast<T**>(&x); }
        template<typename T>
        T* getV() { return *reinterpret_cast<T**>(&v); }
        template<typename T>
        T* getF() { return *reinterpret_cast<T**>(&f); }
        bool readFrame();
        bool fail() { return failed; };

        trr_header& getCurrentHeader() { return header; }

    private:
        enum readwrite { READ, WRITE };

        union real
        {
            float* f;
            double* d;
        };

        struct trr_file
        {
            std::ifstream fp;
            size_t filesize;
            char* buf;
            char* pos;
            bool Read = false;
            bool Write = false;
            bool Double = false;

            bool checkifend()
            {
                if (pos >= buf + filesize)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        bool first;
        real box;
        real x;
        real v;
        real f;

        bool failed;

        trr_file file;
        trr_header header;

        int setfloatsize(trr_header* header);

        bool readframedata(trr_file*, trr_header*, real*, real*, real*, real*);

        bool readheader(trr_file* file, trr_header* header);

        bool readxdr(trr_file* file, void* item, size_t arrsize, iotype type);


    };
}
#endif