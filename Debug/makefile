################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

EMPTY := 
SPACE := $(EMPTY) $(EMPTY)

ORDERED_OBJS += \
$(GEN_CMDS_QUOTED) \
"./RASDemo.obj" \
-l"rtsv7M3_T_le_eabi.lib" \
"../lm3s811.cmd" \

-include ../makefile.init

RM := DEL /F

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: RASdemo.out

dependents:
	-cd C:/StellarisWare/driverlib/ccs/Debug && $(MAKE) all
	-cd C:/Users/MikeMay/working_dir/raslib/RASLib/Debug && $(MAKE) all

# Tool invocations
RASdemo.out: $(GEN_CMDS) $(CMD_SRCS) $(OBJS) C:/StellarisWare/driverlib/ccs/Debug/driverlib.lib C:/Users/MikeMay/working_dir/raslib/RASLib/Debug/raslib.lib
	@echo 'Building target: $@'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual -z -m"RASdemo.map" --stack_size=256 --heap_size=0 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --reread_libs --rom_model -o "RASdemo.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QTD) "RASdemo.out"
	-$(RM) ".\RASDemo.obj" 
	-$(RM) ".\RASDemo.pp" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/StellarisWare/driverlib/ccs/Debug/driverlib.lib:
C:/Users/MikeMay/working_dir/raslib/RASLib/Debug/raslib.lib:

-include ../makefile.targets
