
Stworzona aplikacja pozwala na komunikację urządzenia z wykorzystaniem modułu GSM/GPRS. 
Projekt polega na zbieraniu danych pomiarowych z czujnika ruchu oraz wysyłaniu odpowiednich informacji do zdefiniowanych odbiorców oraz umieszczaniu wpisów na serwerze w chmurze.


1. Wykorzystane moduły:
	- Arduino Leonardo
	- Moduł GSM/GPRS SIM800L
	- Zegar czasu rzeczywistego DS1307RTC
	- Czujnik ruchu PIR HC-SR505
	
2. Wykorzsytane biblioteki:
	- biblitoeka do obsługi DS1307RTC: https://github.com/jarzebski/Arduino-DS1307
	

3. Opis działania systemu

   Program na bieżąco monitoruje stan czujnika ruchu. Po wykryciu stanu wysokiego (wykrycie ruchu) praca czujnika zostaje wstrzymana na 30 sekund. W tym czasie do zdefiniowanego odbiorcy zostaje wysłana wiadomość SMS z informacją alarmową. Następnie taka sama wiadomość jest wysyłana przez GPRS na serwer.
   Komunikacja pomiędzy ARDUINO a modułem GSM/GPRS jest jednostronna. Powyższy moduł służy do wysyłania komunikatów alarmowych. Nie stworzono obsługi wiadomości przychodzących. 
   W pętli głównej programu zczytywane są także data oraz godzina, pobierane z modułu RTC. Moduł ten wyposażony jest również w podtrzymywaną bateryjnie pamięć 56 bajtową. W programie zdefiniowano listę 5 odbiorców (w postaci numerów telefonów), do których wysyłane jest powiadomienie o wykryciu ruchu przez czujnik. Lista ta jest przechowywana w podtrzymywanej bateryjnie pamięci modułu RTC. Dzięki temu, po zaniku zasialania i wznowieniu działania programu, lsita zdefiniowanych odbiorców jest aktualna.
   
   
