#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "gattlib.h"

// serial service
uuid_t g_serial_uuid; // 0xFFE1

gatt_connection_t* connection;

void notification_handler(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data) {
	int i;

	for (i = 0; i < data_length; i++) {
        putchar(data[i]);
	}
    fflush(stdout);
}

static gboolean stdin_callback(GIOChannel *io, GIOCondition condition, gpointer data) {
    GError *error = NULL;

    gchar in, cr = '\r';
    switch (g_io_channel_read_chars(io, &in, 1, NULL, &error)) {
        case G_IO_STATUS_NORMAL:

            // Write to the BLE characteristic implementing serial TX
            if (in == '\n') gattlib_write_char_by_uuid(connection, &g_serial_uuid, &cr, 1);
            gattlib_write_char_by_uuid(connection, &g_serial_uuid, &in, 1);
            return TRUE;
        case G_IO_STATUS_ERROR:
        case G_IO_STATUS_EOF:
        case G_IO_STATUS_AGAIN:
            break;
    }
    return FALSE;
}

void ctrlc_handler(int dummy) {
    gattlib_disconnect(connection);
    exit(0);
}

static void usage(char *argv[]) {
	printf("%s -d <device_address> -c <characteristic>\n", argv[0]);
}

char baddr[16] = {0};
int main(int argc, char *argv[]) {
	int ret;

    int op,u,test = 0;
    while ((op = getopt(argc, argv, "d:c:")) != -1) {
        switch (op) {
            case 'd':
                strcpy(baddr, optarg);
                test |= 0x01;
            break;
            case 'c':
                gattlib_string_to_uuid(optarg, strlen(optarg)+1, &g_serial_uuid);
                test |= 0x02;
            break;
            default:
                usage(argv);
                exit(1);
            break;
        }
    }

    if (test != 3) {
        usage(argv);
        exit(1);
    }

	connection = gattlib_connect(NULL, argv[1], BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	if (connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 1;
	}

	gattlib_register_notification(connection, notification_handler, NULL);

	ret = gattlib_notification_start(connection, &g_serial_uuid);
	if (ret) {
		fprintf(stderr, "Fail to start notification\n.");
		return 1;
	}

    signal(SIGINT, ctrlc_handler);
    setbuf(stdin, NULL);

    GIOChannel *io = NULL;
    io = g_io_channel_unix_new(0); // STDIN_FILENO = 0
    g_io_add_watch(io, G_IO_IN, stdin_callback, NULL);
    g_io_channel_unref(io);

	GMainLoop *loop = g_main_loop_new(NULL, 0);
	g_main_loop_run(loop);

	g_main_loop_unref(loop);
	gattlib_disconnect(connection);
	return 0;
}
