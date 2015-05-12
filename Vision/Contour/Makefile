CC=gcc
CXX=g++
RM=rm -f

SRCS=findContour.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

CXXFLAGS+=$(shell pkg-config --cflags opencv)
LDLIBS+=$(shell pkg-config --libs opencv)

TARGET=contourFinder

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $< -o $(TARGET) $(LDLIBS)

print:
	@echo $(CXXFLAGS)
	@echo $(LDLIBS)
	@echo $(SRCS)
	@echo $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
