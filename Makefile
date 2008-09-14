test:
	cd genes ; g++ -o a add_auditory.cpp $(DL)
	wish genome_builder.tcl
	g++ -ggdb -o b body.cpp $(DL)
