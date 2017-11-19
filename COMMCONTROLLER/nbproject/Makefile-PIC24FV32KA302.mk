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
ifeq "$(wildcard nbproject/Makefile-local-PIC24FV32KA302.mk)" "nbproject/Makefile-local-PIC24FV32KA302.mk"
include nbproject/Makefile-local-PIC24FV32KA302.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC24FV32KA302
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=src/globals.c src/cp_ic.c src/command_processor.c src/cp_bcc.c src/rtcc.c src/main.c src/iocontroller_interface.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/globals.o ${OBJECTDIR}/src/cp_ic.o ${OBJECTDIR}/src/command_processor.o ${OBJECTDIR}/src/cp_bcc.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/iocontroller_interface.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/globals.o.d ${OBJECTDIR}/src/cp_ic.o.d ${OBJECTDIR}/src/command_processor.o.d ${OBJECTDIR}/src/cp_bcc.o.d ${OBJECTDIR}/src/rtcc.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/iocontroller_interface.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/globals.o ${OBJECTDIR}/src/cp_ic.o ${OBJECTDIR}/src/command_processor.o ${OBJECTDIR}/src/cp_bcc.o ${OBJECTDIR}/src/rtcc.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/iocontroller_interface.o

# Source Files
SOURCEFILES=src/globals.c src/cp_ic.c src/command_processor.c src/cp_bcc.c src/rtcc.c src/main.c src/iocontroller_interface.c


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
	${MAKE}  -f nbproject/Makefile-PIC24FV32KA302.mk dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FV32KA302
MP_LINKER_FILE_OPTION=,--script=p24FV32KA302.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/globals.o: src/globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/globals.o.d 
	@${RM} ${OBJECTDIR}/src/globals.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/globals.c  -o ${OBJECTDIR}/src/globals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/globals.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/globals.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/cp_ic.o: src/cp_ic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cp_ic.o.d 
	@${RM} ${OBJECTDIR}/src/cp_ic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cp_ic.c  -o ${OBJECTDIR}/src/cp_ic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/cp_ic.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/cp_ic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/command_processor.o: src/command_processor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/command_processor.o.d 
	@${RM} ${OBJECTDIR}/src/command_processor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/command_processor.c  -o ${OBJECTDIR}/src/command_processor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/command_processor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/command_processor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/cp_bcc.o: src/cp_bcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cp_bcc.o.d 
	@${RM} ${OBJECTDIR}/src/cp_bcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cp_bcc.c  -o ${OBJECTDIR}/src/cp_bcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/cp_bcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/cp_bcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/rtcc.c  -o ${OBJECTDIR}/src/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/rtcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/rtcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/iocontroller_interface.o: src/iocontroller_interface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/iocontroller_interface.o.d 
	@${RM} ${OBJECTDIR}/src/iocontroller_interface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/iocontroller_interface.c  -o ${OBJECTDIR}/src/iocontroller_interface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/iocontroller_interface.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/iocontroller_interface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/src/globals.o: src/globals.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/globals.o.d 
	@${RM} ${OBJECTDIR}/src/globals.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/globals.c  -o ${OBJECTDIR}/src/globals.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/globals.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/globals.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/cp_ic.o: src/cp_ic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cp_ic.o.d 
	@${RM} ${OBJECTDIR}/src/cp_ic.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cp_ic.c  -o ${OBJECTDIR}/src/cp_ic.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/cp_ic.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/cp_ic.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/command_processor.o: src/command_processor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/command_processor.o.d 
	@${RM} ${OBJECTDIR}/src/command_processor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/command_processor.c  -o ${OBJECTDIR}/src/command_processor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/command_processor.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/command_processor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/cp_bcc.o: src/cp_bcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cp_bcc.o.d 
	@${RM} ${OBJECTDIR}/src/cp_bcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cp_bcc.c  -o ${OBJECTDIR}/src/cp_bcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/cp_bcc.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/cp_bcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/rtcc.o: src/rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/rtcc.o.d 
	@${RM} ${OBJECTDIR}/src/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/rtcc.c  -o ${OBJECTDIR}/src/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/rtcc.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/rtcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/iocontroller_interface.o: src/iocontroller_interface.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/iocontroller_interface.o.d 
	@${RM} ${OBJECTDIR}/src/iocontroller_interface.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/iocontroller_interface.c  -o ${OBJECTDIR}/src/iocontroller_interface.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/iocontroller_interface.o.d"        -g -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off    -Wno-pointer-sign
	@${FIXDEPS} "${OBJECTDIR}/src/iocontroller_interface.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../SUPPORT/dist/PIC24FV32KA302/debug/SUPPORT.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../SUPPORT/dist/PIC24FV32KA302/debug/SUPPORT.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include"  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../SUPPORT/dist/PIC24FV32KA302/production/SUPPORT.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ../SUPPORT/dist/PIC24FV32KA302/production/SUPPORT.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DI2C_SLAVE_REGISTER_COUNT=0x0F -DFCY=16000000UL -save-temps=obj -DXPRJ_PIC24FV32KA302=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -I"include" -I"../SUPPORT/include" -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/COMMCONTROLLER.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:
	cd ../SUPPORT && ${MAKE}  -f Makefile CONF=PIC24FV32KA302


# Subprojects
.clean-subprojects:
	cd ../SUPPORT && rm -rf "build/PIC24FV32KA302" "dist/PIC24FV32KA302"

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC24FV32KA302
	${RM} -r dist/PIC24FV32KA302

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
