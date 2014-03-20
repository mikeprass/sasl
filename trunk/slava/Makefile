include ../common.mk
    
TARGET=slava
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS+=`sdl2-config --cflags` -I../alsound -I../libavionics  -I../libaccgl $(LUAJIT_CXXFLAGS) $(SOIL_CXXFLAGS)
LNFLAGS+=-L../libavionics -L../libaccgl -L../alsound $(LUAJIT_LNFLAGS) $(SOIL_LNFLAGS)
LIBS+=-lm `sdl2-config --libs` -lavionics -lalsound -laccgl $(LUAJIT_LIBS) -ldl $(SOIL_LIBS)

ifeq ($(OS),Darwin)
LIBS+=-framework CoreFoundation -framework Foundation -framework OpenGL -framework OpenAL 
else
LIBS+=-lGL -lopenal
endif

all: $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
	
$(TARGET): $(OBJECTS) ../libavionics/libavionics.a ../libaccgl/libaccgl.a
	$(CXX) -o $(TARGET) $(LNFLAGS) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp -f $(TARGET) $(PLUGINS)

run: $(TARGET)
	./$(TARGET) --secret supersecret --data ../data --panel ../examples/Tu-104BN/avionics.lua

        
