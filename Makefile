CXX = g++
SRCS=src/*cpp
FLAGS=--std=c++11
main:
	$(CXX) $(SRCS) -o main -I src $(FLAGS)
interface:

clean:
	@rm main
