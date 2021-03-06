#
# Unit test makefile for ZIP container mini-library.
#
#     https://github.com/michaelrsweet/zipc
#
# Copyright 2017-2021 by Michael R Sweet.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

CC	=	gcc
CFLAGS	=	-g -Wall
LIBS	=	-lz
OBJS	=	testzipc.o zipc.o

.SUFFIXES:	.c .o
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<


all:	testzipc

clean:
	rm -f testzipc $(OBJS)
	rm -f dumpzip

sanitizer:
	$(MAKE) clean
	$(MAKE) CFLAGS="-g -Wall -fsanitize=address" all

testzipc:	$(OBJS)
	$(CC) $(CFLAGS) -o testzipc $(OBJS) $(LIBS)

test:	testzipc
	./testzipc testzipc.dat zipc.h $(OBJS:.o=.c) $(OBJS)

$(OBJS):	zipc.h

dumpzip:	dumpzip.c
	$(CC) $(CFLAGS) -o dumpzip dumpzip.c

# Scan code with the Clang static analyzer <https://clang-analyzer.llvm.org>
clang:
	clang $(CFLAGS) -Werror --analyze $(OBJS:.o=.c)
	rm -rf $(OBJS:.o=.plist)


# Scan code with Cppcheck <http://cppcheck.sourceforge.net>
cppcheck:
	cppcheck --template=gcc --addon=cert.py --suppress=cert-MSC24-C --suppress=cert-EXP05-C --suppress=cert-API01-C $(OBJS:.o=.c) 2>cppcheck.log
	@test -s cppcheck.log && (echo ""; echo "Errors detected:"; echo ""; cat cppcheck.log; exit 1) || exit 0
