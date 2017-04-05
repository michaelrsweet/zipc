/*
 * ZIP dumping program for the ZIP container mini-library.
 *
 *     https://github.com/michaelrsweet/zipc
 *
 * Usage:
 *
 *     ./dumpzip filename
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/*
 * Constants...
 */

#define LOCAL_FILE_HEADER	0x04034b50


/*
 * Local functions...
 */

static unsigned	read_u16(FILE *fp);
static unsigned read_u32(FILE *fp);


/*
 * 'main()' - Main entry for dump program.
 */

int					/* O - Exit status */
main(int  argc,				/* I - Number of command-line arguments */
     char *argv[])			/* I - Command-line arguments */
{
  FILE		*fp;			/* File to read from */
  unsigned char	buffer[8192];		/* Write buffer */
  unsigned	sig;			/* Signature */


 /*
  * Check command-line...
  */

  if (argc != 2)
  {
    puts("Usage: ./dumpzip filename");
    return (1);
  }

 /*
  * Open the ZIP file...
  */

  fp = fopen(argv[1], "rb");
  if (!fp)
  {
    perror(argv[1]);
    return (1);
  }

 /*
  * Read until EOF...
  */

  while ((sig = read_u32(fp)) != 0)
  {
    if (sig == LOCAL_FILE_HEADER)
    {
      unsigned compressed_size;
      unsigned filename_length;
      unsigned extra_field_length;
      unsigned datetime;

      puts("Local File Header");
      puts("-----------------");
      printf("  version needed to extract = %04x\n", read_u16(fp));
      printf("   general purpose bit flag = %04x\n", read_u16(fp));
      printf("         compression method = %d\n", read_u16(fp));
      datetime = read_u16(fp);
      printf("         last mod file time = %04x (%02d:%02d:%02d)\n", datetime, (datetime >> 11) & 31, (datetime >> 5) & 63, 2 * (datetime & 31));
      datetime = read_u16(fp);
      printf("         last mod file date = %04x (%04d-%02d-%02d)\n", datetime, ((datetime >> 9) & 127) + 1980, (datetime >> 5) & 15, datetime & 31);
      printf("                     crc-32 = %08x\n", read_u32(fp));
      printf("            compressed_size = %u\n", compressed_size = read_u32(fp));
      printf("          uncompressed_size = %u\n", read_u32(fp));
      printf("           file name length = %u\n", filename_length = read_u16(fp));
      printf("         extra field length = %u\n", extra_field_length = read_u16(fp));

      fread(buffer, 1, filename_length, fp);
      buffer[filename_length] = '\0';
      printf("                   filename = %s\n", buffer);

      while (extra_field_length > 0)
      {
        unsigned header_id = read_u16(fp);
        unsigned header_len = read_u16(fp);

        extra_field_length -= header_len + 4;

        if (header_id == 0x5455)
        {
	  printf("                extra field = 0x%04x (extended timestamp - %u bytes)\n", header_id, header_len);
        }
        else if (header_id == 0x7875)
        {
	  printf("                extra field = 0x%04x (???? - %u bytes)\n", header_id, header_len);
        }
        else
        {
	  printf("                extra field = 0x%04x (%u bytes)\n", header_id, header_len);
        }

        fseek(fp, header_len, SEEK_CUR);
      }

      if (compressed_size > 0)
        fseek(fp, compressed_size, SEEK_CUR);
    }
    else
    {
      printf("Unknown signature 0x%08x, aborting.\n", sig);
      break;
    }
  }

  fclose(fp);

  return (0);
}


/*
 * 'read_u16()' - Read a 16-bit unsigned integer.
 */

static unsigned				/* O - 16-bit unsigned integer value */
read_u16(FILE *fp)			/* I - File to read from */
{
  unsigned char	buffer[2];		/* Buffer */


  if (fread(buffer, sizeof(buffer), 1, fp) < 1)
  {
    puts("EOF");
    return (0);
  }
  else
    return (buffer[0] | (buffer[1] << 8));
}


/*
 * 'read_u32()' - Read a 32-bit unsigned integer.
 */

static unsigned				/* O - 32-bit unsigned integer value */
read_u32(FILE *fp)			/* I - File to read from */
{
  unsigned char	buffer[4];		/* Buffer */


  if (fread(buffer, sizeof(buffer), 1, fp) < 1)
  {
    puts("EOF");
    return (0);
  }
  else
    return (buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24));
}
