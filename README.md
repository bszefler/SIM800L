
Stworzona aplikacja pozwala na komunikację urządzenia z wykorzystaniem modułu GSM/GPRS. 
Projekt polega na zbieraniu danych pomiarowych z czujnika ruchu oraz wysyłaniu odpowiednich informacji do zdefiniowanych odbiorców oraz umieszczaniu wpisów na serwerze w chmurze.


1. Wykorzystane moduły:
	- Arduino Leonardo
	- Moduł GSM/GPRS SIM800L
	- Zegar czasu rzeczywistego DS1307RTC
	- Czujnik ruchu PIR HC-SR505
	

2. Budowa systemu

	************
	*          *
	* HC-SR505 * ___
	*		   *    |
	************    |
				    |
	***********	    |   *************         ***********
	*         *     --> *           *		  *	        *
	*   RTC	  *         *  ARDUINO  * <-----> * SIM800L *
	*		  * <-----> *           *         *         *
	***********         *************         ***********
							  	  

	
