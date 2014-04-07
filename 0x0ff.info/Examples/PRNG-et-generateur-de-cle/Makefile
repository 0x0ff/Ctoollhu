CC = gcc
CFLAGS = -Wall
LFLAGS = -lgmp -lm -lssl -g
EXEC_INIT=init
EXEC_GEN=generator
EXEC_VAL=validator

init :
	$(CC) -O3 -o $(EXEC_INIT) src/init.c src/primes.c src/obfuscation.c src/crypt.c $(LFLAGS)
gen :
	$(CC) -O3 -o $(EXEC_GEN) src/generator.c src/traductor.c src/prng.c src/obfuscation.c src/crypt.c $(LFLAGS)
	$(CC) -O3 -o $(EXEC_VAL) src/validate.c src/traductor.c src/prng.c src/obfuscation.c src/crypt.c $(LFLAGS)
heap :
	$(CC) -O3 -o $(EXEC_GEN) src/generator.c src/traductor.c src/prng.c src/obfuscation.c src/crypt.c $(LFLAGS)
	$(CC) -O3 -o $(EXEC_VAL) src/pocHeapBufferOverflow/validate.c src/traductor.c src/prng.c src/obfuscation.c src/crypt.c $(LFLAGS)
clean :
	rm -rf $(EXEC_GEN) $(EXEC_VAL)

clean_all :
	rm -rf $(EXEC_INIT) $(EXEC_GEN) $(EXEC_VAL)

	
