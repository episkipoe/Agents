SRC=	genome.cpp \
	execute.cpp
LIB=../libgenome.a

include ~/.makedefs
OBJS=$(SRC:.cpp=.o)

$(LIB):  deps $(OBJS)
	ar rcv $(LIB) $(OBJS)
	ranlib $(LIB)

include deps

deps:
	$(CPP) $(CPPFLAGS) -MM $(SRCS) > deps


clean:
	rm -f *.o *.a
