main:
	g++ -std=c++11 forWong_main.cpp -o forWong `pythia8-config --cppflags --libs --cxxflags` `root-config --cflags --glibs`
	g++ -std=c++11 forWong_mixed.cpp -o forWong_mixed `pythia8-config --cppflags --libs --cxxflags` `root-config --cflags --glibs`
run:
	./forWong forWong.cmnd > forWong.out
run_mixed:
	./forWong_mixed forWong_mixed.cmnd > forWong_mixed.out
pack_forWong:
	tar czvf forWong_Aug24.tar.gz *.txt *pdf forWong.cmnd 
pack_forWong_newest:
	tar czvf forWong_newest.tar.gz *.txt *pdf *.cmnd *hpp *cpp *out Makefile xmldoc README
pack_forWong_code:
	tar czvf forWong_code.tar.gz *cpp *hpp 
help:
	@echo "make: build the program."
	@echo "make run: run the main program."
	@echo "make run_mixed: run the random version of the program."
	@echo "make packe_forWong: deprecated."
	@echo "make pack_forWong_mixed_onlymult_newscheme: pack."
	@echo "make pack_forWong_code: just pack the code"
