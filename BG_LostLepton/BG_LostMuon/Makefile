CXX      = g++

CXXFLAGS= $(shell root-config --cflags)
LIBS    = $(shell root-config --libs) 

SOURCES = NtupleVariables.cc LostMuon.cc 
HEADERS = NtupleVariables.h LostMuon.h 
OBJECTS = $(SOURCES:.cc=.o)

EXECUTABLE = lostMuon

all: $(SOURCES) $(EXECUTABLE)

%.o: %.cc $(HEADERS)
	@echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking $(EXECUTABLE) ..."
	@echo "@$(CXX) $(LIBS) $(OBJECTS) -o $@"
	@$(CXX) -o $@ $^ $(LIBS) 
	@echo "done"

# Specifying the object files as intermediates deletes them automatically after the build process.
.INTERMEDIATE:  $(OBJECTS)

# The default target, which gives instructions, can be called regardless of whether or not files need to be updated.
.PHONY : clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

###
NtupleVariables.o: NtupleVariables.h
LostMuon.o:NtupleVariables.h LostMuon.h
