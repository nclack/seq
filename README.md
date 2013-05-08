Seq
===

Reader for Streampix 3 (Norpix) movie files.  These files typically have a `.seq` extension.

## Caveats

- **Read only**
- Untested for anything other than 8 bit data.

## Example

### In Matlab
```matlab
  h=Seq('my.seq');
  imshow(read(h,length(h))); % read the last frame
```

### In C
```c
  #include "seq.h"
  ...
  SeqReader *f = Seq_Open( path );
  void *buf=0;
  if(f)
  { buf=malloc(f->sizebytes);
    im=Seq_Read_Image_To_Buffer(f,0,buf);
    Seq_Close(f);
  }
  ...
  free(buf);
```

## Building

Requires [CMake](http://www.cmake.org/).

You also need a C++ compiler and, optionally, Matlab.

Building may require an internet connection.  The build process may
attempt to download [Eigen](http://eigen.tuxfamily.org/), which is
the linear algebra library used under the hood.

After installing CMake, and making sure it's on your path.  The first steps look the same for most operating systems.  Open a terminal, change directory to where you cloned this repository, and then type:

### Configure

#### Everywhere
```bash
  mkdir build
  cd build
  cmake ..
```
Note that configuring as a Release build results in much faster (~10X) code than a Debug build.
To make sure you have a release build use:
```
  cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Build

#### On OS X and *nix:
```bash
   make package
```
This will build a .tgz file you can unpack somewhere.  All the matlab stuff will be assembled in a matlab subdirectory in that package.

Note you might want to use `cmake-gui` to edit the kinds of packages that get generated.  See the `CPACK` options.

#### On Windows
with [Microsoft Visual Studio](http://www.microsoft.com/visualstudio/eng/products/visual-studio-express-products):

1. Open the `tps.sln` file that has been created in the build directory.

2. Build the PACKAGE target in the solution that opens up.
This should[1] build an executable installer in the build directory.  It will install to 
something like: "C:\Program Files\seq".  You can also configure the build to produce a .zip file archive if you don't want the installer.


[1]: My CMakeLists.txt might be missing some stuff required for the NSIS installer.  This is untested at the moment.
