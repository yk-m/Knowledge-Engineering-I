#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void verification(FILE *fdr);
void printimg(char data[512]);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr;
	
	for (i=1; 1<argc && i<argc; i++) {
		if ((fdr = fopen(argv[i], "rb")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}

		verification(fdr);

		fclose(fdr);
	}
}

void verification(FILE *fdr)
{
	char data[512];
	int count=0, num;

	fseek(fdr, 0, SEEK_END);
	num = ftell(fdr) / 512;
	fseek(fdr, 0, SEEK_SET);

	while (count<num) {
		if (fread(data, 512, 1, fdr) != 1) {
			printf("can't read a file\n");
			exit(2);
		}

		printf("%d\n", count+1);
		printimg(data);
		printf("\n");
		
		count++;
		// while(getchar()!='\n');
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
					printf("0");
				} else {
					printf("1");
				}
				printf(",");
				mask = mask >>1;
			}
		}
		printf("\n");
	}
}