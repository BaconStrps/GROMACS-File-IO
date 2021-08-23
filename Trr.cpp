#include "Trr.h"
#include "xdr.h"
#define readint(file, item)             readxdr(file, &(item), 0, iotype::INT)
#define readreal(file, item)          readxdr(file, &(item), 0, iotype::REAL)
#define readchar(file, item)            readxdr(file, &(item), 0, iotype::CHAR)
#define readuchar(file, item)           readxdr(file, &(item), 0, iotype::UCHAR)
#define readrvec(file, item, nitem)      readxdr(file, item, nitem, iotype::RVECARR)
#define readintarr(file, item)          readxdr(file, &(item), 0, iotype::INTARR)
#define readrealarr(file, item)       readxdr(file, &(item), 0, iotype::REALARR)
#define readstr(file, item)             readxdr(file, &(item), 0, iotype::CHARARR)

Trr::Trr(const char* fn)
{
    first = true;
    failed = false;
    file.fp.open(fn, std::ios_base::binary);
    file.fp.seekg(0, file.fp.end);
    file.filesize = file.fp.tellg();
    file.buf = static_cast<char*>(calloc(file.filesize, sizeof(char)));
    file.pos = file.buf;

    file.fp.seekg(0);

    file.fp.read(file.buf, file.filesize);
    readheader(&file, &header);

    if (file.Double)
    {
        box.d =    new double[DIM*DIM];
                    // long long to avoid any overflow issues. VS warned me
        x.d =      new double[DIM*static_cast<long long>(header.natoms)];
        v.d =      new double[DIM*static_cast<long long>(header.natoms)];
        f.d =      new double[DIM*static_cast<long long>(header.natoms)];
    }
    else
    {
        box.f =    new float[DIM*DIM];
        x.f =      new float[DIM*static_cast<long long>(header.natoms)];
        v.f =      new float[DIM*static_cast<long long>(header.natoms)];
        f.f =      new float[DIM*static_cast<long long>(header.natoms)];
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
    delete [] file.buf;

    file.fp.close();

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

    first = false;

    return failed;
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
    //printf("%s\n", buf);

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

   /* printf("%d\n", header->ir_size);
    printf("%d\n", header->e_size);
    printf("%d\n", header->box_size);
    printf("%d\n", header->vir_size);
    printf("%d\n", header->pres_size);
    printf("%d\n", header->top_size);
    printf("%d\n", header->sym_size);
    printf("%d\n", header->x_size);
    printf("%d\n", header->v_size);
    printf("%d\n", header->f_size);
    printf("%d\n", header->natoms);*/

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
       // printf("%p\n", box);
        good &= readrvec(file, box, DIM);
    }
 
    if (header->vir_size != 0)
    {
        good &= readrvec(file, &arr, DIM);
    }
    if (header->pres_size != 0)
    {
        good &= readrvec(file, &arr, DIM);
    }
    
    if (header->x_size != 0)
    { 
       good &= readrvec(file, x, header->natoms);
    }

    if (header->v_size != 0)
    {
        good &= readrvec(file, v, header->natoms);
    }
    
    if (header->f_size != 0)
    {
        good &= readrvec(file, f, header->natoms);
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

    char            *cptr = nullptr;
    float           f, *fptr = nullptr;
    float           farr[DIM];
    double          darr[DIM];
    double          d, *dptr = nullptr;
    int             i;

    if (file->pos >= file->buf + file->filesize)
    {
        return false;
    }


    switch (type)
    {
    case iotype::REAL:
            if (file->Double)
            {   if (file->Write)
                {
                    d = *static_cast<double*>(item);
                }
                good = xdr_double(&file->pos, &d);
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
                good = xdr_float(&file->pos, &f);
                if (item)
                {
                    *static_cast<float*>(item) = f;
                }
            }
            break;
    case iotype::INT:
            if (file->Write)
            {
                i = *static_cast<int*>(item);
            }
            good = xdr_int(&file->pos, &i);
            if (item)
            {
                *static_cast<int*>(item) = i;
            }
            break;
    case iotype::REALARR:
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
                

                good = xdr_vector(&file->pos, reinterpret_cast<char*>(darr), DIM, static_cast<unsigned int>(sizeof(double)), &xdr_double);

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
                good = xdr_vector(&file->pos, reinterpret_cast<char*>(farr), DIM, static_cast<unsigned int>(sizeof(float)), &xdr_float);
                if (item)
                {
                    for (int i = 0; i < DIM; i++)
                    {
                        (static_cast<float*>(item))[i] = farr[i];
                    }
                }
            }
            break;
    case iotype::CHARARR:
            size_t slen;
            if (file->Write && item)
            {
                slen = strlen(static_cast<char*>(item)) + 1;
            }
            else
            {
                slen = 0;
            }
            if (!xdr_int(&file->pos, &slen))
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
            good = xdr_string(&file->pos, cptr, slen);
            if (file->Read && !item)
            {
                free(cptr);
            }
            break;

    case iotype::RVECARR:
            if (file->Double)
            {
                dptr = *static_cast<double**>(item);
                good = xdr_vector(&file->pos, reinterpret_cast<char*>(dptr), sizeof(double), arrsize*DIM, &xdr_double);
            }
            else
            {
                fptr = *static_cast<float**>(item);
                good = xdr_vector(&file->pos, reinterpret_cast<char*>(fptr), sizeof(float), arrsize*DIM, &xdr_float);
            }
            break;
    }

    return (good != 0);
}