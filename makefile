CC = g++
CC_FLAGS = -Wall -std=c++11

SRCDIR = src
INCDIR = include
OUTDIR = bin

OUTPUT_FILE = run
SOURCEFILES := $(shell find $(SRCDIR) -name '*.cpp')
INCLUDEFILES := $(shell find $(INCDIR) -name '*.h')
OBJECTFILES = $(addprefix $(OUTDIR)/,$(notdir $(SOURCEFILES:.cpp=.o)))

$(OUTPUT_FILE): $(OBJECTFILES)
	$(CC) $(OBJECTFILES) -o $(OUTPUT_FILE) $(CC_FLAGS)
	
$(OUTDIR)/%.o: $(SOURCEFILES)
	$(CC) -c $(CC_FLAGS) $< -o $@
	
.PHONY: clean

clean:
	rm -f $(OUTPUT_FILE) $(OBJECTFILES)
