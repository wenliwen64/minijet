After you extract all of the files from the compressed package:
1. Move the xmldoc/ directory out of the current working directory. So the final directory structure will be like:
 
  +---MiniJet
      |- xmldoc
      |
      |- code
          |
          |- forWong_mixed.cpp 
	  |- forWong.cpp
          |- other stuff...

2. To compile the main program for Pythia/Pythia(mixing event) run, type `make` under code directory

3. To run the program:
       
     * Type `make run` to run the regular pythia simulation

     * Type `make run_mixed` to run the mixing event simulation
