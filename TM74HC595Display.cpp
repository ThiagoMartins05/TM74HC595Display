/*
Igor Vinicius Batalha Fernandes - 21950642
Ingra Cristina de Oliveira Morais - 21952200
Thiago Da Silva Martins - 22153226
*/

#include "TM74HC595Display.h"
#include <Arduino.h>

// Array que armazena os valores correspondentes aos dígitos de 0 a F
unsigned char digitValues[39];
char map_char[39];
// Construtor da classe TM74HC595Display
TM74HC595Display::TM74HC595Display(int SCLK, int RCLK, int DIO)
{
  // Configuração dos pinos como saída
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DIO, OUTPUT);
    
  // Atribuição dos pinos
  _SCLK = SCLK;
  _RCLK = RCLK;
  _DIO = DIO;
  
  map_char = {'0','1','2','3','4','5','6','7','8','9','@','@','@','@','@','@','@','@','@','@','A','b','C','d','E','F','G','H','I','J','L','n','O','P','q', 'S'. 'U', 'Y', '@'};
  // Inicialização do array com os valores correspondentes aos dígitos de 0 a F
  digitValues[0] = 0xC0; //0
  digitValues[1] = 0xF9; //1
  digitValues[2] = 0xA4; //2
  digitValues[3] = 0xB0; //3
  digitValues[4] = 0x99; //4
  digitValues[5] = 0x92; //5
  digitValues[6] = 0x82; //6
  digitValues[7] = 0xF8; //7
  digitValues[8] = 0x80; //8
  digitValues[9] = 0x90; //9

  digitValues[10] = 0b01000000; //.0
  digitValues[11] = 0b01111001; //.1
  digitValues[12] = 0b00100100; //.2
  digitValues[13] = 0b00110000; //.3
  digitValues[14] = 0b00011001; //.4
  digitValues[15] = 0b00010010; //.5
  digitValues[16] = 0b00000010; //.6
  digitValues[17] = 0b01111000; //.7
  digitValues[18] = 0b00000000; //.8
  digitValues[19] = 0b00010000; //.9

  digitValues[20] = 0x88; //A
  digitValues[21] = 0x83; //b
  digitValues[22] = 0xC6; //C
  digitValues[23] = 0xA1; //d
  digitValues[24] = 0x86; //E
  digitValues[25] = 0x8E; //F
  digitValues[26] = 0xC2; //G
  digitValues[27] = 0x89; //H
  digitValues[28] = 0xF9; //I
  digitValues[29] = 0xF1; //J
  digitValues[30] = 0xC3; //L
  digitValues[31] = 0xA9; //n
  digitValues[32] = 0xC0; //O
  digitValues[33] =  0x8C; //P
  digitValues[34] = 0x98; //q
  digitValues[35] = 0x92; //S
  digitValues[36] = 0xC1; //U
  digitValues[37] = 0x91; //Y
  digitValues[38] = 0xFE; //hight
  
  clear();
}

// Função que atualiza os valores nos registradores de deslocamento e exibe os dígitos no display
void TM74HC595Display::timerIsr()
{
  // Envia o valor do primeiro dígito e define o ponto decimal correspondente
  send(_DATA[0]);
  send(0b0001);
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);

  // Envia o valor do segundo dígito e define o ponto decimal correspondente
  send(_DATA[1]);
  send(0b0010);
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);

  // Envia o valor do terceiro dígito e define o ponto decimal correspondente
  send(_DATA[2]);
  send(0b0100);
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);

  // Envia o valor do quarto dígito e define o ponto decimal correspondente
  send(_DATA[3]);
  send(0b1000);
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);
}

// Função auxiliar para enviar um byte para o registrador de deslocamento
void TM74HC595Display::send(unsigned char X)
{
  for (int i = 8; i >= 1; i--)
  {
    // Define o estado do pino de dados com base no valor do bit atual
    if (X & 0x80)
    {
      digitalWrite(_DIO, HIGH);
    }
    else
    {
      digitalWrite(_DIO, LOW);
    }
    
    // Desloca o byte para a esquerda e envia o pulso de clock para o registrador de deslocamento
    X <<= 1;
    digitalWrite(_SCLK, LOW);
    digitalWrite(_SCLK, HIGH);  
  }
}

// Função auxiliar para enviar um byte e um número de porta para o registrador de deslocamento
void TM74HC595Display::send(unsigned char X, unsigned char port)
{
  // Envia o byte para o registrador de deslocamento
  send(X);
  
  // Envia o número da porta para o registrador de deslocamento
  send(port);
  
  // Atualiza o registrador de deslocamento
  digitalWrite(_RCLK, LOW);
  digitalWrite(_RCLK, HIGH);
}

// Função para exibir um número de 4 dígitos no display, permitindo exibir ou não o zero à esquerda
void TM74HC595Display::digit4(int n, bool showZero)
{
  // Separação do número em dígitos individuais
  int n1, n2, n3, n4;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100) - n1) / 10;
  n3 = (int) ((n % 1000) - n2 - n1) / 100;
  n4 = (int) ((

n % 10000) - n3 - n2 - n1) / 1000;

  // Define o valor dos dígitos nos respectivos registradores de deslocamento
  set(digitValues[n1], 0);
  if (showZero || n > 9) set(digitValues[n2], 1);
  if (showZero || n > 99) set(digitValues[n3], 2);
  if (showZero || n > 999) set(digitValues[n4], 3);
}

// Função sobrecarregada para exibir um número de 4 dígitos no display sem exibir o zero à esquerda
void TM74HC595Display::digit4(int n)
{
  digit4(n, false);
}

// Função para exibir um número de 4 dígitos no display, incluindo o zero à esquerda
void TM74HC595Display::digit4showZero(int n)
{
  digit4(n, true);
}

// Função para exibir um número de 2 dígitos em um determinado conjunto de portas
void TM74HC595Display::digit2(int n, int port)
{
  // Separação do número em dígitos individuais
  int n1, n2;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100) - n1) / 10;
  
  // Define o valor dos dígitos nos respectivos registradores de deslocamento
  set(digitValues[n1], port);
  set(digitValues[n2], port + 1); 
}

// Função para definir um valor para um determinado conjunto de portas
void TM74HC595Display::set(unsigned char X, int port)
{
  _DATA[port] = X;
}

// Função para limpar todos os dígitos do display
void TM74HC595Display::clear()
{
  _DATA[0] = 0xFF;
  _DATA[1] = 0xFF;
  _DATA[2] = 0xFF;
  _DATA[3] = 0xFF;
}



// Função para exibir um número inteiro com um ponto decimal em uma determinada posição
void TM74HC595Display::int_dot(int n, int pos)
{
  // Separação do número em dígitos individuais
  int n1, n2, n3, n4;
  n1 = (int) n % 10;
  n2 = (int) ((n % 100) - n1) / 10;
  n3 = (int) ((n % 1000) - n2 - n1) / 100;
  n4 = (int) ((n % 10000) - n3 - n2 - n1) / 1000;

  // Definição dos pontos decimais
  int dot1 = 0, dot2 = 0

, dot3 = 0;
  switch (pos) {
    case 1: dot1 = 10; break;
    case 2: dot2 = 10; break;
    case 3: dot3 = 10; break;
  }

  // Define o valor dos dígitos nos respectivos registradores de deslocamento, incluindo os pontos decimais
  set(digitValues[n1], 0);
  if (n > 9) set(digitValues[n2 + dot1], 1);
  if (n > 99) set(digitValues[n3 + dot2], 2);
  if (n > 999) set(digitValues[n4 + dot3], 3);
}

// Função para exibir um número decimal com um ponto decimal em uma determinada posição
void TM74HC595Display::float_dot(float value, int pos)
{
  // Separação da parte inteira e da parte decimal do número
  int whole = floor(value);
  int fract = round(((float) value - whole) * pow(10, pos));

  // Separação da parte inteira em dígitos individuais
  byte w1, w2, w3, w4;
  w1 = (int) whole % 10;
  w2 = (int) ((whole % 100) - w1) / 10;
  w3 = (int) ((whole % 1000) - w2 - w1) / 100;
  w4 = (int) ((whole % 10000) - w3 - w2 - w1) / 1000;

  // Definição dos pontos decimais
  byte dot1 = 0, dot2 = 0, dot3 = 0;
  switch (pos) {
    case 1: dot1 = 10; break;
    case 2: dot2 = 10; break;
    case 3: dot3 = 10; break;
  }

  // Define o valor dos dígitos nos respectivos registradores de deslocamento, incluindo os pontos decimais
  set(digitValues[w1], 0);
  if (whole > 9) set(digitValues[w2 + dot1], 1);
  if (whole > 99) set(digitValues[w3 + dot2], 2);
  if (whole > 999) set(digitValues[w4 + dot3], 3);
  if (fract > 0) {
    set(digitValues[fract], pos - 1);
  }


// Função para obter o valor de um determinado conjunto de portas
unsigned char TM74HC595Display::get(int port)
{
  return _DATA[port];
}

// Função para definir um valor para um dígito específico
void TM74HC595Display::digit(int display, unsigned char valor)
{
  _DATA[display] = valor;
}

// Função para definir um valor para um dígito específico
void TM74HC595Display::setChar(int display, char valor)
{
  int counter = 0;

  while(counter <= 39 && map_char[count] != valor) {
    counter++;
  }

  if(map_char[count] == '@' || counter > 39) {
    return 0;
  } else {
    _DATA[display] = digitValues[counter];
    return 1;
  }

  
}

// Função para limpar um dígito específico do display
void TM74HC595Display::clearPosition(int pos)
{
  _DATA[pos] = 0xFF;
}
}