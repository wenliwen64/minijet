0. Extract all files from tarball. You should have Pythia8186 installed on your computer

1. Move the `xmldoc/` directory out of the current working directory. So the final directory structure will be like:
 
```c++
  +---MiniJet
      |- xmldoc
      |
      |- code
          |
          |- forWong_mixed.cpp 
	  |- forWong.cpp
          |- other stuff...
```

2. To compile the main program for Pythia/Pythia(mixing event) run, type `make` under code directory

3. To run the program:
       
     * Type `make run` to run the regular pythia simulation

     * Type `make run_mixed` to run the mixing event simulation
