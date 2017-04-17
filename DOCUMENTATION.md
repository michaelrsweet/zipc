# How to Use the zipc "Library"

## Creating a ZIP Container

The `zipcOpen` function creates a ZIP container:

    zipc_t *zc = zipcOpen("filename.epub", "w");


## Adding Files to a ZIP Container

Note: You can only add one file to a ZIP container at a time.

The `zipcCreateFile` function creates a file within the ZIP container:

    zipc_file_t *zf = zipcCreateFile(zc, "META-INF/container.xml", 1);

The `zc` argument is the container, "META-INF/container.xml" specifies the
filename within the container, and the `1` value specifies that the file should
be compressed.  You would specify `0` to add the file without compression.

Once created, the `zipcFilePrintf`, `zipcFilePuts`, `zipcFileWrite`, and
`zipcFileXMLPrintf()` functions can be used to write to the file in the
container:

    zipcFilePuts(zf, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

    zipcFilePrintf(zf, "<element>%d</element>\n", intvalue);

    zipcFileWrite(zf, buffer, length);

    zipcFileXMLPrintf(zf, "<element>%s</element>\n", stringvalue);

When all data has been written, call the `zipcFileFinish()` function to
"close" it:

    zipcFileFinish(zf);

Sometimes ZIP container files contain short constant strings.  These may be
added using the `zipcCreateFileWithString` function:

    zipcCreateFileWithString(zc, "metadata", "application/epub+zip");

Files created in this way are always stored uncompressed.


## Adding Directories to a ZIP Container

Although strictly not required, you can add directories to a ZIP container
using the `zipcCreateDirectory` function:

    zipcCreateDirectory(zc, "META-INF/");

If you forget to include a trailing slash, one will be added for you.


## Copying Files to a ZIP Container

The `zipcCopyFile` function efficiently copies a file to the ZIP container.

    /* Copy text file and compress */
    zipcCopyFile(zc, "CONTENT/filename.html", "tempfile.html", 1, 1);

    /* Copy image file and don't compress */
    zipcCopyFile(zc, "CONTENT/filename.png", "tempfile.png", 0, 0);

    /* Copy binary data file and compress */
    zipcCopyFile(zc, "CONTENT/filename.dat", "tempfile.dat", 0, 1);


## Closing the ZIP Container

Once you have added all of the files you need to the container, close it with
the `zipcClose` function:

    zipcClose(zc);


## Error Handling/Reporting

The various functions either return `-1` or a `NULL` pointer on error.  You can
call the `zipcError()` function to get a string describing the last error for
the specified container:

    printf("Error: %s\n", zipcError(zc));


# Reference

## zipc_t - ZIP Container

    typedef struct _zipc_s zipc_t;

The `zipc_t` object tracks a ZIP container that was opened with the `zipcOpen`
function.


## zipc_file_t - ZIP Container File

    typedef struct _zipc_file_s zipc_file_t;

The `zipc_file_t` object tracks a single file in a ZIP container that was
created using the `zipcCreateFile` function.


## zipcClose - Close a ZIP Container File

    int
    zipcClose(zipc_t *zc);

The `zipcClose` function closes a ZIP container file, writing the central
directory for every file and directory in the container.


## zipcCopyFile - Copy a File Into a ZIP Container

    int
    zipcCopyFile(zipc_t *zc, const char *dstname, const char *srcname,
                 int text, int compressed);

The `zipcCopyFile` function copies an existing file `srcname` into a ZIP
container file `dstname`.  If the `text` parameter is non-zero then the file
will be stored as plain text.  If the `compressed` parameter is non-zero then
the file will be compressed when copied into the container.


## zipcCreateDirectory - Create a Directory in a ZIP Container

    int
    zipcCreateDirectory(zipc_t *zc, const char *filename);

The `zipcCreateDirectory` function creates a directory in the ZIP container.


## zipcCreateFile - Create a File in a ZIP Container

    zipc_file_t *
    zipcCreateFile(zipc_t *zc, const char *filename, int compressed);

The `zipcCreateFile` function creates a file in the ZIP container for streamed
writing.  If the `compressed` parameter is non-zero then the file will be
compressed in the container.

Data is written to the created file using the `zipcFilePrintf`, `zipcFilePuts`, `zipcFileWrite`, and `zipcFileXMLPrintf` functions.  The file is "closed" using
the `zipcFileFinish` function.


## zipcCreateFileWithString - Create a File From a String in a ZIP Container

    int
    zipcCreateFileWithString(zipc_t *zc, const char *filename,
                             const char *contents);

The `zipcCreateFileWithString` function creates a file in the ZIP container that
contains the `contents` string.  The file is not compressed.


## zipcError - Return the Last Error for a ZIP Container

    const char *
    zipcError(zipc_t *zc);

The `zipcError` function returns the last error message for a ZIP container.
`NULL` is returned if no error has occurred.


## zipcFileFinish - Close a Created File

    int
    zipcFileFinish(zipc_file_t *zf);

The `zipcFileFinish` function "closes" a ZIP container file that was created
using the `zipcCreateFile` function.


## zipcFilePrintf - Write a Formatted String to a ZIP Container File

    int
    zipcFilePrintf(zipc_file_t *zf, const char *format, ...);

The `zipcFilePrintf` function writes a formatted string to a ZIP container file
that was created using the `zipcCreateFile` function.  The format string uses
the same syntax as the `printf` function.


## zipcFilePuts - Write a String to a ZIP Container File

    int
    zipcFilePuts(zipc_file_t *zf, const char *s);

The `zipcFilePuts` function writes a literal string to a ZIP container file
that was created using the `zipcCreateFile` function.


## zipcFileWrite - Write Bytes to a ZIP Container File

    int
    zipcFileWrite(zipc_file_t *zf, const void *data, size_t bytes);

The `zipcFileWrite` function writes a buffer of data bytes to a ZIP container
file that was created using the `zipcCreateFile` function.


## zipcFileXMLPrintf - Write a Formatted XML String to a ZIP Container File

    int
    zipcFileXMLPrintf(zipc_file_t *zf, const char *format, ...);

The `zipcFileXMLPrintf` function writes a formatted XML string to a ZIP
container file that was created using the `zipcCreateFile` function.  The format
string supports the "%%", "%d", and "%s" `printf` directives, where the "%s"
(string) directive handles escaping of special XML/HTML characters.


## zipcOpen - Open a ZIP Container File

    zipc_t *
    zipcOpen(const char *filename, const char *mode);

The `zipcOpen` function opens a new ZIP container file.  The `mode` argument
specifies how the container is opened and currently must be the string "w" to
indicate that the ZIP container file is opened for writing, replacing any
existing file of the given name.
