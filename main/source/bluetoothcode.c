#include "ssd1306_i2c.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <pthread.h>
struct ps
{
	int st;
	pthread_t *thr;
};
//
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//

int status = 0;
void *recvsocket(void *arg)//
{
	struct ps *p = (struct ps *)arg;
	int st = p->st;
	char s[1024];
 
	while(1)
	{
		memset(s, 0, sizeof(s));
		//bytes_read = read(client, buf, sizeof(buf));
		int rc = read(st, s, sizeof(s));
		if (rc <= 0)//
			break;
		printf("phone：%s", s);
		printf("Display OLED\n");
		ssd1306_Display();
		ssd1306_drawString(s);
		ssd1306_display();
		//delay(100);
               }
	
	status = 0;
	pthread_mutex_unlock(&mutex);
	pthread_cancel(*(p->thr));
	return NULL;
}
	
void *sendsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];
	while(1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s));
		write(st, s, strlen(s));
	}
	return NULL;
}

int main(int arg, char *args[]) {
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    //ssd1306 begin
	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening 
