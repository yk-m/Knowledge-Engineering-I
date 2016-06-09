#include <stdio.h>
#include <stdlib.h>

#define SIDE_LEN 64

void fileRead(FILE *fdr);
void output(char pattern[64][64]);
void importFile(char data[], char pattern[64][64]);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr;
	char fn[100];
	
	for (i=1; 1<argc && i<argc; i++) {
		if ((fdr = fopen(argv[i], "rb")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}
		fileRead(fdr);
		fclose(fdr);
	}
	
	while(getchar()!='\n');
}

void fileRead(FILE *fdr)
{
	char pattern[64][64];
	char data[512];
	int count=0;
	while (count<20) {
		if (fread(data, 512, 1, fdr) != 1) {
			printf("can't open a file\n");
			exit(2);
		}
		importFile(data, pattern);
		output(pattern);
		count++;

		while(getchar()!='\n');
	}
}

void output(char pattern[][SIDE_LEN])
{
	int i, j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			printf("%c", (pattern[i][j] == 0)? '.' : '*');
		}
		printf("\n");
	}
}

void importFile(char data[], char pattern[64][64])
{
	int i, j, k;
	int count=0;
	unsigned char mask;

	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					pattern[i][count] = 0;
				} else {
					pattern[i][count] = 1;
				}
				mask = mask >>1;
				count++;
				if (count == 64) count = 0;
			}
		}
	}
}