SRC=	main.cpp \
	agent.cpp \
	display.cpp \
	input.cpp \
	manage_agents.cpp

LIBS=	$(DEFAULT_LIBRARIES)  -lglut 

include ~/.makedefs

OBJS=$(SRC:.cpp=.o)

evolution:  deps $(OBJS)
	$(CPP) -o evolution $(OBJS) $(CPPFLAGS) $(LIBS) 

include deps

deps:
	$(CPP) $(CPPFLAGS) -MM $(SRCS) > deps


clean:
	rm evolution -f *.o *.a
