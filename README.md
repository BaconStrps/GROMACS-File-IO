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
  - char* grname   (Name for the specific group)
  - int* start     (Starting indexes for that group)
  - int* end       (Ending indexes for that group)
  - int size       (Amount of swaps)
  
  Indexes for the group follow in numerical order. If you had a file like  
  \[ MEMB ]  
  1 2 5 6  
  \[ WATER ]  
  3 4
  
  For the MEMB group,  
  start\[0] would contain 1  
  end\[0] would contain 2  
  start\[1] would contain 5  
  end\[1] would contain 6  

## Topol
The Topol class must be constructed with the filename when declared with a string literal/c string.

The Topol class provides a data structure for each residue in order along with their residue amounts named *residue*.  

The best way to use Topol is to use topol.getResidues(int& size) which will return a residue array and initialize *size* to the size of the array. 

  ***residue***
  - char* resname (Name for the residue group)
  - int nmolc     (Number of molecules for that residue)


# Trr
The Trr class must be constructed with the filename when delcared with a string literal/c string.  

The Trr class provides a way to read each frame sequentially from a .trr file.   

To use, you should declare a .trr object with a valid filename. Then, when you'd like to read a frame you call the readFrame() method which will return false if it is successful.  
After, you should call the getPrecision() method which will return true if the .trr is double-precision, or false if it is single-precision. This is because GROMACS can write to a trr in either double or single precision and you either need to know ahead of time or figure it out on the fly. If you know the precision ahead of time you can skip the getPrecision() step.  

Then depending on whether you want the coordinates, velocities, or forces, you should call the getX\<type\>() getV\<type\>() or getF\<type\>() method templates with the corresponding type (double or float) which will return a pointer to an array of (DIM \* natoms) size. To index through this array you should do something like x\[j + i\*DIM\]. The array is set up like so  

 - x\[i\*DIM\] = {(1st), (2nd), (3rd)}  
  
in x\[j + i\*DIM\], j will index (1st) (2nd) or (3rd) and of course, i\*DIM will index i\*DIM.  
  
You can get natoms by calling the getnatoms() method which will return the amount of atoms (int).  
# To-Do
- .tpr file additions
- .edr file additions
