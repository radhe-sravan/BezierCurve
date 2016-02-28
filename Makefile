all: bezier1 bezier2
bezier1:
	gcc bezier1.c -lGLU -lglut -lGL -lm -o bezier1
bezier2:
	gcc bezier2.c -lGLU -lglut -lGL -lm -o bezier2
