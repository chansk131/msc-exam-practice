OBJ = main.o stamp.o
EXE = stamp
CXX = g++ 
CXXFLAGS = -Wall -g -MMD 

$(EXE): $(OBJ)
	$(CXX) $^ -o $@ -lcrypto 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< 

-include $(OBJ:.o=.d)

clean:
	rm -f $(OBJ) $(EXE) $(OBJ:.o=.d)

.PHONY: clean