include ../common.mk

PLUGINS="../../plugin/sasl"



ifeq ($(OS),Darwin)
TARGET=mac.xpl
CXXFLAGS+=-DAPL=1
else
TARGET=lin.xpl
CXXFLAGS+=-DLIN=1
endif

HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS+=-I$(XPSDK)/CHeaders/XPLM -I$(XPSDK)/CHeaders/Widgets $(LUAJIT_CXXFLAGS) $(SOIL_CXXFLAGS) -I../libavionics -I../libaccgl -I../alsound -DXPLM200
LNFLAGS+=-shared -rdynamic -fno-stack-protector -ffast-math -nodefaultlibs -L../libavionics -L../libaccgl -L../alsound $(LUAJIT_LNFLAGS) $(SOIL_LNFLAGS)
LIBS+=-lm -lavionics -laccgl -lalsound $(LUAJIT_LIBS) $(SOIL_LIBS)

ifeq ($(OS),Darwin)
LNFLAGS+=-dynamiclib -undefined warning -flat_namespace -Wl,-exported_symbols_list -Wl,linkscript.mac
LIBS+=-F$(XPSDK)/Libraries/Mac/ -framework XPWidgets -framework XPLM -framework CoreFoundation -framework OpenGL -framework OpenAL 
else
LNFLAGS+= -Wl,--version-script=linkscript.linux
LIBS+=-lopenal
endif


all: $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
	
$(TARGET): $(OBJECTS) ../libavionics/libavionics.a ../libaccgl/libaccgl.a ../alsound/libalsound.a
	$(CXX) -o $(TARGET) $(LNFLAGS) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp -f $(TARGET) $(PLUGINS)


