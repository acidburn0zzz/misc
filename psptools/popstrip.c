#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>

/* The maximum possible number of ISO indexes */
#define MAX_INDEXES 0x7E00
/* The location of the PSAR offset in the PBP header */
#define HEADER_PSAR_OFFSET 0x24
/* The location of the ISO indexes in the PSAR */
#define PSAR_INDEX_OFFSET 0x4000
/* The location of the ISO data in the PSAR */
#define PSAR_ISO_OFFSET 0x100000
/* The size of one "block" of the ISO */
#define ISO_BLOCK_SIZE 0x930

/* Struct to store an ISO index */
typedef struct {
    int offset;
    int length;
} INDEX;

/* Global variable to store executable name */
char *me;

/* Function to print the usage error message */
int usage_error() {
    printf("USAGE: %s [-o output.iso] EBOOT.PBP\n", me);
    return -1;
}

int main(int argc, char **argv) {
    FILE *pbp_stream;
    FILE *iso_stream;
    char *pbp_name = "";
    char *iso_name = "";
    unsigned char *in_buffer;
    unsigned char *out_buffer;
    INDEX iso_index[MAX_INDEXES];
    z_stream z;
    int psar_offset;
    int this_offset;
    int num_indexes;
    int out_length;
    int bytes_out = 0;
    int iso_length = 0;
    int offset;
    int length;
    int dummy[6];
    int count;

    /* Store executable name for the usage_error() function */
    me = argv[0];

    /* Check for correct number of arguments */
    if (argc < 2) return usage_error();

    /* Read in the arguments */
    for (count = 1; count < argc; count++) {
        /* Check for '-o' (extract iso) */
        if (strcmp(argv[count], "-o") == 0) {
            /* Make sure a filename is specified */
            if (count + 1 > argc) return usage_error();
            /* Store the iso filename */
            iso_name = argv[count + 1];
            /* Skip the next argument (used for iso filename) */
            count++;
            /* Check for target pbp */
        } else {
            /* Store the pbp filename */
            pbp_name = argv[count];
        }
    }

    /* Make sure a target pbp was specified */
    if (strcmp(pbp_name, "") == 0) return usage_error();

    /* Check if no iso name was specified */
    if (strcmp(iso_name, "") == 0) {
        /* Allocate the memory for the iso_name string */
        iso_name = malloc(strlen(pbp_name));
        /* Make sure the iso_name string is clean */
        strcpy(iso_name, "");
        /* Use the name of the target pbp (excluding extension) for the iso filename */
        strncat(iso_name, pbp_name, strlen(pbp_name) - 4);
        /* Append the '.iso' extension to the iso filename */
        strcat(iso_name, ".iso");
    }

    /* Open the PBP file */
    pbp_stream = fopen(pbp_name, "rb");
    if (pbp_stream == NULL) {
        printf("ERROR: Couldn't open '%s' for reading\n", argv[1]);
        return -1;
    }

    /* Open the output ISO file */
    iso_stream = fopen(iso_name, "wb");
    if (iso_stream == NULL) {
        printf("ERROR: Couldn't open '%s' for writing\n", iso_name);
        return -1;
    }

    /* Read in the offset of the PSAR file */
    fseek(pbp_stream, HEADER_PSAR_OFFSET, SEEK_SET);
    if (fread(&psar_offset, 4, 1, pbp_stream) != 1) {
        fprintf(stderr, "Unable to read from stream\n");
        return -1;
    }

    /* Go to the location of the ISO indexes in the PSAR */
    fseek(pbp_stream, psar_offset + PSAR_INDEX_OFFSET, SEEK_SET);

    /* Store the current location in the PBP */
    this_offset = ftell(pbp_stream);

    /* Reset the counter variable */
    count = 0;

    /* Read indexes until the start of the ISO file */
    while (this_offset < psar_offset + PSAR_ISO_OFFSET) {

        /* Read in the block offset from the index */
        if (fread(&offset, 4, 1, pbp_stream) != 1) {
            fprintf(stderr, "Unable to read from stream\n");
            return -1;
        }
        /* Read in the block length from the index */
        if (fread(&length, 4, 1, pbp_stream) != 1) {
            fprintf(stderr, "Unable to read from stream\n");
            return -1;
        }
        /* Read in the dummy bytes */
        if (fread(&dummy, 4, 6, pbp_stream) != 1) {
            fprintf(stderr, "Unable to read from stream\n");
            return -1;
        }

        /* Record our current location in the PBP */
        this_offset = ftell(pbp_stream);

        /* Check if this looks like a valid offset */
        if (offset != 0 || length != 0) {

            /* Store the block offset */
            iso_index[count].offset = offset;
            /* Store the block length */
            iso_index[count].length = length;
            count++;
        }
    }

    /* Store the number of indexes we've read in */
    num_indexes = count;

    /* For each of the indexes */
    for (count = 0; count < num_indexes; count++) {

        /* Go to the offset specified in the index */
        this_offset = psar_offset + PSAR_ISO_OFFSET + iso_index[count].offset;
        fseek(pbp_stream, this_offset, SEEK_SET);

        /* Allocate memory for our output buffer */
        out_buffer = malloc(16 * ISO_BLOCK_SIZE);

        /* Check if this block isn't compressed */
        if (iso_index[count].length == 16 * ISO_BLOCK_SIZE) {

            /* It's not compressed, make an exact copy */
            if (fread(out_buffer, 1, 16 * ISO_BLOCK_SIZE, pbp_stream) != 1) {
                fprintf(stderr, "Unable to read from stream\n");
                return -1;
            }

            /* Output size is a full block */
            out_length = 16 * ISO_BLOCK_SIZE;

            /* If the block is compressed */
        } else {

            /* Allocate memory for our input buffer    */
            in_buffer = malloc(iso_index[count].length);

            /* Read in the number of bytes specified in the index */
            if (fread(in_buffer, 1, iso_index[count].length, pbp_stream) != 1) {
                fprintf(stderr, "Unable to read from stream\n");
                return -1;
            }

            /* Set up the zlib inflation */
            z.zalloc = Z_NULL;
            z.zfree = Z_NULL;
            z.opaque = Z_NULL;
            z.avail_in = 0;
            z.next_in = Z_NULL;
            inflateInit2(&z, -15);

            /* Set up the input stream */
            z.avail_in = iso_index[count].length;
            z.next_in = in_buffer;

            /* Set up the output stream */
            z.avail_out = 16 * ISO_BLOCK_SIZE;
            z.next_out = out_buffer;

            /* Inflate the data from the PBP */
            inflate(&z, Z_NO_FLUSH);

            /* Output size should be a full block */
            out_length = 16 * ISO_BLOCK_SIZE - z.avail_out;

            /* Clean up the input buffer */
            free(in_buffer);

            /* Clean up the zlib inflation */
            inflateEnd(&z);
        }

        /* The ISO size is contained in the data referenced in index #2
           If we've just read in index #2, grab the ISO size from the output buffer */
        if (count == 1) {
            iso_length = (out_buffer[104] + (out_buffer[105] << 8) + (out_buffer[106] << 16) + (out_buffer[107] << 24)) * ISO_BLOCK_SIZE;
            printf("Extracting iso (%i bytes) from '%s' to '%s'...\n", iso_length, pbp_name, iso_name);
        }

        /* Record the total bytes that we've written (including what's in the output buffer) */
        bytes_out += out_length;

        /* If the total bytes is larger than the ISO size, reduce the output buffer to the correct length
           This makes sure that the output ISO is the same size as the original, removing the padding added by popstation */
        if (iso_length > 0 && bytes_out > iso_length) {
            out_length = out_length - (bytes_out - iso_length);
        }

        /* Write the output buffer to the extracted ISO file */
        fwrite(out_buffer, 1, out_length, iso_stream);

        /* Clean up the output buffer */
        free(out_buffer);
    }

    /* Close the input and output files */
    fclose(iso_stream);
    fclose(pbp_stream);

    /* Exit successful */
    return 0;
}
