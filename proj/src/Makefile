# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/game

# source code files to be compiled  serial_pPOrt.Port.c
SRCS = main.c video.c utils.c keyboard.c KBC.c mouse.c wombat.c timer.c sprite.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>