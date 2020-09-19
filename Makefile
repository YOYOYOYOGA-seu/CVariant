CC = gcc
CXX = g++
CFLAGS = -g  
CXXFLAG =   -Wall -g

ARCH:=$(shell uname -m)

ARCH32:=i686
ARCH64:=x86_64

ifeq ($(ARCH),$(ARCH64))
SYSTEM_LIB_PATH:=/usr/lib64
else
SYSTEM_LIB_PATH:=/usr/lib
endif

VPATH = . ./demo
INC = -I . -I ./demo
DIRS=$(VPATH)


INC := $(INC)


LDFLAGS := $(LDFLAGS) -D_LINUX_OS_ 

CPP_SRCS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
C_SRCS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS)) $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cc,%.o,$(CC_SRCS))


TARGET = cVariantTest

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -g -o $@ $^ $(LDFLAGS)

%.o:%.cpp
	$(CXX) $(INC) $(CXXFLAG) -c -o $@ $< $(LDFLAGS)
%.o:%.cc
	$(CXX) $(INC) $(CXXFLAG) -c -o $@ $< $(LDFLAGS)
%.o:%.c
	$(CC) $(INC) $(CXXFLAG) -c -o $@ $< $(LDFLAGS)
clean:
	rm -f ./demo/$(OBJS)
	rm -f $(TARGET)
        
