objects = main.o input.o tokenizer.o interpreter.o helpers.o

expression : $(objects)
	cc -o expression $(objects)

.PHONY : clean
clean :
	-rm expression $(objects)