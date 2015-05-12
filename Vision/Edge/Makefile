TARGET=edge
OBJ=edge.o myTools.o

CPPFLAGS=`pkg-config --cflags opencv`
LDFLAGS=`pkg-config --libs opencv`

all: print ${TARGET}

print:
	@echo ${CPPFLAGS}
	@echo ${LDFLAGS}

%.o : %.cpp
	g++ $(CPPFLAGS) -o $@ $< -c

${TARGET}: ${OBJ}
	g++ $(CPPFLAGS) -o ${TARGET} ${OBJ} ${LDFLAGS}

clean:
	rm -rf ${TARGET} *.o
