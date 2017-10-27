CXX = g++
SRCS=src/*cpp
FLAGS=--std=c++11
main:
	$(CXX) $(SRCS) -o main -I src $(FLAGS)
clean:
	@rm main
