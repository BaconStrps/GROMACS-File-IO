#ifndef GMX_TRR_FIO
#define GMX_TRR_FIO
#define DIM 3
#define MAGICNUM 1993 // this may be changed at any point, refer to GROMACS source code in trrio.cpp and change this if needed
#include <rpc/xdr.h>
#include <cstring>
#include <stdio.h>

class Trr
{
    enum iotype{INT, REAL, CHAR, UCHAR, INTARR, REALARR, CHARARR, RVECARR};
    
    public:
                                // Trr trr;
        Trr(const char*);
        ~Trr();

        bool getPrecision() {return file.Double;}; // true if double, false if float
        int getnatoms() {return header.natoms;};
        template<typename T>
            T* getBox() {return *reinterpret_cast<T**>(&box);}
        template<typename T>
            T* getX()   {return *reinterpret_cast<T**>(&x);}
        template<typename T>
            T* getV()   {return *reinterpret_cast<T**>(&v);}
        template<typename T>
            T* getF()   {return *reinterpret_cast<T**>(&f);}
        bool readFrame();
        bool fail() {return failed;};

    private:
    enum readwrite{READ, WRITE};

    union real
    {
        float* f;
        double* d;
    };

    struct trr_header // ngl this is ripped from the GROMACS source code. Go support GROMACS!
    {
        bool bDouble;
        int      ir_size;   
        int      e_size;    
        int      box_size;
        int      vir_size;  
        int      pres_size; 
        int      top_size;  
        int      sym_size;  
        int      x_size;
        int      v_size;
        int      f_size;

        int     natoms;
        int     step;
        int     nre;
        double    t;
        double    lambda;
        int     fep_state;
    };
    
    struct trr_file
    {
        FILE* fp = nullptr;
        bool Read = false;
        bool Write = false;
        bool Double = false;
        XDR* xdrptr;
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

    void openxdr(trr_file* file, readwrite rw, const char* filename);

    bool readframedata(trr_file*, trr_header*, real*, real*, real*, real*);

    bool readheader(trr_file* file, trr_header* header);

    bool readxdr(trr_file* file, void* item, size_t arrsize, iotype type);


};

#endif