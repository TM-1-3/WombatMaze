# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/io/
.PATH: ${.CURDIR}/io/KBC
.PATH: ${.CURDIR}/io/KBC/keyboard
.PATH: ${.CURDIR}/io/KBC/mouse
.PATH: ${.CURDIR}/io/timer
.PATH: ${.CURDIR}/io/video
.PATH: ${.CURDIR}/game
.PATH: ${.CURDIR}/display

# source code files to be compiled  serial_pPOrt.Port.c
SRCS = 	main.c timer.c utils.c sprite.c keyboard.c mouse.c video.c KBC.c wombat.c maze.c

# ./controllers/keyboardMouse/KBC.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>