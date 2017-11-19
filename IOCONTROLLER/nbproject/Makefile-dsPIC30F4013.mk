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
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=src/setup.c src/i2c_callbacks.c src/globals.c src/isrs.c src/main.c src/eeprom_utils.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/setup.o ${OBJECTDIR}/src/i2c_callbacks.o ${OBJECTDIR}/src/globals.o ${OBJECTDIR}/src/isrs.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/eeprom_utils.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/setup.o.d ${OBJECTDIR}/src/i2c_callbacks.o.d ${OBJECTDIR}/src/globals.o.d ${OBJECTDIR}/src/isrs.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/eeprom_utils.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/setup.o ${OBJECTDIR}/src/i2c_callbacks.o ${OBJECTDIR}/src/globals.o ${OBJECTDIR}/src/isrs.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/eeprom_utils.o

# Source Files
SOURCEFILES=src/setup.c src/i2c_callbacks.c src/globals.c src/isrs.c src/main.c src/eeprom_utils.c


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
	${MAKE}  -f nbproject/Makefile-dsPIC30F4013.mk dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4013
MP_LINKER_FILE_OPTION=,--script=p30F4013.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/setup.o: src/setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup.o.d 
	@${RM} ${OBJECTDIR}/src/setup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/setup.c  -o ${OBJECTDIR}/src/setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/setup.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/setup.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/i2c_callbacks.o: src/i2c_callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c_callbacks.o.d 
	@${RM} ${OBJECTDIR}/src/i2c_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c_callbacks.c  -o ${OBJECTDIR}/src/i2c_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/i2c_callbacks.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/i2c_callbacks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/globals.o: src/globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/globals.o.d 
	@${RM} ${OBJECTDIR}/src/globals.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/globals.c  -o ${OBJECTDIR}/src/globals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/globals.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/globals.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/isrs.o: src/isrs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isrs.o.d 
	@${RM} ${OBJECTDIR}/src/isrs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/isrs.c  -o ${OBJECTDIR}/src/isrs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/isrs.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/isrs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/eeprom_utils.o: src/eeprom_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o.d 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/eeprom_utils.c  -o ${OBJECTDIR}/src/eeprom_utils.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/eeprom_utils.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/eeprom_utils.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/src/setup.o: src/setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup.o.d 
	@${RM} ${OBJECTDIR}/src/setup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/setup.c  -o ${OBJECTDIR}/src/setup.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/setup.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/setup.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/i2c_callbacks.o: src/i2c_callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/i2c_callbacks.o.d 
	@${RM} ${OBJECTDIR}/src/i2c_callbacks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/i2c_callbacks.c  -o ${OBJECTDIR}/src/i2c_callbacks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/i2c_callbacks.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/i2c_callbacks.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/globals.o: src/globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/globals.o.d 
	@${RM} ${OBJECTDIR}/src/globals.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/globals.c  -o ${OBJECTDIR}/src/globals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/globals.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/globals.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/isrs.o: src/isrs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isrs.o.d 
	@${RM} ${OBJECTDIR}/src/isrs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/isrs.c  -o ${OBJECTDIR}/src/isrs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/isrs.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/isrs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/eeprom_utils.o: src/eeprom_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o.d 
	@${RM} ${OBJECTDIR}/src/eeprom_utils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/eeprom_utils.c  -o ${OBJECTDIR}/src/eeprom_utils.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/eeprom_utils.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -mconst-in-data -O1 -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/eeprom_utils.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../SUPPORT/dist/dsPIC30F4013/debug/SUPPORT.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../SUPPORT/dist/dsPIC30F4013/debug/SUPPORT.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include"  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=32,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../SUPPORT/dist/dsPIC30F4013/production/SUPPORT.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../SUPPORT/dist/dsPIC30F4013/production/SUPPORT.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x12 -DFCY=10000000UL -DXPRJ_dsPIC30F4013=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=32,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/IOCONTROLLER.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:
	cd ../SUPPORT && ${MAKE}  -f Makefile CONF=dsPIC30F4013


# Subprojects
.clean-subprojects:
	cd ../SUPPORT && rm -rf "build/dsPIC30F4013" "dist/dsPIC30F4013"

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
