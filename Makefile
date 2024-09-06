test : 
# Question 1
	g++ -c Question1.cpp
	g++ -g -o debug1 Question1.o
	g++ -O3 -o optimize1 Question1.o

# Question 2
	g++ -c Question2.cpp
	g++ -g -o debug2 Question2.o
	g++ -O3 -o optimize2 Question2.o
