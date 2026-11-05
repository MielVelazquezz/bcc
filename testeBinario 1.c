#include <stdio.h>

struct bits{
	unsigned char b7:1;
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
	unsigned char b0:1;
	
};

int main(){
	int codigo[] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,1,0};
	int tl=32, i;
	int vet[32];
	
	
	
	struct bits b;
	
	
	FILE *arq = fopen("codificacao.dat", "ab");
	
	for(i=0; i<tl;){
		b.b0 = codigo[i++];
		b.b1 = codigo[i++];
		b.b2 = codigo[i++];
		b.b3 = codigo[i++];
		b.b4 = codigo[i++];
		b.b5 = codigo[i++];
		b.b6 = codigo[i++];
		b.b7 = codigo[i++];
		
		fwrite(&b, sizeof(struct bits), 1, arq);
		
	}
	fclose(arq);
	
	arq = fopen("codificacao.dat", "rb");
	
	fread(&b, sizeof(struct bits), 1, arq);
	i=0;
	
	while(!feof(arq)){
		vet[i++] = b.b0;
		vet[i++] = b.b1;
		vet[i++] = b.b2;
		vet[i++]  =b.b3;
		vet[i++] =b.b4;
		vet[i++] =b.b5;
		vet[i++] =b.b6;
		vet[i++] =b.b7;
		
		fread(&b, sizeof(struct bits), 1, arq);
	}
	fclose(arq);
	
	for(i=0; i<tl; i++){
		printf("%d, ", vet[i]);
	}
}
