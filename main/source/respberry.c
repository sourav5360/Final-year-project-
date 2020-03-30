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
		ssd1306_clearDisplay();
		ssd1306_drawString(s);
		ssd1306_display();
		//delay(100);
 
	}
	pthread_mutex_lock(&mutex);
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

	listen(s, 1);
	pthread_t thrd1, thrd2;
    while(1){
		// accept one connection
		printf("start pthread\n");
		client = accept(s, (struct sockaddr *)&rem_addr, &opt);
		pthread_mutex_lock(&mutex);
		status++;
		pthread_mutex_unlock(&mutex);
		if (status>1){
			close(client);
			continue;
		}
		if(client==-1){
			printf("failure\n");
		}
		
		ba2str( &rem_addr.rc_bdaddr, buf );
		fprintf(stderr, "accepted connection from %s\n", buf);
		//memset(buf, 0, sizeof(buf));
		struct ps ps1;
		ps1.st=client;
		ps1.thr=&thrd2;
		pthread_create(&thrd1,NULL,recvsocket,&ps1);
		pthread_detach(thrd1);
		pthread_create(&thrd2,NULL,sendsocket,&client);
		pthread_detach(thrd2);
	}
	close(client);
    close(s);
	while(1){
		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		
		
		
		if( bytes_read > 0 ) {
			printf("received [%s]\n", buf);
			//char* text = "This is demo for SSD1306 i2c driver for Raspberry Pi";
			ssd1306_clearDisplay();
			ssd1306_drawString(buf);
			ssd1306_display();
			delay(100);
		
		}
		else{
			printf("no receive");
		}
	}
    // close connection
    close(client);
    close(s);


	ssd1306_display(); //Adafruit logo is visible
	ssd1306_clearDisplay();
	delay(5000);

	char* text = "This is demo for SSD1306 i2c driver for Raspberry Pi";
	ssd1306_drawString(text);
	ssd1306_display();
	delay(5000);

	ssd1306_dim(1);
	ssd1306_startscrollright(00,0xFF);
	delay(5000);

	ssd1306_clearDisplay();
	ssd1306_fillRect(10,10, 50, 20, WHITE);
	ssd1306_fillRect(80, 10, 130, 50, WHITE);
	ssd1306_display();
}

