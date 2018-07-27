include ../makefile_includes/Makefile.defaults

# TINYXML_USE_STL must be turned on with STL support.
DEFS = -DTIXML_USE_STL

EXCEPT_DIR = -I../Exceptions
MATHTOOL_DIR = -I../mathtool

INCL = $(EXCEPT_DIR) $(MATHTOOL_DIR)
OBJECTS = \
					tinyxml/tinystr.o \
					tinyxml/tinyxml.o \
					tinyxml/tinyxmlerror.o \
					tinyxml/tinyxmlparser.o \
					XMLNode.o
TARGET = libtinyxml.a

default_target: library

library: $(TARGET)

$(TARGET): ${OBJECTS}
	${AR} $@ ${OBJECTS}

CLEAN = $(TARGET) ${OBJECTS}
