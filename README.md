Jacquin's algorithm for images compression
==========================================

**Disclamer** : this is rough and dirty, it's an old project that I found
amusing at the time, but the code may be in the "write only" category.

You need openCV (see their files in the include folder for licenses) to compile
and run this. Apart from that, the Makefile is human readable (I guess?) and
the execution is straightforward : `./jacquin -z file.jpg` outputs a compressed
.jpgz file (hey, the naming convention is shit, OK, but at the time I had
a rough understanding of who's who in the great world of codecs and filetypes)
that you can then decompress using `jacquin -u file.jpgz`. It should create
a folder with the iterations of the decompression, as well as the final file
somewhere (?).

Note that the jpgz file is **highly** compressible using standard text compression
algorithms.

Parameters should be changed before compiling, everything is in the define.h
file, have fun to understand who's who in there.

More information available
[here](http://www.dptinfo.ens-cachan.fr/~msableme/TIPE/)

Otherwise you can ask me, I don't really want to read this right now but I kind
of remember the global intuition behind this.
