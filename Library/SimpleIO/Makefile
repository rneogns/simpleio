CXX = g++
CC = $(CXX)
INCLUDE = -I../ExternalLibs/Boost/Include -I../ExternalLibs/Hiredis/Include \
	  -I../ExternalLibs/MySQL/Include
CXXFLAGS = -std=c++11 -D_GNU_SOURCE -D_REENTRANT $(INCLUDE)
CFLAGS = $(CXXFLAGS)
LIB_DIR = -L./ -L../ExternalLibs/Boost/Lib/ -L../ExternalLibs/MySQL/Lib/
LDFLAGS = $(LIB_DIR) -lboost_system -lmysql -lstdc++ -lpthread -lrt

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
GCH = Precompiled.h.gch

TARGET = libsimpleio.a

all : $(TARGET)

$(TARGET) : $(GCH) $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)


$(GCH) :
	$(CXX) $(CXXFLAGS) -x c++-header Precompiled.h -o $@

clean :
	rm -rf $(OBJS) $(TARGET) Precompiled.h.gch

echo :
	@echo "CXX = $(CXX)"
	@echo "OBJS = $(OBJS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
