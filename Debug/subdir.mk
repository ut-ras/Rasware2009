################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RASDemo.c 

CMD_SRCS += \
../lm3s811.cmd 

OBJS += \
./RASDemo.obj 

C_DEPS += \
./RASDemo.pp 

OBJS__QTD += \
".\RASDemo.obj" 

C_DEPS__QTD += \
".\RASDemo.pp" 

C_SRCS_QUOTED += \
"../RASDemo.c" 


# Each subdirectory must supply rules for building sources it contributes
RASDemo.obj: ../RASDemo.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --include_path="C:/Users/MikeMay/working_dir/raslib/RASLib/include" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="RASDemo.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


