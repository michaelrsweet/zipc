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
  char		filename[256],		/* Filename for archive */
		*ptr,			/* Pointer into filename */
                data_bin[1024],         /* Random data for data.bin */
                buffer[1024];           /* Read back buffer */
  ssize_t       bytes;                  /* Bytes read */


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
    printf("zipcOpen(\"%s\", \"w\"): %s\n", argv[1], strerror(errno));
    return (1);
  }

  printf("zipcOpen(\"%s\", \"w\"): OK\n", argv[1]);

 /*
  * Add a file using a constant string...
  */

  if (zipcCreateFileWithString(zc, "testzipc.txt", "application/testzipc"))
  {
    printf("zipcCreateFileWithString(\"testzipc.txt\"): %s\n", zipcError(zc));
    status = 1;
  }
  else
    puts("zipcCreateFileWithString(\"testzipc.txt\"): OK");

 /*
  * Create a directory...
  */

  if (zipcCreateDirectory(zc, "META-INF")) /* Omitting trailing slash for test */
  {
    printf("zipcCreateDirectory(\"META-INF\"): %s\n", zipcError(zc));
    status = 1;
  }
  else
    puts("zipcCreateDirectory(\"META-INF\"): OK");

 /*
  * Create a file and write using a bunch of different functions...
  */

  if ((zf = zipcCreateFile(zc, "META-INF/testzipc.xml", 0)) != NULL)
  {
    puts("zipcCreateFile(\"META-INF/testzipc.xml\", 0): OK");

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
      printf("zipcFileFinish(\"META-INF/testzipc.xml\"): %s\n", zipcError(zc));
      status = 1;
    }
  }
  else
  {
    printf("zipcCreateFile(\"META-INF/testzipc.xml\", 0): %s\n", zipcError(zc));
    status = 1;
  }

 /*
  * Create another directory...
  */

  if (zipcCreateDirectory(zc, "CONTENTS/"))
  {
    printf("zipcCreateDirectory(CONTENTS/): %s\n", zipcError(zc));
    status = 1;
  }
  else
    puts("zipcCreateDirectory(CONTENTS/): OK");

 /*
  * Create a random data file...
  */

  for (i = 0; i < sizeof(data_bin); i ++)
    data_bin[i] = random();

  if ((zf = zipcCreateFile(zc, "CONTENTS/data.bin", 1)) != NULL)
  {
    puts("zipcCreateFile(\"CONTENTS/data.bin\", 1): OK");

    if (zipcFileWrite(zf, data_bin, sizeof(data_bin)))
    {
      printf("zipcFileWrite: %s\n", zipcError(zc));
      status = 1;
    }

    if (zipcFileFinish(zf))
    {
      printf("zipcFileFinish(\"CONTENTS/data.bin\"): %s\n", zipcError(zc));
      status = 1;
    }
  }
  else
  {
    printf("zipcCreateFile(\"CONTENTS/data.bin\", 1): %s\n", zipcError(zc));
    status = 1;
  }

 /*
  * Add files from the command-line...
  */

  for (i = 2; i < argc; i ++)
  {
    if ((ptr = strrchr(argv[i], '/')) != NULL)
      ptr ++;
    else
      ptr = argv[i];

    snprintf(filename, sizeof(filename), "CONTENTS/%s", ptr);

    if ((ptr = strrchr(argv[i], '.')) != NULL)
      ptr ++;
    else
      ptr = "o";

    if (zipcCopyFile(zc, filename, argv[i], strcmp(ptr, "o"), 1))
    {
      printf("zipcCopyFile(\"%s\", \"%s\"): %s\n", filename, argv[i], zipcError(zc));
      status = 1;
    }
    else
      printf("zipcCopyFile(\"%s\", \"%s\"): OK\n", filename, argv[i]);
  }

  if (zipcClose(zc))
  {
    printf("zipcClose(\"%s\"): %s\n", argv[1], strerror(errno));
    status = 1;
  }
  else
    printf("zipcClose(\"%s\"): OK\n", argv[1]);

 /*
  * Read tests...
  */

  if ((zc = zipcOpen(argv[1], "r")) == NULL)
  {
    printf("zipcOpen(\"%s\", \"r\"): %s\n", argv[1], strerror(errno));
    return (1);
  }

  printf("zipcOpen(\"%s\", \"r\"): OK\n", argv[1]);

  if ((zf = zipcOpenFile(zc, "testzipc.txt")) != NULL)
  {
    puts("zipcOpenFile(\"testzipc.txt\"): OK");
    if ((bytes = zipcFileRead(zf, buffer, sizeof(buffer))) > 0)
    {
      buffer[bytes] = '\0';

      if (!strcmp(buffer, "application/testzipc"))
      {
        puts("zipcFileRead: OK");
      }
      else
      {
        printf("zipcFileRead: Bad (%s)\n", buffer);
        status = 1;
      }
    }
    else
    {
      printf("zipcFileRead: %s\n", zipcError(zc));
      status = 1;
    }

    if (zipcFileFinish(zf))
    {
      printf("zipcFileFinish(\"testzipc.txt\"): %s\n", zipcError(zc));
      status = 1;
    }
  }
  else
  {
    printf("zipcOpenFile(\"testzipc.txt\"): %s\n", zipcError(zc));
    status = 1;
  }

  if ((zf = zipcOpenFile(zc, "CONTENTS/data.bin")) != NULL)
  {
    puts("zipcOpenFile(\"CONTENTS/data.bin\"): OK");
    if ((bytes = zipcFileRead(zf, buffer, sizeof(buffer))) > 0)
    {
      if (bytes == sizeof(buffer) && !memcmp(buffer, data_bin, sizeof(buffer)))
      {
        puts("zipcFileRead: OK");
      }
      else
      {
        puts("zipcFileRead: Bad (binary data doesn't match)");
        status = 1;
      }
    }
    else
    {
      printf("zipcFileRead: %s\n", zipcError(zc));
      status = 1;
    }

    if (zipcFileFinish(zf))
    {
      printf("zipcFileFinish(\"CONTENTS/data.bin\"): %s\n", zipcError(zc));
      status = 1;
    }
  }
  else
  {
    printf("zipcOpenFile(\"CONTENTS/data.bin\"): %s\n", zipcError(zc));
    status = 1;
  }

  if (zipcClose(zc))
  {
    printf("zipcClose(\"%s\"): %s\n", argv[1], strerror(errno));
    status = 1;
  }
  else
    printf("zipcClose(\"%s\"): OK\n", argv[1]);

  return (status);
}

