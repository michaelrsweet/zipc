# zipc

This is a simple ZIP container "library" consisting of a C source file and
accompanying header file.  The ZIP format is documented in the PKWARE
[APPNOTE.TXT - .ZIP File Format Specification](http://www.pkware.com/appnote).

Currently I have only implemented reading and writing of files smaller than 4GB
with or without deflate compression.  There is no support for signatures,
encryption, password protection, etc. as those features of ZIP are typically not
used for ZIP container-based formats.

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

If you only use the ZIP reading functionality, define the `ZIPC_ONLY_READ`
preprocessor macro.  If you only use the ZIP writing functionality, define the
`ZIPC_ONLY_WRITE` preprocessor macro.  Both will reduce the size of the code
that is included with your project.


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
        1024  Defl:X      512  50% 04-05-2017 08:22 0f066577  CONTENTS/data.bin
        2649  Defl:X     1139  57% 04-05-2017 08:22 0f066577  CONTENTS/zipc.h
        5422  Defl:X     1955  64% 04-05-2017 08:22 8c8d34e8  CONTENTS/testzipc.c
       23480  Defl:X     5720  76% 04-05-2017 08:22 9eb6a0e7  CONTENTS/zipc.c
        9244  Defl:X     3689  60% 04-05-2017 08:22 e59302d9  CONTENTS/testzipc.o
       23608  Defl:X    10126  57% 04-05-2017 08:22 de8bcbd8  CONTENTS/zipc.o
    --------          -------  ---                            -------
       64562            22788  65%                            9 files


## Changes in vCURRENT

- Fixed some issues when building using Visual Studio.


## Changes in v1.2

- Added C++ header wrapper.
- Added ZIP read support using the `zipcOpenFile`, `zipcFileGets`,
  `zipcFileRead`, `zipcFileXMLGets`, and `zipcXMLGetAttribute` functions.
- The `zipcFileXMLPrintf` function now supports `%f`.
- Fixed builds using Visual C++.


## Changes in v1.1

The following changes were made for v1.1:

- Added the `zipcCopyFile` function.
- Now update the CRC and lengths in the file headers for compatibility with all
  EPUB and OPC applications.


## Legal Stuff

Copyright 2017-2018 by Michael R Sweet.

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
