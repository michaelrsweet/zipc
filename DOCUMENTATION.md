# How to Use the zipc "Library"

## Creating a ZIP Container

The `zipc_open` function creates a ZIP container:

    zipc_t *zc = zipc_open("filename.epub", "w");


## Adding Files to a ZIP Container

Note: You can only add one file to a ZIP container at a time.

The `zipc_create_file` function creates a file within the ZIP container:

    zipc_file_t *zf = zipc_create_file(zc, "META-INF/container.xml", 1);

The `zc` argument is the container, "META-INF/container.xml" specifies the
filename within the container, and the `1` value specifies that the file should
be compressed.  You would specify `0` to add the file without compression.

Once created, the `zipc_file_printf`, `zipc_file_puts`, `zipc_file_write`, and
`zipc_xml_printf()` functions can be used to write to the file in the container:

    zipc_file_puts(zf, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

    zipc_file_printf(zf, "<element>%d</element>\n", intvalue);

    zipc_file_write(zf, buffer, length);

    zipc_file_xml_printf(zf, "<element>%s</element>\n", stringvalue);

When all data has been written, call the `zipc_file_finish()` function to
"close" it:

    zipc_file_finish(zf);

Sometimes ZIP container files contain short constant strings.  These may be
added using the `zipc_create_file_with_string` function:

    zipc_create_file_with_string(zc, "metadata", "application/epub+zip");

Files created in this way are always stored uncompressed.


## Closing the ZIP Container

Once you have added all of the files you need to the container, close it with
the `zipc_close` function:

    zipc_close(zc);


## Error Handling/Reporting

The various functions either return `-1` or a `NULL` pointer on error.  You can
call the `zipc_error()` function to get a string describing the last error for
the specified container:

    printf("Error: %s\n", zipc_error(zc));
