include ../makefile_includes/Makefile.defaults

CLEAN = test example

example: example.cpp $(LIBFILE)
	${CXX} ${OPTS} ${CXXFLAGS} $< -o $@

test: test.cpp $(LIBFILE)
	${CXX} ${OPTS} ${CXXFLAGS} $< -o $@
