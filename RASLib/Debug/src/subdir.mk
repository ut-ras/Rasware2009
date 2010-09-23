################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/encoder.c \
../src/init.c \
../src/linesensor.c \
../src/motor.c \
../src/servo.c \
../src/timer.c 

OBJS += \
./src/encoder.obj \
./src/init.obj \
./src/linesensor.obj \
./src/motor.obj \
./src/servo.obj \
./src/timer.obj 

C_DEPS += \
./src/encoder.pp \
./src/init.pp \
./src/linesensor.pp \
./src/motor.pp \
./src/servo.pp \
./src/timer.pp 

OBJS__QTD += \
".\src\encoder.obj" \
".\src\init.obj" \
".\src\linesensor.obj" \
".\src\motor.obj" \
".\src\servo.obj" \
".\src\timer.obj" 

C_DEPS__QTD += \
".\src\encoder.pp" \
".\src\init.pp" \
".\src\linesensor.pp" \
".\src\motor.pp" \
".\src\servo.pp" \
".\src\timer.pp" 

C_SRCS_QUOTED += \
"../src/encoder.c" \
"../src/init.c" \
"../src/linesensor.c" \
"../src/motor.c" \
"../src/servo.c" \
"../src/timer.c" 


# Each subdirectory must supply rules for building sources it contributes
src/encoder.obj: ../src/encoder.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/encoder.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/init.obj: ../src/init.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/init.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/linesensor.obj: ../src/linesensor.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/linesensor.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/motor.obj: ../src/motor.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/motor.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/servo.obj: ../src/servo.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/servo.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

src/timer.obj: ../src/timer.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S811 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/timer.pp" --obj_directory="src" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


