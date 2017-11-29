uart: uart.c
	gcc -o uart uart.c `pkg-config --cflags --libs glib-2.0` -lbluetooth -lgattlib 
