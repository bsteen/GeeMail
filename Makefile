all: GeeMail

GeeMail: main.o driver.o login.o encrypt.o
	g++ main.o driver.o login.o encrypt.o -o GeeMail -lsqlite3 -lgcrypt -std=c++11
	
encrypt.o:
	g++ -c encrypt.cpp -lgcrypt -std=c++11
	
login.o:
	g++ -c login.cpp -std=c++11
	
sqlite3_driver.o:
	g++ -c sqlite3_driver.cpp -std=c++11
	
main.o:
	g++ -c main.cpp -std=c++11
	
clean:
	rm *.o GeeMail