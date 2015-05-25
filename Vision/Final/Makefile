TARGET=vision
OBJ=Image_converter.o findContour.o detectColour.o detectFeature.o util.o

CPPFLAGS=`pkg-config --cflags opencv`
CPPFLAGS+=`pkg-config --cflags roscpp`
CPPFLAGS+=`pkg-config --cflags image_transport`
CPPFLAGS+=`pkg-config --cflags cv_bridge`

LDFLAGS=`pkg-config --libs opencv`
LDFLAGS+=`pkg-config --libs roscpp`
LDFLAGS+=`pkg-config --libs image_transport`
LDFLAGS+=`pkg-config --libs cv_bridge`

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
