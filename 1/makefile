OBJ = main.o substring.o
EXE = main
CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -g -c $<

-include $(OBJ:.o=.d)

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d) errors.o errors.d

.PHONY: clean