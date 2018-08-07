CXX = g++
CC = $(CXX)
INCLUDE = -I../../Library/Deps/Boost/Include -I../../Library/Deps/Hiredis/Include \
	  -I../../Library/Deps/MySQL/Include \
	  -I../../Library/SimpleIO/
CXXFLAGS = -std=c++11 -g -ggdb -D_GNU_SOURCE -D_REENTRANT $(INCLUDE)
CFLAGS = $(CXXFLAGS)
LIB_DIR = -L. -L../../Library/Deps/Boost/Lib/ -L../../Library/Deps/MySQL/Lib/ -L../../Library/Deps/Hiredis/Lib/
LDFLAGS = $(LIB_DIR) -lstdc++ -lpthread -lrt -lsimpleio -ldl -lboost_system -lboost_filesystem -lmysqlclient -lhiredis

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
GCH = Precompiled.h.gch

TARGET = gameserver

all : $(TARGET)

$(TARGET) : $(GCH) $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)


$(GCH) :
	$(CXX) $(CXXFLAGS) -x c++-header Precompiled.h -o obj/$@

clean :
	rm -rf $(OBJS) $(TARGET) obj/Precompiled.h.gch

echo :
	@echo "CXX = $(CXX)"
	@echo "OBJS = $(OBJS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
