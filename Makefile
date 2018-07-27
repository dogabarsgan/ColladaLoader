include ./makefile_includes/Makefile.defaults

# TINYXML_USE_STL must be turned on with STL support.
DEFS = -DTIXML_USE_STL

EXCEPT_DIR = -I./Exceptions
MATHTOOL_DIR = -I./mathtool

XML_DIR = -I./XML
HOME_DIR = -I.
GLM_DIR = -I./glm

LIBS = -L./XML -ltinyxml


INCL = $(XML_DIR) $(MATHTOOL_DIR) $(EXCEPT_DIR) $(HOME_DIR) $(GLM_DIR)

OBJECTS = \
					ColladaLoader.o \
					
TARGET = libcollada.a

default_target: library

library: $(TARGET)

$(TARGET): ${OBJECTS}
	${AR} $@ ${OBJECTS}
	

CLEAN = ${TARGET} ${OBJECTS} ./a.out

#g++ libcollada.a  -I./Exceptions -I./mathtool -I./XML -I. -I./glm -L./XML -ltinyxml