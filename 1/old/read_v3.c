#include <stdio.h>
#include <stdlib.h>

#define SIDE_LEN 64

void fileRead(FILE *fdr);
void output(char img[][SIDE_LEN]);
void importFile(unsigned char data[], char [][SIDE_LEN]);

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
	char img[SIDE_LEN][SIDE_LEN];
	unsigned char data[20][512];
	int count=0;
	while (count<20) {
		if (fread(data[count], 512, 1, fdr) != 1) {
			printf("can't open a file\n");
			exit(2);
		}
		importFile(data[count], img);
		output(img);
		count++;

		while(getchar()!='\n');
	}
}

void output(char img[][SIDE_LEN])
{
	int i, j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			printf("%c", (img[i][j] == '0')? '.' : '*');
		}
		printf("\n");
	}
}

void importFile(unsigned char data[], char img[][SIDE_LEN])
{
	int i, j, k;
	int count=0;
	unsigned char mask;

	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					img[i][count] = '0';
				} else {
					img[i][count] = '1';
				}
				mask = mask >>1;
				count++;
				if (count == 64) count = 0;
			}
		}
	}
}