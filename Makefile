monitor: main.c Lx_Loadaverage.c Lx_grepMemavailable.c cpu_monitor.c
	gcc -o monitor main.c Lx_Loadaverage.c Lx_grepMemavailable.c cpu_monitor.c 
clean:
	rm -f monitor
