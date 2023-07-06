/*
Igor Vinicius Batalha Fernandes - 21950642
Ingra Cristina de Oliveira Morais  - 21952200
Thiago Da Silva Martins - 22153226
*/

#ifndef TM74HC595Display_h
#define TM74HC595Display_h
  
class TM74HC595Display
{
  public:
    TM74HC595Display(int SCLK, int RCLK, int DIO);
	void timerIsr();
    void send(unsigned char X);
	void send(unsigned char X, unsigned char port);
	void digit4(int n, bool showZero);
	void digit4(int n);
	void digit4showZero(int n);
	void digit2(int n, int port);
	void set(unsigned char X, int port );
	void clear();
	
	void float_dot(float value, int pos);
	void int_dot(int n, int pos);
	
	/* new */
	void clearPosition(int pos);
	void digit(int display, unsigned char valor);
	int setChar(int display, char valor);
    unsigned char get(int port );

  private:
    int _SCLK;
	int _RCLK;
	int _DIO;
	int _DATA[4];
};
 
#endif
