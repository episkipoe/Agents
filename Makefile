test:
	cd genes ; g++ -o a add_auditory.cpp $(DL)
	wish genome_builder.tcl
	g++ -ggdb -o b organism.cpp $(DL)

clean:
	rm -f *.net *.genome
