#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decode(unsigned char *in, unsigned char *out, size_t size);
void encode(unsigned char *in, unsigned char *out, size_t size);

int main(int argc, char **argv) {
    FILE *input, *output;
    size_t size;
    int decode_f;
    unsigned char *in_buffer, *out_buffer;

    if (argc < 4) {
        printf("Usage: %s action input output\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!strcmp(argv[1], "d"))
        decode_f = 1;
    else if (!strcmp(argv[1], "e"))
        decode_f = 0;
    else {
        printf("Action must be (d)ecode or (e)ncode\n");
        return EXIT_FAILURE;
    }

    input = fopen(argv[2], "rb");
    fseek(input, 0, SEEK_END);
    size = ftell(input);
    fseek(input, 0, SEEK_SET);

    output = fopen(argv[3], "wb");

    in_buffer = malloc(size);
    out_buffer = malloc(size);

    fread(in_buffer, size, 1, input);
    
    if (decode_f)
        decode(in_buffer, out_buffer, size);
    else
        encode(in_buffer, out_buffer, size);

    fwrite(out_buffer, size, 1, output);

    fclose(input);
    fclose(output);
    free(in_buffer);
    free(out_buffer);

    return EXIT_SUCCESS;
}

void decode(unsigned char *in, unsigned char *out, size_t size) {
    int i;
    unsigned char k;

    for (i=0, k=0; i<size; i++, k++)
        out[i] = in[i] - k;
}

void encode(unsigned char *in, unsigned char *out, size_t size) {
    int i;
    unsigned char k;

    for (i=0, k=0; i<size; i++, k++)
        out[i] = in[i] + k;
}
