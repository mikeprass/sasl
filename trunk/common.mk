TOPDIR ?= ..
include $(TOPDIR)/conf.mk

CXXFLAGS+=$(EXTRA_CXXFLAGS) -Wall -fPIC -fno-stack-protector
LNFLAGS+=$(EXTRA_LNFLAGS)

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
  CXXFLAGS+=-mmacosx-version-min=10.7
  LNFLAGS+=-mmacosx-version-min=10.7
endif

ifneq ($(BUILD_64),yes)
CXXFLAGS+=-m32
LNFLAGS+=-m32
ifneq ($(OS),Darwin)
LNFLAGS+-L/usr/lib32
endif
endif

ifeq ($(BUILD_RELEASE),yes)
CXXFLAGS+=$(RELEASE_CXXFLAGS) -DNDEBUG=1
LNFLAGS+=$(RELEASE_LNFLAGS)
else
CXXFLAGS+=$(DEBUG_CXXFLAGS)
LNFLAGS+=$(DEBUG_LNFLAGS)
endif

LUAJIT_CXXFLAGS=-I$(LUAJIT)/include/luajit-2.0
ifneq ($(OS),Darwin)
LUAJIT_LNFLAGS=-L$(LUAJIT)/lib
else
ifeq ($(BUILD_64),yes)
LUAJIT_LNFLAGS=-L$(LUAJIT)/lib
else
LUAJIT_LNFLAGS=-L$(LUAJIT)/lib32
endif
endif
LUAJIT_LIBS=-lluajit-5.1

SOIL_CXXFLAGS=-I$(SOIL)/include
ifeq ($(OS),Darwin)
SOIL_LIBS+=-lSOIL
ifeq ($(BUILD_64),yes)
SOIL_LNFLAGS=-L$(SOIL)/lib
else
SOIL_LNFLAGS=-L$(SOIL)/lib32
endif
else
SOIL_LNFLAGS=-L$(SOIL)/lib
ifneq ($(BUILD_64),yes)
SOIL_LIBS+=-lSOIL32
else
SOIL_LIBS+=-lSOIL
endif
endif

