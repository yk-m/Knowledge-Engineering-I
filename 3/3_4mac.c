#include <stdio.h>
#include <stdlib.h>

void fileRead(FILE *fdr);
void printimg(char data[512]);
void printnum(char pattern[64][64]);
void printpattern(char pattern[64][64]);
void copy(char b[64][64], char a[64][64]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);
int label(char p[64][64]);
void numbering(char p[64][64], int y, int x, int num);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr;
	char fn[100];
	
	scanf("%s", fn);
	if ((fdr = fopen(fn, "rb")) == NULL) {
		printf("can't open a file\n");
		exit(1);
	}
	fileRead(fdr);
	fclose(fdr);
	
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
		expand(data,pattern);
		printf("元データ\n");
		printpattern(pattern);
		label(pattern);
		printf("ラベリング後\n");
		printnum(pattern);
		
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
void printnum(char pattern[64][64])
{
	int i,j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			printf("%d", pattern[i][j]);
		}
		printf("\n");
	}
}
void printpattern(char pattern[64][64])
{
	int i,j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			printf("%s", (pattern[i][j] == 0)? "." : "*");
		}
		printf("\n");
	}
}

void copy(char b[64][64], char a[64][64])
{
	int i,j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			a[i][j] = b[i][j];
		}
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

int label(char p[64][64])
{
	int y, x, num = 2;
	for (y=0; y<64; y++) {
		for (x=0; x<64; x++) {
			if (p[y][x]==1) {
				numbering(p, y, x, num);
				num++;
				if (num > 255) return 1;
			}
		}
	}
	return 0;
}

void numbering(char p[64][64], int y, int x, int num)
{
	int v, u;
	p[y][x] = num;

	if (x!=0  && p[y][x-1]==1) numbering(p, y, x-1, num);
	if (x!=63 && p[y][x+1]==1) numbering(p, y, x+1, num);

	if (y!=0) {
		if (x!=0  && p[y-1][x-1]==1) numbering(p, y-1, x-1, num);
		if (x!=63 && p[y-1][x+1]==1) numbering(p, y-1, x+1, num);
		if (p[y-1][x]==1) numbering(p, y-1, x, num);
	}
	if (y!=63) {
		if (x!=0  && p[y+1][x-1]==1) numbering(p, y+1, x-1, num);
		if (x!=63 && p[y+1][x+1]==1) numbering(p, y+1, x+1, num);
		if (p[y+1][x]==1) numbering(p, y+1, x, num);
	}
}
