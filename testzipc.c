/*
 * Unit test program for the ZIP container mini-library.
 *
 *     https://github.com/michaelrsweet/zipc
 *
 * Usage:
 *
 *     ./testzipc filename.zip [files to add]
 *
 * Copyright 2017 by Michael R Sweet.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Include necessary headers...
 */

#include "zipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/*
 * 'main()' - Main entry for unit test program.
 */

int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line arguments */
     char *argv[])			/* I - Command-line arguments */
{
  int		status = 0;		/* Exit status */
  zipc_t	*zc;			/* ZIP container */
  zipc_file_t	*zf;			/* ZIP container file */
  int		i;			/* Looping var */
  FILE		*fp;			/* File to read from */
  unsigned char	buffer[8192];		/* Write buffer */
  size_t	bytes;			/* Bytes read */


 /*
  * Check command-line...
  */

  if (argc < 2)
  {
    puts("Usage: ./testzipc filename.zip [... other files to add ...]");
    return (1);
  }

 /*
  * Open the container...
  */

  if ((zc = zipcOpen(argv[1], "w")) == NULL)
  {
    printf("zipcOpen(%s): %s\n", argv[1], strerror(errno));
    return (1);
  }

  printf("zipcOpen(%s): OK\n", argv[1]);

 /*
  * Add a file using a constant string...
  */

  if (zipcCreateFileWithString(zc, "testzipc.txt", "application/testzipc"))
  {
    printf("zipcCreateFileWithString(testzipc.txt): %s\n", zipcError(zc));
    status = 1;
  }
  else
    puts("zipcCreateFileWithString(testzipc.txt): OK");

 /*
  * Create a file and write using a bunch of different functions...
  */

  if ((zf = zipcCreateFile(zc, "testzipc.xml", 0)) != NULL)
  {
    puts("zipcCreateFile(testzipc.xml): OK");

    if (zipcFilePuts(zf, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"))
    {
      printf("zipcFilePuts: %s\n", zipcError(zc));
      status = 1;
    }

    if (zipcFilePrintf(zf, "<integer>%d</integer>\n", 42))
    {
      printf("zipcFilePrintf: %s\n", zipcError(zc));
      status = 1;
    }

    if (zipcFileXMLPrintf(zf, "<string>%s</string>\n", "\"testzipc\" <string> value & test string."))
    {
      printf("zipcFileXMLPrintf: %s\n", zipcError(zc));
      status = 1;
    }

    if (zipcFileFinish(zf))
    {
      printf("zipcFileFinish(testzipc.xml): %s\n", zipcError(zc));
      status = 1;
    }
  }
  else
  {
    printf("zipcCreateFile(testzipc.xml): %s\n", zipcError(zc));
    status = 1;
  }

 /*
  * Add files from the command-line...
  */

  for (i = 2; i < argc; i ++)
  {
    if ((fp = fopen(argv[i], "rb")) == NULL)
    {
      perror(argv[i]);
      status = 1;
      continue;
    }

    if ((zf = zipcCreateFile(zc, argv[i], 1)) != NULL)
    {
      printf("zipcCreateFile(%s): OK\n", argv[i]);

      while ((bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0)
      {
	if (zipcFileWrite(zf, buffer, bytes))
	{
	  printf("zipcFileWrite(%s): %s\n", argv[i], zipcError(zc));
	  status = 1;
	}
      }

      if (zipcFileFinish(zf))
      {
	printf("zipcFileFinish(%s): %s\n", argv[i], zipcError(zc));
	status = 1;
      }

      fclose(fp);
    }
    else
    {
      printf("zipcCreateFile(%s): %s\n", argv[i], zipcError(zc));
      status = 1;
    }
  }

  if (zipcClose(zc))
  {
    printf("zipcClose: %s\n", strerror(errno));
    return (1);
  }

  puts("zipcClose: OK");

  return (status);
}

