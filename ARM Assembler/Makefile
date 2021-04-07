PROGS = project04
OBJS = armasm.o scan.o parse.o codegen.o 
HEADERS = armasm.h

ELF = elf/elf.a

CFLAGS = -g

# Pattern rules to avoid explicit rules
%.o : %.c
	gcc ${CFLAGS} -c -o $@ $<

%.o : %.s
	as -o $@ $<

all : ${PROGS}

$(ELF):
	make -C elf

project04: ${HEADERS} ${OBJS} ${ELF}
	gcc ${CFLAGS} -o $@ ${OBJS} ${ELF}

clean :
	make -C elf clean
	rm -rf ${PROGS} ${OBJS} *~ x
