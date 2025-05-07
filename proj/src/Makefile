# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/game

# source code files to be compiled  serial_pPOrt.Port.c
SRCS = \
  main.c \
  io/utils.c \
  io/video/video.c \
  io/timer/timer.c \
  io/KBC/KBC.c \
  io/KBC/keyboard/keyboard.c \
  io/KBC/mouse/mouse.c \
  display/sprite.c \
  game/wombat.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>