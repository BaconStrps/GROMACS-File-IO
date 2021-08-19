#!/bin/bash

gcc -c Index.cpp -lstdc++
gcc -c Gro.cpp -lstdc++
gcc -c Topol.cpp -lstdc++
gcc -c Trr.cpp -lstdc++

ar rvs -o libgfile.a Gro.o Index.o Topol.o Trajdump.o
