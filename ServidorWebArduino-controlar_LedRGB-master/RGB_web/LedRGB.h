#ifndef _LEDRGB_h
#define _LEDRGB_h
// As duas linhas acima são diretivas de compilação condicional, que usualmente constam nesta mesma forma (alterando apenas o nome da biblioteca) em todos os arquivos de cabeçalho. Sua finalidade é evitar problemas caso um mesmo programa tente incluir indevidamente mais de uma vez a mesma biblioteca.

#include "Arduino.h"
// A linha acima faz com que a biblioteca possa usar os tipos e outras definições do próprio Arduino.

class LedRGB {
	public:
    LedRGB(int r, int g, int b);
		void begin();
		void red(int estado);
		void green(int estado);
		void blue(int estado);
	private:
		int pinB;
		int pinG;
		int pinR;
	};

#endif
