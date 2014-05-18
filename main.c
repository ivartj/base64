#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "base64.h"

int reverse = 0;
char *outfile = NULL;
FILE *out = NULL;
char *infile = NULL;
FILE *in = NULL;

void usage(FILE *out)
{
	fprintf(out, "Usage: base64 [ -o <output-file> ] [ -r | --reverse ] [ <input-file> ]\n");
}

void parseargs(int argc, char *argv[])
{
	int c;
	static struct option longopts[] = {
		{ "out", required_argument, NULL, 'o' },
		{ "reverse", no_argument, NULL, 'r' },
		{ "help", no_argument, NULL, 'h' },
		{ 0, 0, 0, 0 },
	};

	while((c = getopt_long(argc, argv, "hro:", longopts, NULL)) != -1)
	switch(c) {
	case 'o':
		outfile = optarg;
		break;
	case 'r':
		reverse = 1;
		break;
	case 'h':
		usage(stdout);
		exit(EXIT_SUCCESS);
	case '?':
	case ':':
		usage(stderr);
		exit(EXIT_FAILURE);
	}

	switch(argc - optind) {
	case 0:
		break;
	case 1:
		infile = argv[optind];
		break;
	default:
		usage(stderr);
		exit(EXIT_FAILURE);
	}
}

void openfiles(void)
{
	if(infile != NULL) {
		in = fopen(infile, "rb");
		if(in == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	} else
		in = stdin;

	if(outfile != NULL) {
		out = fopen(outfile, "w");
		if(out == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	} else
		out = stdout;
}

void decode(void)
{
	base64_decode(NULL, in, NULL, out);
}

void encode(void)
{ 
	base64_encode(NULL, in, NULL, out);
	fputc('\n', out);
}

int main(int argc, char *argv[])
{
	parseargs(argc, argv);
	openfiles();
	if(reverse)
		decode();
	else
		encode();
	exit(EXIT_SUCCESS);
}
