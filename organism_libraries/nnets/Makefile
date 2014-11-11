SRC=	net.cpp \
	fire_net.cpp \
	reward.cpp \
	punish.cpp \
	neuron.cpp \
	synapse.cpp

LIB=../libnnets.a

include ~/.makedefs
OBJS=$(SRC:.cpp=.o)

$(LIB):	deps $(OBJS) Makefile
	ar rcv $(LIB) $(OBJS)
	ranlib $(LIB)

include deps

deps:
	$(CPP) $(CPPFLAGS) -MM $(SRCS) > deps

clean:
	rm -f *.o *.a
