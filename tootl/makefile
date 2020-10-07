 ##################################################
##                                                ##
## This Makefile Should Allow It's Users To       ##
## Compile C Code To Linux or Windows From Linux. ##
##                                                ##
 ##################################################

 LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

 ################################################
##                                              ##
## Flags: modify to suit project before runtime ##
##                                              ##
 ##  ############################################
  # #
  #

# Compiler Flags
COMPILER_FLAGS :=
# Library Flags
WINDOWS_LINKER_LIBS := hypergameengine.dll
LINUX_LINKER_LIBS := -lxml2 -lhypergameengine -lfreetype

 ##############################################
##                                            ##
## Parameters: may be set by users on runtime ##
##                                            ##
 ##  ##########################################
  # #
  #

# What Should The Outpurted File Be Named?
TARGET_NAME ?= a
# What OS Should We Compile To?
TARGET_OS ?= linux
# 32 or 64 bit? (i686 or x86_64)
TARGET_CPU ?= x86_64
# Is this a library?
ARCHIVE ?= false
# Is this a static library? (ARCHIVE must be set to true)
ARCHIVE_STATIC ?= false

 #####################################
##                                   ##
## Logic: everything pass this point ##
## requires NO user action           ##
##                                   ##
 ##############       ################
               ##   ##
                 # #
                  #

# Error Checking
ifneq ($(TARGET_OS),$(filter $(TARGET_OS), linux windows))
        $(error Target OS '$(TARGET_OS)' is NOT a valid option!  Please Select linux or windows)
endif

ifneq ($(TARGET_CPU),$(filter $(TARGET_CPU), i686 x86_64))
        $(error Target CPU '$(TARGET_CPU)' is NOT a valid option!  Please Select i686 or x86_64)
endif

ifneq ($(ARCHIVE),$(filter $(ARCHIVE), false true))
        $(error ARCHIVE is a BOOL and must be true or false.  '$(ARCHIVE)' is NOT a valid option!)
endif

ifneq ($(ARCHIVE_STATIC),$(filter $(ARCHIVE_STATIC), false true))
        $(error ARCHIVE_STATIC is a BOOL and must be true or false.  '$(ARCHIVE_STATIC)' is NOT a valid option!)
endif

# Directory Where Target And Objects Will Be Generated
BUILD_DIR_ROOT := build
BUILD_DIR ?= $(BUILD_DIR_ROOT)/$(TARGET_OS)/$(TARGET_CPU)
# Directory Where Source Code Is Located
SRC_DIR ?= src

# All Found C Source Files
SRCS := $(shell find $(SRC_DIR) -name *.c)
# All Found Generated Objects
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# All Found Generated Dependencies
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Linux
ifeq ($(TARGET_OS), linux)
	ifeq ($(TARGET_CPU), i686)
		COMPILER_CPU := #-m32
	else ifeq ($(TARGET_CPU), x86_64)
		COMPILER_CPU := #-m64
	endif
	COMPILER ?= gcc
	ARCHIVER ?= ar
	PROJECT_STRUCTURE_FLAGS := $(COMPILER_CPU) -L ./lib/linux/$(TARGET_CPU)/ -I ./inc/ -lm
	ifeq ($(ARCHIVE), true)
		ifeq ($(ARCHIVE_STATIC), true)
			# Creating Static Library
			TARGET_NAME := lib$(TARGET_NAME).a
			LINKER_FLAGS := $(INC_FLAGS) $(PROJECT_STRUCTURE_FLAGS) $(COMPILER_FLAGS) $(LINUX_LINKER_LIBS)
		else
			# Creating Shared Object
			TARGET_NAME := lib$(TARGET_NAME).so
			LINKER_FLAGS := -shared $(INC_FLAGS) $(PROJECT_STRUCTURE_FLAGS) $(COMPILER_FLAGS) $(LINUX_LINKER_LIBS) -fPIC
		endif
	else
		# Creating Executable
		TARGET_NAME := $(TARGET_NAME).out
		LINKER_FLAGS := $(INC_FLAGS) $(PROJECT_STRUCTURE_FLAGS)$(COMPILER_FLAGS) $(LINUX_LINKER_LIBS)
	endif
endif

# Windows
ifeq ($(TARGET_OS), windows)
	ifeq ($(TARGET_CPU), i686)
		# 32bit Compiler
		COMPILER ?= i686-w64-mingw32-gcc
		ARCHIVER ?= i686-w64-mingw32-ar
		# 32bit Linker Libs
		PROJECT_STRUCTURE_FLAGS := -L ./lib/windows/x86_64/ -I ./inc/ -I /usr/include/ -lgdi32 -lws2_32 -lwinmm -lm
	else ifeq ($(TARGET_CPU), x86_64)
		# 64bit Compiler
		COMPILER ?= x86_64-w64-mingw32-gcc
		ARCHIVER ?= x86_64-w64-mingw32-ar
		# 364bit Linker Libs
		PROJECT_STRUCTURE_FLAGS := -L ./lib/windows/x86_64/ -I ./inc/ -I /usr/include/ -lgdi32 -lws2_32 -lwinmm -lm
	endif
	ifeq ($(ARCHIVE), true)
		ifeq ($(ARCHIVE_STATIC), true)
			# Creating Static Library
			TARGET_NAME := $(TARGET_NAME).lib
			LINKER_FLAGS := $(INC_FLAGS) $(PROJECT_STRUCTURE_FLAGS) $(COMPILER_FLAGS) $(WINDOWS_LINKER_LIBS)
		else
			# Creating Dynamic-Link Library
			TARGET_NAME := $(TARGET_NAME).dll
			LINKER_FLAGS := -DBUILDING_EXAMPLE_DLL -shared $(INC_FLAGS) $(PROJECT_STRUCTURE_FLAGS) $(COMPILER_FLAGS) $(WINDOWS_LINKER_LIBS) -fPIC
		endif
	else
		# Creating Executable
		TARGET_NAME := $(TARGET_NAME).exe
		LINKER_FLAGS := $(INC_FLAGS) $(PROJECT_STRUCTURE_FLAGS) $(COMPILER_FLAGS) $(WINDOWS_LINKER_LIBS)
	endif
endif

 #############
##  ( V) Cmp ## This is Cmp: the compiling bunny!
##  ( ..)    ## Cmp enjoys his job, but equaly values his time off.
##  c(")(")  ## Help Cmp get his job done faster by contributing to his instructions
## B u n n y ## or by optimizing the source code he needs to compile.
 #############

# Compiling/Archiving Commands
ifeq ($(ARCHIVE_STATIC), true)
	PRE_ASSEMBLY_COMMAND := $(ARCHIVER) rcs
	POST_ASSEMBLY_COMMAND := $(OBJS)
else
	PRE_ASSEMBLY_COMMAND := $(COMPILER) $(OBJS) -o
	POST_ASSEMBLY_COMMAND := $(LDFLAGS) $(LINKER_FLAGS)
endif

# generate output
$(BUILD_DIR)/$(TARGET_NAME): $(OBJS)
	$(PRE_ASSEMBLY_COMMAND) $@ $(POST_ASSEMBLY_COMMAND)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(COMPILER) $(CFLAGS) $(LINKER_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR_ROOT) # Delete build folder... In future, delete just src folders

-include $(DEPS)

MKDIR_P ?= mkdir -p
