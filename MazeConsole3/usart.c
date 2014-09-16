
#include "main.h"

void usart_putc(const char c){
	printf("%c",c);
}

void usart_printstr(const char* str){

	while (*str != CHAR_NULL) {


		if (*str == '\n'){
			PUT_NEWLINE;
		}
		else{
			usart_putc(*str);
		}

		str++;
	}
}
