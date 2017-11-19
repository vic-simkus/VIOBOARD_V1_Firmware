#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-dsPIC30F4013.mk)" "nbproject/Makefile-local-dsPIC30F4013.mk"
include nbproject/Makefile-local-dsPIC30F4013.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=dsPIC30F4013
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=a
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=a
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/I2C/i2c_callbacks.c src/I2C/i2c_isr_master.c src/I2C/simkus_net.c src/I2C/i2c_logger_reg.c src/I2C/i2c.c src/I2C/i2c_logger.c src/I2C/i2c_isr_slave.c src/support.c src/calibration.c src/support_asm.s src/eeprom_utils.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/I2C/i2c_callbacks.o ${OBJECTDIR}/src/I2C/i2c_isr_master.o ${OBJECTDIR}/src/I2C/simkus_net.o ${OBJECTDIR}/src/I2C/i2c_logger_reg.o ${OBJECTDIR}/src/I2C/i2c.o ${OBJECTDIR}/src/I2C/i2c_logger.o ${OBJECTDIR}/src/I2C/i2c_isr_slave.o ${OBJECTDIR}/src/support.o ${OBJECTDIR}/src/calibration.o ${OBJECTDIR}/src/support_asm.o ${OBJECTDIR}/src/eeprom_utils.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/I2C/i2c_callbacks.o.d ${OBJECTDIR}/src/I2C/i2c_isr_master.o.d ${OBJECTDIR}/src/I2C/simkus_net.o.d ${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d ${OBJECTDIR}/src/I2C/i2c.o.d ${OBJECTDIR}/src/I2C/i2c_logger.o.d ${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d ${OBJECTDIR}/src/support.o.d ${OBJECTDIR}/src/calibration.o.d ${OBJECTDIR}/src/support_asm.o.d ${OBJECTDIR}/src/eeprom_utils.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/I2C/i2c_callbacks.o ${OBJECTDIR}/src/I2C/i2c_isr_master.o ${OBJECTDIR}/src/I2C/simkus_net.o ${OBJECTDIR}/src/I2C/i2c_logger_reg.o ${OBJECTDIR}/src/I2C/i2c.o ${OBJECTDIR}/src/I2C/i2c_logger.o ${OBJECTDIR}/src/I2C/i2c_isr_slave.o ${OBJECTDIR}/src/support.o ${OBJECTDIR}/src/calibration.o ${OBJECTDIR}/src/support_asm.o ${OBJECTDIR}/src/eeprom_utils.o

# Source Files
SOURCEFILES=src/I2C/i2c_callbacks.c src/I2C/i2c_isr_master.c src/I2C/simkus_net.c src/I2C/i2c_logger_reg.c src/I2C/i2c.c src/I2C/i2c_logger.c src/I2C/i2c_isr_slave.c src/support.c src/calibration.c src/support_asm.s src/eeprom_utils.s


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-dsPIC30F4013.mk dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4013
MP_LINKER_FILE_OPTION=,--script=p30F4013.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/I2C/i2c_callbacks.o: src/I2C/i2c_callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_callbacks.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_callbacks.c  -o ${OBJECTDIR}/src/I2C/i2c_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_callbacks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_callbacks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_isr_master.o: src/I2C/i2c_isr_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_master.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_isr_master.c  -o ${OBJECTDIR}/src/I2C/i2c_isr_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_isr_master.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_isr_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/simkus_net.o: src/I2C/simkus_net.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/simkus_net.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/simkus_net.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/simkus_net.c  -o ${OBJECTDIR}/src/I2C/simkus_net.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/simkus_net.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/simkus_net.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_logger_reg.o: src/I2C/i2c_logger_reg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger_reg.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_logger_reg.c  -o ${OBJECTDIR}/src/I2C/i2c_logger_reg.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c.o: src/I2C/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c.c  -o ${OBJECTDIR}/src/I2C/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_logger.o: src/I2C/i2c_logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_logger.c  -o ${OBJECTDIR}/src/I2C/i2c_logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_logger.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_isr_slave.o: src/I2C/i2c_isr_slave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_slave.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_isr_slave.c  -o ${OBJECTDIR}/src/I2C/i2c_isr_slave.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/support.o: src/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/support.o.d 
	@${RM} ${OBJECTDIR}/src/support.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/support.c  -o ${OBJECTDIR}/src/support.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/support.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/support.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/calibration.o: src/calibration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/calibration.o.d 
	@${RM} ${OBJECTDIR}/src/calibration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/calibration.c  -o ${OBJECTDIR}/src/calibration.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/calibration.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1    -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/calibration.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/src/I2C/i2c_callbacks.o: src/I2C/i2c_callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_callbacks.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_callbacks.c  -o ${OBJECTDIR}/src/I2C/i2c_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_callbacks.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_callbacks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_isr_master.o: src/I2C/i2c_isr_master.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_master.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_master.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_isr_master.c  -o ${OBJECTDIR}/src/I2C/i2c_isr_master.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_isr_master.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_isr_master.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/simkus_net.o: src/I2C/simkus_net.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/simkus_net.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/simkus_net.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/simkus_net.c  -o ${OBJECTDIR}/src/I2C/simkus_net.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/simkus_net.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/simkus_net.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_logger_reg.o: src/I2C/i2c_logger_reg.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger_reg.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_logger_reg.c  -o ${OBJECTDIR}/src/I2C/i2c_logger_reg.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_logger_reg.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c.o: src/I2C/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c.c  -o ${OBJECTDIR}/src/I2C/i2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_logger.o: src/I2C/i2c_logger.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_logger.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_logger.c  -o ${OBJECTDIR}/src/I2C/i2c_logger.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_logger.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_logger.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/I2C/i2c_isr_slave.o: src/I2C/i2c_isr_slave.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src/I2C" 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d 
	@${RM} ${OBJECTDIR}/src/I2C/i2c_isr_slave.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/I2C/i2c_isr_slave.c  -o ${OBJECTDIR}/src/I2C/i2c_isr_slave.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/I2C/i2c_isr_slave.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/support.o: src/support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/support.o.d 
	@${RM} ${OBJECTDIR}/src/support.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/support.c  -o ${OBJECTDIR}/src/support.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/support.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/support.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/calibration.o: src/calibration.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/calibration.o.d 
	@${RM} ${OBJECTDIR}/src/calibration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/calibration.c  -o ${OBJECTDIR}/src/calibration.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/calibration.o.d"        -g -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -mconst-in-data -O1 -msmart-io=1 -Werror -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/calibration.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/support_asm.o: src/support_asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/support_asm.o.d 
	@${RM} ${OBJECTDIR}/src/support_asm.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/support_asm.s  -o ${OBJECTDIR}/src/support_asm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  -I"include" -Wa,-MD,"${OBJECTDIR}/src/support_asm.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/support_asm.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eeprom_utils.o: src/eeprom_utils.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o.d 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/eeprom_utils.s  -o ${OBJECTDIR}/src/eeprom_utils.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  -I"include" -Wa,-MD,"${OBJECTDIR}/src/eeprom_utils.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/eeprom_utils.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/support_asm.o: src/support_asm.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/support_asm.o.d 
	@${RM} ${OBJECTDIR}/src/support_asm.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/support_asm.s  -o ${OBJECTDIR}/src/support_asm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  -I"include" -Wa,-MD,"${OBJECTDIR}/src/support_asm.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/support_asm.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eeprom_utils.o: src/eeprom_utils.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o.d 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/eeprom_utils.s  -o ${OBJECTDIR}/src/eeprom_utils.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DFCY=10000000UL -DI2C_SLAVE_REGISTER_COUNT=0x12 -save-temps=obj -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  -I"include" -Wa,-MD,"${OBJECTDIR}/src/eeprom_utils.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/src/eeprom_utils.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: archive
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX} 
	${MP_AR} $(MP_EXTRA_AR_PRE)  -omf=elf -r dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX} 
	${MP_AR} $(MP_EXTRA_AR_PRE)  -omf=elf -r dist/${CND_CONF}/${IMAGE_TYPE}/SUPPORT.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/dsPIC30F4013
	${RM} -r dist/dsPIC30F4013

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
