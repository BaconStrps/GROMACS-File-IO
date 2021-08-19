#include "Trr.h"
#define readint(file, item)             readxdr(file, &(item), 0, INT)
#define readreal(file, item)          readxdr(file, &(item), 0, REAL)
#define readchar(file, item)            readxdr(file, &(item), 0, CHAR)
#define readuchar(file, item)           readxdr(file, &(item), 0, UCHAR)
#define readrvec(file, item, nitem)      readxdr(file, item, nitem, RVECARR)
#define readintarr(file, item)          readxdr(file, &(item), 0, INTARR)
#define readrealarr(file, item)       readxdr(file, &(item), 0, REALARR)
#define readstr(file, item)             readxdr(file, &(item), 0, CHARARR)

Trr::Trr(const char* fn)
{
    first = true;
    failed = false;
    openxdr(&file, READ, fn);
    readheader(&file, &header);

    if (file.Double)
    {
        box.d =    new double[DIM*DIM];
        x.d =      new double[DIM*header.natoms];
        v.d =      new double[DIM*header.natoms];
        f.d =      new double[DIM*header.natoms];
    }
    else
    {
        box.f =    new float[DIM*DIM];
        x.f =      new float[DIM*header.natoms];
        v.f =      new float[DIM*header.natoms];
        f.f =      new float[DIM*header.natoms];
    }
}

Trr::~Trr()
{
    if (file.Double)
    {
        delete [] box.d;
        delete [] x.d;
        delete [] v.d;
        delete [] f.d;
    }
    else
    {
        delete [] box.f;
        delete [] x.f;
        delete [] v.f;
        delete [] f.f;
    }

    fclose(file.fp);
    delete file.xdrptr;

}
bool Trr::readFrame()
{
    if (!first)
    {
        //printf("notfirst\n");
        if (!readheader(&file, &header) || failed)
        {
            failed = true;
            //printf("readheader fail\n");
            return failed;
        }
    }
    if(!readframedata(&file, &header, &box, &x, &v, &f))
    {
        failed = true;
        //printf("readframedata fail\n");
        return failed;
    }

    if (feof(file.fp))
    {
        failed = true;
    }

    first = false;

    return failed;
}

void Trr::openxdr(trr_file* file, readwrite rw, const char* filename)
{
    bool good;
    file->xdrptr = new XDR;
    file->xdrptr->x_ops = NULL;
    if (rw == WRITE)
    {
        file->Write = true;
        file->fp = fopen(filename, "w");
    }
    if (rw == READ)
    {
        file->Read = true;
        file->fp = fopen(filename, "r");
    }
    xdrstdio_create(file->xdrptr, file->fp, (rw==READ) ? XDR_DECODE : XDR_ENCODE);

    if (file->xdrptr->x_ops == NULL)
    {
        printf("xdr null\n");
    }
}

int Trr::setfloatsize(trr_header* header)
{
    int size = 0;

    if (header->box_size)
    {
        size = header->box_size / (DIM*DIM);
    }
    else if (header->x_size)
    {
        size = header->x_size / (header->natoms * DIM);
    }
    else if (header->v_size)
    {
        size = header->v_size / (header->natoms * DIM);
    }
    else if (header->f_size)
    {
        size = header->f_size / (header->natoms * DIM);
    }
    else
    {
        printf("dunno man\n");
        size = 0;
    }

    if (size != sizeof(double) && size != sizeof(float))
    {
        printf("????: %d\n", size);
    }
    return size;
    
    
}

bool Trr::readheader(trr_file* file, trr_header* header)
{
    int magic;
    char buf[256];
    bool good = true;

    if (!readint(file, magic))
    {
        failed = true;
        return false;
    }

    if (magic != MAGICNUM)
    {
        printf("magicnum not found. Are you sure this is a trr file? magicnum: %d\n", magic);
        failed = true;
        return false;
    }

    if (!readstr(file, buf))
    {
        printf("strfail\n");
        return false;
    }

    good &= readint(file, header->ir_size);
    good &= readint(file, header->e_size);
    good &= readint(file, header->box_size);
    good &= readint(file, header->vir_size);
    good &= readint(file, header->pres_size);
    good &= readint(file, header->top_size);
    good &= readint(file, header->sym_size);
    good &= readint(file, header->x_size);
    good &= readint(file, header->v_size);
    good &= readint(file, header->f_size);
    good &= readint(file, header->natoms);

    if (setfloatsize(header) == sizeof(double))
    {
        file->Double = true;
    }
    else
    {
        file->Double = false;
    }

    good &= readint(file, header->step);
    good &= readint(file, header->nre);
    good &= readreal(file, header->t);
    good &= readreal(file, header->lambda);

    return (good != 0);
}

bool Trr::readframedata(trr_file* file, trr_header* header, real* box, real* x, real* v, real* f)
{
    bool good = true;
    real arr;
    if (file->Double)
    {
        arr.d = new double[DIM*DIM];
    }
    else 
    {
        arr.f = new float[DIM*DIM];

    }


    if (header->box_size != 0)
    {
        good &= readrvec(file, box, DIM);
        //printf("box: %d\n", good);
    }
 
    if (header->vir_size != 0)
    {
        good &= readrvec(file, &arr, DIM);
        //printf("arr1: %d\n", good);
    }
    if (header->pres_size != 0)
    {
        good &= readrvec(file, &arr, DIM);
        //printf("arr2: %d\n", good);
    }
    
    if (header->x_size != 0)
    { 
       good &= readrvec(file, x, header->natoms);
       //printf("x: %d\n", good);
    }

    if (header->v_size != 0)
    {
        good &= readrvec(file, v, header->natoms);
        //printf("v: %d\n", good);
    }
    
    if (header->f_size != 0)
    {
        good &= readrvec(file, f, header->natoms);
        //printf("f: %d\n", good);
    }

    if (file->Double)
    {
        delete [] arr.d;
    }
    else
    {
        delete [] arr.f;
    }


    return (good != 0);
}

bool Trr::readxdr(trr_file* file, void* item, size_t arrsize, iotype type)
{
    bool good = true;

    unsigned char   uc, *ucptr = nullptr;
    char            c, *cptr = nullptr;
    float           f, *fptr = nullptr;
    float           farr[DIM];
    double          darr[DIM];
    double          d, *dptr = nullptr;
    int             i, icurpos, *iptr = nullptr;


    switch (type)
    {
        case REAL:
            if (file->Double)
            {   if (file->Write)
                {
                    d = *static_cast<double*>(item);
                }
                good = xdr_double(file->xdrptr, &d);
                if (item)
                {
                    *static_cast<double*>(item) = d;
                }
            }
            else
            {
                if (file->Write)
                {
                    f = *static_cast<float*>(item);
                }
                good = xdr_float(file->xdrptr, &f);
                if (item)
                {
                    *static_cast<float*>(item) = f;
                }
            }
            break;
        case INT:
            if (file->Write)
            {
                i = *static_cast<int*>(item);
            }
            good = xdr_int(file->xdrptr, &i);
            if (item)
            {
                *static_cast<int*>(item) = i;
            }
            break;
        case UCHAR:
            if (file->Write)
            {
                uc = *static_cast<unsigned char*>(item);
            }
            good = xdr_u_char(file->xdrptr, &uc);
            if (item)
            {
                *static_cast<unsigned char*>(item) = uc;
            }
            break;
        case CHAR:
            if (file->Write)
            {
                c = *static_cast<char*>(item);
            }
            good = xdr_char(file->xdrptr, &c);
            if (item)
            {
                *static_cast<char*>(item) = c;
            }
            break;
        case INTARR:
            iptr = static_cast<int*>(item);
            for (int i = 0; i < DIM; i++)
            {
                iptr[i] = icurpos;
                good = xdr_int(file->xdrptr, &icurpos);
                if (item)
                {
                    iptr[i] = icurpos;
                }
            }
            break;
        case REALARR:
            if (file->Double)
            {
                if (item && file->Write)
                {
                    for (int i = 0; i < DIM; i++)
                    {
                        printf("darrinit\n");
                        darr[i] = (static_cast<double*>(item))[i];
                    }
                }
                

                good = xdr_vector(file->xdrptr, reinterpret_cast<char*>(darr), DIM, static_cast<unsigned int>(sizeof(double)), reinterpret_cast<xdrproc_t>(xdr_double));

                if (item)
                {
                    for (int i = 0; i < DIM; i++)
                    {
                        (static_cast<double*>(item))[i] = darr[i];
                    }
                }
            }
            else
            {
                if (item && file->Write)
                {
                    for (int i = 0; i < DIM; i++)
                    {
                        printf("farrinit %d\n", i);
                        farr[i] = static_cast<float*>(item)[i];
                    }
                }
                good = xdr_vector(file->xdrptr, reinterpret_cast<char*>(farr), DIM, static_cast<unsigned int>(sizeof(float)), reinterpret_cast<xdrproc_t>(xdr_float));
                if (item)
                {
                    for (int i = 0; i < DIM; i++)
                    {
                        (static_cast<float*>(item))[i] = farr[i];
                    }
                }
            }
            break;
        case CHARARR:
            int slen;
            if (file->Write && item)
            {
                slen = strlen(static_cast<char*>(item)) + 1;
            }
            else
            {
                slen = 0;
            }
            if (xdr_int(file->xdrptr, &slen) <= 0)
            {
                printf("strlenfail\n");
                good = true;
                break;
            }
            if (!item && file->Read)
            {
                
                cptr = static_cast<char*>(calloc(slen, sizeof(char)));
            }
            else
            {
                cptr = static_cast<char*>(item);
            }
            if (!cptr)
            {
                good = true;
                break;
            }
            good = xdr_string(file->xdrptr, &cptr, slen);
            if (file->Read && !item)
            {
                free(cptr);
            }
            break;

        case RVECARR:
            if (file->Double)
            { 
                for (int i = 0; i < arrsize; i++)
                {
                    dptr = *(static_cast<double**>(item)) + i*DIM;
                    good = readxdr(file, dptr, 1, REALARR);
                }
            }
            else
            {
                for (int i = 0; i < arrsize; i++)
                {
                    fptr = *(static_cast<float**>(item)) + i*DIM;
                    good = readxdr(file, fptr, 1, REALARR);
                }
            }
            break;
    }

    return (good != 0);
}