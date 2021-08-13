# GROMACS-File-IO
A GROMACS file IO library intended for quick, reliable, and safe reading of GROMACS files directly.

Currently only .gro files are supported but I will be updating this with new additions for each file.  

To use: Run genlib.sh (assumes you have GCC installed), then link the outputted libgfile.a file with whatever you are compiling. Be sure to include all header files inside of your source directoy and include gfile.h inside of your source.

## Gro
The Gro class must be constructed with the filename when declared with a string literal/c string. 

Currently the most useful feature for the Gro class is the getdata() function which will give you a c string array for every row that you specify (char**).  
Note: The memory storing the data is re-allocated after calling getdata() for the 2nd time. If you wish to change the data returned by the function you may call it again, just be wary as the previous pointer may be pointing to useless memory.

  Usage: Gro.getdata(int start, int end, const char* dat)
  - start and end are in atom numbers specifically located in column 3 of a .gro file.
  - dat specifies what columns you want returned.
    - Residue Number: "rnum"
    - Residue Name:   "rnam"
    - Atom Name:      "anam"
    - Atom Number:    "anum" (kinda useless?)
    - X position:     "x"
    - Y position:     "y"
    - Z position:     "z"
    - X velocity:     "vx"
    - Y velocity:     "vy"
    - Z velocity:     "vz"

## Index
The Index class must be constructed with the filename when declared with a string literal/c string. 

The Index class provides a data structure for each index group named *group*. The groups may be retrieved from the class using the member function getGroups() which takes in an integer reference for the size of the *group* array held by the Index object and returns a *group* pointer for the array of *groups*.  
  
  ***group***
  - char* grname  (Name for the specific group)
  - int start     (Starting index for that group)
  - int end       (Ending index for that group)

## Topol
The Topol class must be constructed with the filename when declared with a string literal/c string.

The Topol class provides a data structure for each residue in order along with their residue amounts named *residue*.  

The best way to use Topol is to use topol.getResidues(int& size) which will return a residue array and initialize *size* to the size of the array. 

  ***residue***
  - char* resname (Name for the residue group)
  - int nmolc     (Number of molecules for that residue)

# To-Do
- .top/.itp file additions
- .trr file aditions
- .tpr file additions
- .edr file additions
