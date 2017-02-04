#include "const.h"

void error(char *str,int helps);

/*error: Выводит сообщение об ошибке и выходит	*/
void error(char *str,int helps){
	printf("[%serror%s]: %s\n",RED,DEFAULT,str);
	if(helps) 
		help();
	exit(1);
}
