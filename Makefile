all: GeeMail

GeeMail: main.o email.o login.o encrypt.o sqlite3_driver.o 
	g++ main.o sqlite3_driver.o email.o encrypt.o login.o -o GeeMail -lsqlite3 -lgcrypt -std=c++11
	
email.o: email.cpp
	g++ -c email.cpp -std=c++11

encrypt.o: encrypt.cpp
	g++ -c encrypt.cpp -lgcrypt -std=c++11
	
login.o: login.cpp
	g++ -c login.cpp -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11
	
sqlite3_driver.o: sqlite3_driver.cpp
	g++ -c sqlite3_driver.cpp -std=c++11

clean:
	rm -f *.o *.out GeeMail