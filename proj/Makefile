# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = SRCS = main.c video.c utils.c keyboard.c KBC.c mouse.c dingoe.c game.c maze.c obstacle.c wombat.c timer.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DPROJ

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
