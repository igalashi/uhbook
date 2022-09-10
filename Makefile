#
#
#

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -O
INCLES =
LIBS = -lm

EXECS = uhbook
all: $(EXECS)

uhbook: uhbook.cxx uhbook.hxx
	$(CXX) $(CXXFLAGS) -DTEST_MAIN -o $@ $(INCLDES) \
		$< \
		$(LIBS)


clean:
	rm  $(EXECS)
