#
# InterProcess Makefile
#
#
PROGRAM = InterProcess
LIBS    = -L$(BUILD_OUT_DIR) $(TASKLIBS)
SOURCE  = $(wildcard *.cpp)
OBJS 	= $(patsubst %.cpp,%.o, $(SOURCE)) 

%.o: %.cpp
	$(CXX) -c $(BUILD_INCS) $(FORCE_INCLUDE) -o $@ $?

$(PROGRAM):$(OBJS)
	$(CXX) -o $(BUILD_OUT_DIR)/$@ $^ $(LIBS) $(BUILD_LIBS)

clean:
	rm -f $(OBJS) $(BUILD_OUT_DIR)/$(PROGRAM)

