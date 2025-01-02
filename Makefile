all:
	cc main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o shaderbrot