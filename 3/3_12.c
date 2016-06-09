#include <stdio.h>
#include <stdlib.h>

void fileRead(FILE *fdr);
void printimg(char data[512]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);
void normalize(char p[64][64]);
void copy(char b[64][64], char a[64][64]);
void searchend(double ends[4], char p[64][64]);

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
		normalize(pattern);
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

void normalize(char p[64][64])
{
	int u, v;
	double hratio, wratio;
	double ends[4];
	char o[64][64];
	
	copy(p, o);
	searchend(ends, p);
	wratio = ends[0] / ends[2];
	hratio = ends[1] / ends[3];
	for (v=0; v<64; v++) {
		for (u=0; u<64; u++) {
			p[v][u] = o[(int)(v*hratio + ends[1] +0.5)][(int)(u*wratio + ends[0] + 0.5)];
			printf("%d/%d ", (int)(v*hratio + ends[1] +0.5), (int)(u*wratio + ends[0] + 0.5));
		}
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

void searchend(double ends[4], char p[64][64])
{
	int x, y;
	for (x=0; x<64; x++) {
		for (y=0; y<64; y++) {
			if (p[y][x]==1) {
				ends[0] = x;
				break;
			}
		}
		if (p[y][x]==1) break;
	}
	
	for (y=0; y<64; y++) {
		for (x=0; x<64; x++) {
			if (p[y][x]==1){
				ends[1] = y;
				break;
			}
		}
		if (p[y][x]==1) break;
	}

	for (x=63; x>=0; x--) {
		for (y=63; y>=0; y--) {
			if (p[y][x]==1) {
				ends[2] = x-ends[0];
				break;
			}
		}
		if (p[y][x]==1) break;
	}

	for (y=63; y>=0; y--) {
		for (x=63; x>=0; x--) {
			if (p[y][x]==1){
				ends[3] = y-ends[1];
				break;
			}
		}
		if (p[y][x]==1) break;
	}
}