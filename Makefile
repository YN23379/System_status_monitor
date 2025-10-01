monitor: main.c Lx_Loadaverage.c Lx_grepMemavailable.c
	gcc -o monitor main.c Lx_Loadaverage.c Lx_grepMemavailable.c

clean:
	rm -f monitor
