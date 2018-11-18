all_target = base_class pb_test
all: $(all_target) 
	@echo "compile all" $^
base_class:
	g++ base_class.cpp -o base_class -std=c++11
pb_test:
	g++ pb_test.cpp -o pb_test -std=c++11 -I../ -I/usr/local/Cellar/protobuf/3.6.0/include/ -L/usr/local/Cellar/protobuf/3.6.0/lib -lprotobuf -L../src/interface -linterface

clean:
	@rm $(all_target) 
