COMPILER  = g++
CFLAGS    = -MMD -MP -Wall -Wextra -Winit-self -Wno-missing-field-initializers
ARGV   = ./netlist/test.cir 
ifeq "$(shell getconf LONG_BIT)" "64"
  LDFLAGS =
else
  LDFLAGS =
endif
LIBS      =
INCLUDE   = -I./include
TRGDIR    = ./bin
TARGET    = exe
SRCDIR    = ./source
ifeq "$(strip $(SRCDIR))" ""
  SRCDIR  = .
endif
SOURCES   = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR    = ./obj
ifeq "$(strip $(OBJDIR))" ""
  OBJDIR  = .
endif
OBJECTS   = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS   = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS) $(LIBS)
	-mkdir -p $(TRGDIR)
	$(COMPILER) -o $(TRGDIR)/$@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

run:
	$(TRGDIR)/$(TARGET) $(ARGV)

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TRGDIR)/$(TARGET)

-include $(DEPENDS)