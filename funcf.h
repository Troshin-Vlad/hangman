#include "const.h"

void popenf(short int n,short int e,short int *begin,short int *end,short int *tf,short int debug);
void open_file(int begin,int end,FILE *fd[],int debug);
void close_file(FILE *fd[],int n,int debug);

/*open_file: функция открывает файлы hangman_image[begin-end].txt
begin: с какого знач открывать
end: по какое знач открывать
debug: выводит отладочную информацию (если 1)	*/
void open_file(int begin,int end,FILE *fd[],int debug){

	//char *fname = "hangman_image";
	char file[32];

	for(int i = 0;begin <= end;begin++,i++){
		sprintf(file,"hangman_image%i.txt",begin);
		if(!(fd[i] = fopen(file,"r"))){
			if(debug) printf("File open:%s:%i:[%sNo%s]\n",file,i,RED,DEFAULT);
			exit(1);
		}
		else if(debug) printf("File open:%s:%i:[%sYes%s]\n",file,i,GREEN,DEFAULT);
	}

}

/*popenf: помещает в переменные значения с кокого и по какой файл открывать (preparation open file)	*/
/*tf: будет равняться кол-ву задействованных фалов, также = макс. кол-ву ошибок	*/
/*debug: выводит отладочную информацию (если 1)	*/
void popenf(short int n,short int e,short int *begin,short int *end,short int *tf,short int debug){
			
	if(debug){
		printf("Will open the file:\n");
		for(int i = n;i <= e;i++){
			printf(" hangman_image%i.txt\n",i);
		}
	}

	*begin = n;
	*end = e;
	*tf = e - n;
	
}

/*close_file: функция закрывает файлы в массиве	*/
/*n: по какой элемент в массиве закрывать файл	*/
/*debug: выводит отладочную информацию (если 1)	*/
void close_file(FILE *fd[],int n,int debug){

	int i = 0;

	while(i <= n){
		if(!fclose(fd[i])){
			if(debug) printf("File close:[%i]:[%sYes%s]\n",i,GREEN,DEFAULT);
		}
		else if(debug) printf("File close:%i:[%sNo%s]\n",i,RED,DEFAULT);
		i++;
	}
}
