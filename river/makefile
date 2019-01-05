OBJ = main.o river.o
EXE = river
CXX = g++
CXXFLAGS = -Wall -g -MMD

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OJB:.o=.d)

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)

.PHONY: clean