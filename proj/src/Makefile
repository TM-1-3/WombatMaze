# Name of the program (Minix service)
PROG=proj

# Search paths
.PATH: ${.CURDIR}/io/
.PATH: ${.CURDIR}/io/KBC
.PATH: ${.CURDIR}/io/KBC/keyboard
.PATH: ${.CURDIR}/io/KBC/mouse
.PATH: ${.CURDIR}/io/timer
.PATH: ${.CURDIR}/io/video
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/display

# Source files to be compiled
SRCS = 	main.c timer.c utils.c sprite.c keyboard.c mouse.c video.c KBC.c wombat.c maze.c dingoe.c

# Object file output dir
OBJDIR = obj

# Tell make to output object files into obj/
MAKEOBJDIRPREFIX = ${.CURDIR}/${OBJDIR}

# Compilation flags
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# Library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# Include LCOM's Makefile
.include <minix.lcom.mk>