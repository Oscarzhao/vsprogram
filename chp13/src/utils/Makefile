#
# Net Library Makefile
#
#
PROGRAM = libutils.a

SOURCE1 = $(wildcard *.cpp)
OBJS1 	= $(patsubst %.cpp,%.o, $(SOURCE1)) 
SOURCE2 = $(wildcard *.c)
OBJS2 	= $(patsubst %.c,%.o, $(SOURCE2)) 
OBJS 	= $(OBJS1) $(OBJS2)

%.o: %.cpp
	$(CXX) -c $(BUILD_INCS) -o $@ $?

%.o: %.c
	$(CXX) -c $(BUILD_INCS) -o $@ $?

$(PROGRAM):$(OBJS)
	ar -cr $(BUILD_OUT_DIR)/$@ $(OBJS)
	ranlib $(BUILD_OUT_DIR)/$@

clean:
	rm -f $(OBJS) $(BUILD_OUT_DIR)/$(PROGRAM)

