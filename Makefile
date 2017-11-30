bleuart: bleuart.c
	gcc -o bleuart bleuart.c `pkg-config --cflags --libs glib-2.0` -lbluetooth -lgattlib 
