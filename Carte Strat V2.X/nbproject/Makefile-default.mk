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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Fonctions_deplacements.c Uart.c asserv.c autom_R1.c autom_R2.c codeurs.c debug.c gestion_AX12.c interruptions.c main.c pwm.c strategie.c system.c Serialus.c evitement.c autom_virtual.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Fonctions_deplacements.o ${OBJECTDIR}/Uart.o ${OBJECTDIR}/asserv.o ${OBJECTDIR}/autom_R1.o ${OBJECTDIR}/autom_R2.o ${OBJECTDIR}/codeurs.o ${OBJECTDIR}/debug.o ${OBJECTDIR}/gestion_AX12.o ${OBJECTDIR}/interruptions.o ${OBJECTDIR}/main.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/strategie.o ${OBJECTDIR}/system.o ${OBJECTDIR}/Serialus.o ${OBJECTDIR}/evitement.o ${OBJECTDIR}/autom_virtual.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Fonctions_deplacements.o.d ${OBJECTDIR}/Uart.o.d ${OBJECTDIR}/asserv.o.d ${OBJECTDIR}/autom_R1.o.d ${OBJECTDIR}/autom_R2.o.d ${OBJECTDIR}/codeurs.o.d ${OBJECTDIR}/debug.o.d ${OBJECTDIR}/gestion_AX12.o.d ${OBJECTDIR}/interruptions.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/pwm.o.d ${OBJECTDIR}/strategie.o.d ${OBJECTDIR}/system.o.d ${OBJECTDIR}/Serialus.o.d ${OBJECTDIR}/evitement.o.d ${OBJECTDIR}/autom_virtual.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Fonctions_deplacements.o ${OBJECTDIR}/Uart.o ${OBJECTDIR}/asserv.o ${OBJECTDIR}/autom_R1.o ${OBJECTDIR}/autom_R2.o ${OBJECTDIR}/codeurs.o ${OBJECTDIR}/debug.o ${OBJECTDIR}/gestion_AX12.o ${OBJECTDIR}/interruptions.o ${OBJECTDIR}/main.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/strategie.o ${OBJECTDIR}/system.o ${OBJECTDIR}/Serialus.o ${OBJECTDIR}/evitement.o ${OBJECTDIR}/autom_virtual.o

# Source Files
SOURCEFILES=Fonctions_deplacements.c Uart.c asserv.c autom_R1.c autom_R2.c codeurs.c debug.c gestion_AX12.c interruptions.c main.c pwm.c strategie.c system.c Serialus.c evitement.c autom_virtual.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128MC804
MP_LINKER_FILE_OPTION=,--script=p33FJ128MC804.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Fonctions_deplacements.o: Fonctions_deplacements.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Fonctions_deplacements.o.d 
	@${RM} ${OBJECTDIR}/Fonctions_deplacements.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Fonctions_deplacements.c  -o ${OBJECTDIR}/Fonctions_deplacements.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Fonctions_deplacements.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/Fonctions_deplacements.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Uart.o: Uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Uart.o.d 
	@${RM} ${OBJECTDIR}/Uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Uart.c  -o ${OBJECTDIR}/Uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/Uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/asserv.o: asserv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/asserv.o.d 
	@${RM} ${OBJECTDIR}/asserv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  asserv.c  -o ${OBJECTDIR}/asserv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asserv.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/asserv.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/autom_R1.o: autom_R1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/autom_R1.o.d 
	@${RM} ${OBJECTDIR}/autom_R1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  autom_R1.c  -o ${OBJECTDIR}/autom_R1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/autom_R1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/autom_R1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/autom_R2.o: autom_R2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/autom_R2.o.d 
	@${RM} ${OBJECTDIR}/autom_R2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  autom_R2.c  -o ${OBJECTDIR}/autom_R2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/autom_R2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/autom_R2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/codeurs.o: codeurs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/codeurs.o.d 
	@${RM} ${OBJECTDIR}/codeurs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  codeurs.c  -o ${OBJECTDIR}/codeurs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/codeurs.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/codeurs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/debug.o: debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/debug.o.d 
	@${RM} ${OBJECTDIR}/debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  debug.c  -o ${OBJECTDIR}/debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/debug.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/gestion_AX12.o: gestion_AX12.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gestion_AX12.o.d 
	@${RM} ${OBJECTDIR}/gestion_AX12.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  gestion_AX12.c  -o ${OBJECTDIR}/gestion_AX12.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/gestion_AX12.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/gestion_AX12.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interruptions.o: interruptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interruptions.o.d 
	@${RM} ${OBJECTDIR}/interruptions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interruptions.c  -o ${OBJECTDIR}/interruptions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interruptions.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/interruptions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/pwm.o: pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pwm.c  -o ${OBJECTDIR}/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/strategie.o: strategie.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/strategie.o.d 
	@${RM} ${OBJECTDIR}/strategie.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  strategie.c  -o ${OBJECTDIR}/strategie.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/strategie.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/strategie.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system.o: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system.c  -o ${OBJECTDIR}/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Serialus.o: Serialus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Serialus.o.d 
	@${RM} ${OBJECTDIR}/Serialus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Serialus.c  -o ${OBJECTDIR}/Serialus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Serialus.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/Serialus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/evitement.o: evitement.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/evitement.o.d 
	@${RM} ${OBJECTDIR}/evitement.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  evitement.c  -o ${OBJECTDIR}/evitement.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/evitement.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/evitement.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/autom_virtual.o: autom_virtual.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/autom_virtual.o.d 
	@${RM} ${OBJECTDIR}/autom_virtual.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  autom_virtual.c  -o ${OBJECTDIR}/autom_virtual.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/autom_virtual.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/autom_virtual.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/Fonctions_deplacements.o: Fonctions_deplacements.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Fonctions_deplacements.o.d 
	@${RM} ${OBJECTDIR}/Fonctions_deplacements.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Fonctions_deplacements.c  -o ${OBJECTDIR}/Fonctions_deplacements.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Fonctions_deplacements.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/Fonctions_deplacements.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Uart.o: Uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Uart.o.d 
	@${RM} ${OBJECTDIR}/Uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Uart.c  -o ${OBJECTDIR}/Uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Uart.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/Uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/asserv.o: asserv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/asserv.o.d 
	@${RM} ${OBJECTDIR}/asserv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  asserv.c  -o ${OBJECTDIR}/asserv.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/asserv.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/asserv.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/autom_R1.o: autom_R1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/autom_R1.o.d 
	@${RM} ${OBJECTDIR}/autom_R1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  autom_R1.c  -o ${OBJECTDIR}/autom_R1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/autom_R1.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/autom_R1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/autom_R2.o: autom_R2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/autom_R2.o.d 
	@${RM} ${OBJECTDIR}/autom_R2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  autom_R2.c  -o ${OBJECTDIR}/autom_R2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/autom_R2.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/autom_R2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/codeurs.o: codeurs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/codeurs.o.d 
	@${RM} ${OBJECTDIR}/codeurs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  codeurs.c  -o ${OBJECTDIR}/codeurs.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/codeurs.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/codeurs.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/debug.o: debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/debug.o.d 
	@${RM} ${OBJECTDIR}/debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  debug.c  -o ${OBJECTDIR}/debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/debug.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/gestion_AX12.o: gestion_AX12.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/gestion_AX12.o.d 
	@${RM} ${OBJECTDIR}/gestion_AX12.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  gestion_AX12.c  -o ${OBJECTDIR}/gestion_AX12.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/gestion_AX12.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/gestion_AX12.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interruptions.o: interruptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interruptions.o.d 
	@${RM} ${OBJECTDIR}/interruptions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interruptions.c  -o ${OBJECTDIR}/interruptions.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interruptions.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/interruptions.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/pwm.o: pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pwm.c  -o ${OBJECTDIR}/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/pwm.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/pwm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/strategie.o: strategie.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/strategie.o.d 
	@${RM} ${OBJECTDIR}/strategie.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  strategie.c  -o ${OBJECTDIR}/strategie.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/strategie.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/strategie.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/system.o: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system.c  -o ${OBJECTDIR}/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/system.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Serialus.o: Serialus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Serialus.o.d 
	@${RM} ${OBJECTDIR}/Serialus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Serialus.c  -o ${OBJECTDIR}/Serialus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Serialus.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/Serialus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/evitement.o: evitement.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/evitement.o.d 
	@${RM} ${OBJECTDIR}/evitement.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  evitement.c  -o ${OBJECTDIR}/evitement.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/evitement.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/evitement.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/autom_virtual.o: autom_virtual.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/autom_virtual.o.d 
	@${RM} ${OBJECTDIR}/autom_virtual.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  autom_virtual.c  -o ${OBJECTDIR}/autom_virtual.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/autom_virtual.o.d"        -g -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -fno-short-double -O0 -msmart-io=1 -Werror -Wall -msfr-warn=off   -fshort-enums
	@${FIXDEPS} "${OBJECTDIR}/autom_virtual.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
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
dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -legacy-libc  $(COMPARISON_BUILD)  -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Carte_Strat_V2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
