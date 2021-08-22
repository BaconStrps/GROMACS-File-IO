/*
 * xdr.h, External Data Representation Serialization Routines.
 *
 * Copyright (c) 2010, 2012, Oracle America, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name of the "Oracle America, Inc." nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /*
    This is my own rewritten xdr.h that doesn't require any external non standard headers.
    If you rewrite this code or use this code outside of this project I have a few pointers.

        1. This was specifically written to work with .trr files. I cannot guarantee it'll work on other xdr/rpc systems.
        2. If you do use this on other systems, make sure to verify it works with your system using the actual xdr implementation first.
        3. This only works on c stdio xdr files there is no code to make it work with other streams but I can't guarantee it wont work on other streams.
        4. This only has a few of the xdr filters but you could make the other filters out of these ones I believe.

    If you do plan to use this code in an outside project please visit the link below for more information on XDR.

    https://docs.oracle.com/cd/E18752_01/html/816-1435/xdrnts-1.html
    
 */

#ifndef XDR_PORT
#define XDR_PORT
#include <stdio.h>


typedef bool (*xdrproc)(FILE*, void*);

bool xdr_int(FILE* file, void* dat)
{
    int temp;
    char* ptr = reinterpret_cast<char*>(dat);
    char* ptemp = reinterpret_cast<char*>(&temp);
    if (!fread(ptemp, sizeof(int), 1, file))
    {
        return false;
    }
    for (int i = 0; i < 4; i++)
    {
        ptr[i] = ptemp[3 - i];
    }
    return true;
}

bool xdr_long(FILE* file, void* dat)
{
    int temp;
    char* ptr = reinterpret_cast<char*>(dat);
    char* ptemp = reinterpret_cast<char*>(&temp);
    if (!fread(ptemp, sizeof(long), 1, file))
    {
        return false;
    }
    for (int i = 0; i < 8; i++)
    {
        ptr[i] = ptemp[7 - i];
    }

    return true;
}

bool xdr_string(FILE* file, void* dat, const int& strlen) //this is specifically written differently for .trr files
{
    char arr[4];
    int actualstrlen;
    xdr_int(file, &actualstrlen); // god damnit gromacs
    printf("actualstrlen: %d\n", actualstrlen);
    if (fread(dat, sizeof(char), actualstrlen, file) < actualstrlen)
    {
        printf("fail???\n");
        return false;
    }
    if (actualstrlen % 4)
    {
        printf("its in here???\n");
        if(fread(arr, sizeof(char), 4 - actualstrlen%4, file) < 4 - actualstrlen%4)
        {
            return false;
        }
    }
    reinterpret_cast<char*>(dat)[actualstrlen] = 0; //why would you do this to me

    return true;
}

bool xdr_float(FILE* file, void* dat)
{
    return xdr_int(file, dat);
}

bool xdr_double(FILE* file, void* dat)
{
    return xdr_long(file, dat);
}

bool xdr_vector(FILE* file, char* dat, const unsigned long& elsize, const unsigned long& elcount,  xdrproc f)
{
    for (int i = 0; i < elcount*elsize; i+=elsize)
    {
        if (!((*f)(file, (dat + i))))
        {
            return false;
        }
    }
    return true;
}

#endif