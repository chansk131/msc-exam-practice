OBJ = main.o playfair.o
EXE = playfair
CXX = g++
CXXFLAGS = -Wall -g -MMD -std=c++11

$(EXE): $(OBJ)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)

.PHONY: clean