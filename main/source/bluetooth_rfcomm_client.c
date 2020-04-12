#include "ssd1306_i2c.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
int main(int argc, char **argv)
{
    struct sockaddr_rc addr = {0};
    int s, status;
    //allocate a socket
    s = socket(AF_BLUETOOTH, sock_stream, BTPROTO_RFCOMM);
    //set the connection parameters(who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    str2ba(dest, &addr.rc_bdaddr);
    //connect to source(in this case mobile)
    status= connect(s, (struct sockaddr *)&addr, sizeof(addr));
    //send a message
    if(status == 0)
                  {
                  status = write(s, "hello!", 6);
                  }
    if( status < 0)
                  {
                  error("oh oh");
                  }
    close(s);
    return 0 ;
}
