# zipc

This is a simple ZIP container "library" consisting of a C source file and
accompanying header file.  Currently I have only implemented writing of files
smaller than 4GB with or without deflate compression.  There is no support for
signatures, encryption, password protection, etc. as those features of ZIP are
typically not used for ZIP container-based formats.

I'm providing this as open source under the "new" 2-clause BSD license which
allows you do pretty much do whatever you like with it.  Please do provide
feedback and report bugs to the Github project page at:

    https://github.com/michaelrsweet/zipc

so that everyone can benefit.


## Requirements

You'll need ZLIB and a C compiler.


## How to Incorporate With Your Project

Add the `zipc.c` and `zipc.h` files to your project.  Include the `zipc.h`
header in any file that needs to write ZIP containers.


## "Kicking the Tires"

The supplied makefile allows you to build the unit tests on Linux and macOS (at
least), which verify that all of the functions work as expected to produce a
ZIP container called `testzipc.dat`.  Currently the only way to validate the
output is with a ZIP extractor like `unzip`, e.g.:

    make
    unzip -lv testzipc.dat

The unzip command should produce output that looks similar to this:

    Archive:  testzipc.dat
     Length   Method    Size  Cmpr    Date    Time   CRC-32   Name
    --------  ------  ------- ---- ---------- ----- --------  ----
          20  Stored       20   0% 04-05-2017 08:22 e2caffa4  testzipc.txt
           0  Stored        0   0% 04-05-2017 08:22 00000000  META-INF/
         139  Stored      139   0% 04-05-2017 08:22 ae7d2a18  META-INF/testzipc.xml
           0  Stored        0   0% 04-05-2017 08:22 00000000  CONTENTS/
        2649  Defl:X     1139  57% 04-05-2017 08:22 0f066577  CONTENTS/zipc.h
        5422  Defl:X     1955  64% 04-05-2017 08:22 8c8d34e8  CONTENTS/testzipc.c
       23480  Defl:X     5720  76% 04-05-2017 08:22 9eb6a0e7  CONTENTS/zipc.c
        9244  Defl:X     3689  60% 04-05-2017 08:22 e59302d9  CONTENTS/testzipc.o
       23608  Defl:X    10126  57% 04-05-2017 08:22 de8bcbd8  CONTENTS/zipc.o
    --------          -------  ---                            -------
       64562            22788  65%                            9 files


## Legal Stuff

Copyright 2017 by Michael R Sweet.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
