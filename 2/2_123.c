#include <stdio.h>
#include <stdlib.h>

void fileRead(FILE *fdr);
void printimg(char data[512]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);

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
	int x, y, i;
	while (count<20) {
		if (fread(data, 512, 1, fdr) != 1) {
			printf("can't open a file\n");
			exit(2);
		}
		printimg(data);
		expand(data, pattern);
		for(y=0; y<64; y++) {
			for(x=0; x< 64; x++) {
				putchar(pattern[y][x]==1 ? '@':'o');
			}
			putchar('\n');
		}
		for (i=0; i<512; i++) data[i] = 0;
		compress(data, pattern);
		printimg(data);
		
		count++;
		
		while(getchar()!='\n');
	}
}

void printimg(char data[512])
{
	int i, j, k;
	unsigned char mask = 128;
	
	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					printf(".");
				} else {
					printf("*");
				}
				mask = mask >>1;
			}
		}
		printf("\n");
	}
}

void expand( char data[512], char pattern[64][64])
{
	int i, j, k;
	int count=0;
	unsigned char mask;

	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					pattern[i][8*j%64+k] = 0;
				} else {
					pattern[i][8*j%64+k] = 1;
				}
				mask = mask >>1;
			}
		}
	}
}

void compress(char data[512], char pattern[64][64])
{
	int i,j,k;
	int count=0;
	unsigned char mask;
	
	for (i=0; i<64; i++) {
		for (j=0; j<64; j+=8) {
			mask = 0x0;
			for (k=0; k<8; k++) {
				mask = mask <<1;
				if (pattern[i][j+k]==1) mask++;
			}
			data[count] = mask;
			count++;
		}
	}
}