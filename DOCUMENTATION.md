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


## Closing the ZIP Container

Once you have added all of the files you need to the container, close it with
the `zipcClose` function:

    zipcClose(zc);


## Error Handling/Reporting

The various functions either return `-1` or a `NULL` pointer on error.  You can
call the `zipcError()` function to get a string describing the last error for
the specified container:

    printf("Error: %s\n", zipcError(zc));
