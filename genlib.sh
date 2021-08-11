#!/bin/bash

gcc -c Index.cpp -lstdc++
gcc -c Gro.cpp -lstdc++

ar rvs -o libgfile.a Gro.o Index.o