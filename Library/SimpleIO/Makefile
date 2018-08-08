CXX = g++
INC = -I../Deps/Boost/Include -I../Deps/Hiredis/Include -I../Deps/MySQL/Include
CXXFLAGS = -std=c++11 -O0 -g -ggdb -Wall -Werror -fpermissive -D_GNU_SOURCE -D_REENTRANT $(INC)
#LIB_DIR = -L./ -L../Deps/Boost/Lib/ -L../Deps/MySQL/Lib/
#LDFLAGS = $(LIB_DIR) -lboost_system -lmysql -lstdc++ -lpthread -lrt

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
GCHS = Precompiled.h.gch

TARGET = libsimpleio.a

all : $(GCH) $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -c $(OBJS)
	ar rcs $@ $(OBJS)

$(GCH) :
	$(CXX) $(CXXFLAGS) -x c++-header Precompiled.h -o $(GCHS)

clean :
	rm -rf $(OBJS) Precompiled.h.gch
	rm -rf $(TARGET)

echo :
	@echo "CXX = $(CXX)"
	@echo "OBJS = $(OBJS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
