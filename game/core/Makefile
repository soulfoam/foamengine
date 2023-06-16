CC 			= gcc
EXTENSION 	= c

SRCS_DIR	= src
OBJS_DIR	= build/objs

INCS_DIR	= -I/mnt/data/code/c/libs/foamengine/inc -Iinc

CFLAGS		+= -std=gnu99 -Wall -Wextra -O2 $(INCS_DIR)

ifdef SERVER
CFLAGS      += -D_SERVER
endif

LFLAGS		+= -lm

SRCS		= $(shell find $(SRCS_DIR) -type f -name *.$(EXTENSION))
OBJS		= $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(SRCS:.$(EXTENSION)=.o))
DEPS		= $(OBJS:.o=.d)

MAKE		= make --no-print-directory -C
ECHO		= echo

DIRS := . $(shell find $(OBJS_DIR) -type d)
GARBAGE_PATTERNS := *.o *.d
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))

all		: $(OBJS)

$(OBJS_DIR)/%.o	: $(SRCS_DIR)/%.$(EXTENSION)
		  @$(CC) $(CFLAGS) -MD -MF $(@:.o=.d) -c -o $@ $<

clean		:
		  @rm -rf $(GARBAGE)

-include $(DEPS)

.PHONY		: all clean
