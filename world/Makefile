SRC=	main.cpp \
	agent.cpp \
	display.cpp \
	input.cpp \
	announcements.cpp \
	manage_agents.cpp \
	message_dispatch.cpp

LIBS=	$(DEFAULT_LIBRARIES) -lgraphics -lglut 

include ~/.makedefs

OBJS=$(SRC:.cpp=.o)

evolution:  deps $(OBJS)
	$(CPP) -o evolution $(OBJS) $(CPPFLAGS) $(LIBS) 

include deps

deps:
	$(CPP) $(CPPFLAGS) -MM $(SRCS) > deps


clean:
	rm evolution -f *.o *.a
