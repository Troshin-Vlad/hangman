#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "const.h"
#include "info.h"
#include "dbg.h"
#include "funcf.h"

/*	hangman_image[0-12].txt	*/
/*
	4 - 10 = level 6 - height
	4 - 12 = level 8 - normal
	0 - 12 = level 13 - lower
*/

void cursor_null();
void set_cursor(int row,int col);
void display_status(char *word,int cnt_err,int cnt_good);

int main(int argc,char *argv[]){

	srand(time(NULL));


	FILE *fd[13];

	char *words[COUNT_WORD]={"education","upbringing","adopt","teacher","student","development","school","academy","training",\
					"repetition","mathematics","rest","vacation","sanatorium","guidebook","hotel","bag","binoculars","camera",\
					"map","soap","umbrella","laboratory","blackboard","pencil","pen","organ","forehead","cheek","ear","glass",\
					"fingerprint","nail","muscles","table","clock","dust","sport","gym","track","field","winner","game","sun",\
					"player","final","finish","run","round","shoot","goal","net","network","universe","galaxy","star","cover",\
					"solar","planet","invest","pay","transfer","import","new","price","money","bank","client","credit","lend",\
					"hobby","stamp","coin","hook","hunter","gun","trap","water","information","message","data","file","plate",\
					"computer","editor","radio","television","programme","channel","host","listener","player","screen","mark",\
					"print","book","classics","collection","manual","guide","title","page","footer","end","begin","glassware",\
					"point","download","display","section","text","country","fork","knife","street","city","capital","central"};


	char word[255] = "";						/* массив символов для загаданного слова					*/
	char input[255] = "";						/* сюда будет записан ввод									*/
	char buf[BUFSIZE] = "";						/* сюда будет записан файл 									*/
	char time_chars[32];						/* временноя переменноя для ввода							*/
	char line[32] = "";							/* линия ________...										*/
	char level[6] = "normal";					/* переменноя для уровня сложности (по ум. средний)			*/
	char sign;									/* символ для знака в статусе								*/
	char ch;									/* для чтения символа из файла								*/
	char opt;									// for char in option
	char *clr;									/* для цвета												*/	
	char hint[255];								/* подсказка												*/
	

	short int good,gc,err,n,tf;					/* good - кол-во угад. букв, gc - за 1 попытку, err - счотчик ошибок, tf - кол-во файлов*/
	good = gc = err = n = tf = 0;

	short int begin;							/* с кокого и 												*/
	short int end;								/* по какой файл открыват 									*/

	typedef struct {
		unsigned int HINT:1;				/* флаг показа (введённой) подсказки*/
		unsigned int REPEAT:1;			/* флаг повтора цикла */
		unsigned int IMG:1;				/* флаг вывода картинки	*/
		unsigned int DEBUG:1;			/* флаг для отладочной информации */
	} flag;

	int optid;
	struct option long_opt[] = {
		{"level",1,0,'l'},
		{"debug",0,0,'d'},
		{"word",1,0,'w'},
		{"hint",1,0,'t'},
		{"version",0,0,'v'},
		{"help",0,0,'h'},
		{0,0,0,0}
	};

	flag FLAGS = {FALSE};
	sprintf(word,"%s",words[rand() % COUNT_WORD]);		/* рандомноя генерация англ. слова из массива		*/
	

	while(TRUE){
		if( (opt = getopt_long(argc,argv,"l:dw:t:vh",long_opt,&optid)) == -1)
			break;

		switch(opt){
			case 'l':
				if(!strcmp(optarg,"height"))
					sprintf(level,"%s","height");
				else if(!strcmp(optarg,"normal"))
					sprintf(level,"%s","normal");
				else if(!strcmp(optarg,"lower"))
					sprintf(level,"%s","lower");
				break;
			case 'd':
				FLAGS.DEBUG = TRUE;
				break;
			case 'w':
				sprintf(word,"%s",optarg);
				system("clear");
				break;
			case 't':
				FLAGS.HINT = TRUE;
				sprintf(hint,"%s",optarg);
				break;
			case 'v':
				version();
			case 'h':
				help();
		}
	}
	
	system("clear");

	/* подготовка линии для слова	*/
	for(int i = 0;i < strlen(word);i++) strcat(line,"_");

	/*	Проверка уст. уровня и помещение значение в переменные 	*/
	if(!strcmp(level,"height")) 		popenf(4,10,&begin,&end,&tf,FLAGS.DEBUG);
	else if(!strcmp(level,"normal")) 	popenf(4,12,&begin,&end,&tf,FLAGS.DEBUG);
	else if(!strcmp(level,"lower")) 	popenf(0,12,&begin,&end,&tf,FLAGS.DEBUG);
	else error("unknow level",0);

	/* открытие файлов	*/
	open_file(begin,end,fd,FLAGS.DEBUG);

	/*	Вывод выбранного уровня	*/
	//printf("You select level:[%s%s%s]\n",GREEN,level,DEFAULT);

	do{

		FLAGS.REPEAT = FLAGS.IMG = FALSE;
		gc = 0;

		/* Разнацветноя статистика ошибок	*/
		set_cursor(26,0);
		clr = (err > (tf/3)*2) ? RED : (err > (tf/3)) ? YELLOW : GREEN;
		printf("Error:[%s%i/%i%s]\n",clr,err,tf,DEFAULT);
		printf("Word: %s\n",line);

		printf("Enter: ");									/* Вывод приглашения буквы					*/
		fgets(time_chars,31,stdin);							/* Ввод строки или буквы					*/
		input[n] = time_chars[0];							/* помещение первого символа в переменную	*/

		/*	Ошибка: если пустой ввод или нет	*/
		if(!input[0]) error("Input is empty !",0);
		/*	Повтор цикла если был пустой ввод	*/
		if(!input[n]) FLAGS.REPEAT = TRUE;

		for(int i = 0;i < (strlen(input)-1);i++){
			/*	 Если мы уже вводили эту букву (повтор цикла)	*/
			if(input[i] == input[n]){
				set_cursor(26,20);
				printf("%sChars repeat ['%c']!%s",YELLOW,input[n],DEFAULT);
				FLAGS.REPEAT = TRUE;
			}
		}

		/* Повтор цикла при необходимости	*/
		if(FLAGS.REPEAT) continue;

		/* Цикл по слову	*/
		for(int i = 0;i < strlen(word);i++){
			/* если есть такая буква	*/
			if(word[i] == input[n]){
				line[i] = word[i];
				gc++;
				good++;
			}
		}

		/* Ошибка: если ни угадана ни одна буква	*/
		if(gc == 0){
			err++;
			FLAGS.IMG = TRUE;
		}
		if(err >= tf) break;

		/*	Вывод изображения (блочный вывод)*/
		if(FLAGS.IMG){
			set_cursor(0,0);
			fread(buf,sizeof(char),BUFSIZE,fd[err]);
			printf("%s\n\n",buf);
		}

		// display hints
		if(err == (tf/2)){
			if(FLAGS.HINT){
				set_cursor(30,0);
				printf("Hint: %s\n",hint);
				FLAGS.HINT = FALSE;
			}
		}

		//cursor_null();
		n++;

	}while(good < strlen(word));

	if(good == strlen(word)){
		printf("\n\n\t\t%s***YOU WIN !!!***%s\n\n",GREEN,DEFAULT);
		sign = '+';
	}
	else{
		while((ch = getc(fd[err])) != EOF){
				putchar(ch);
		}
		printf("\n\n\t\t%s***YOU LOST !!!***%s\n\n",RED_BOLD,DEFAULT);
		sign = '-';
	}

	printf("Inputs:[%s]\n",input);

	if(sign == '+')	printf("[%s%c%s]",GREEN,sign,DEFAULT);
	else			printf("[%s%c%s]",RED,sign,DEFAULT);
	
	// print status game
	display_status(word,err,good);

	/* закрытие файлов	*/
	close_file(fd,tf,FLAGS.DEBUG);
	return 0;

}

void set_cursor(int row,int col){
	printf("\033[%i;%iH",row,col);
}

void display_status(char *word,int cnt_err,int cnt_good){
	printf("Status:\n");
	printf("\tHidden word: %s\n",word);
	printf("\tErrors[%s%i%s]\n",RED,cnt_err,DEFAULT);
	printf("\tGuess[%s%i%s]\n",GREEN,cnt_good,DEFAULT);
}

void cursor_null(){
	printf("%s","\033[0;0H");
}
