program_NAME := main
program_CXX_SRCS := srcs/main.cpp srcs/emu.cpp srcs/cart.cpp srcs/cpu.cpp srcs/memory.cpp srcs/parse_utils.cpp
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_CXX_OBJS)
program_INCLUDE_DIRS := /usr/include/SDL2
program_LIBRARY_DIRS :=
program_LIBRARIES := SDL2 SDL2_ttf
CXX=c++

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDLIBS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean re

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME) $(LDLIBS)

clean:
	$(RM) $(program_NAME)
	$(RM) $(program_OBJS)

re: clean all

distclean: clean