#include "const.h"

void version();
void help();

void version(){
	printf("hangman: %0.2f\n",VERSION);
	exit(0);
}

void help(){
	printf("Usage: hangman [options]\n");
	printf("  --level <lvl>: select level\n");
	printf("       \t%sheight%s - 6 attempt (default)\n",RED,DEFAULT);
	printf("       \t%snormal%s - 8 attempt\n",YELLOW,DEFAULT);
	printf("       \t%slower%s - 13 attempt\n",GREEN,DEFAULT);
	printf("  --word <word>: input self word\n");
	printf("  --debug/-d: print debug information\n");
	printf("  --hint/-t: input hint for input word or just hint\n");
	printf("  --version/-v: print version program and exit\n");
	printf("  --help/-h: print this help page and exit\n");
	exit(0);
}
