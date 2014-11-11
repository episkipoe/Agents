DIRS	= common \
	genome \
	graphics \
	messages \
	nnets 

lib:
	for d in $(DIRS) ; do (cd $$d ; make); done	

clean:
	for d in $(DIRS) ; do (cd $$d ; make clean); done	
