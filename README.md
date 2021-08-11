# GROMACS-File-IO
A GROMACS file IO library intended for quick, reliable, and safe reading of GROMACS files directly.

Currently only .gro files are supported but I will be updating this with new additions for each file.

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

# To-Do
- .top/.itp file additions
- .trr file aditions
- .tpr file additions
- .edr file additions
