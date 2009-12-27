ifeq (${TARGET},debug)
CXXFLAGS 		+= -g
else
CXXFLAGS 		+= -O3 -DNDEBUG
endif 

LDFLAGS    	+= -L/usr/X11R6/lib

XLIBS      	= -L/usr/X11R6/lib -lXt -lXmu -lSM -lX11 
WIN_SYS_LIBS =
GLLIBS     	= -lGLU -lGL 
GLUTLIBS	= -lglut

AR					= ar -cr
DEPFLAGS		= -MM
INSTALL			= install -m 0755
