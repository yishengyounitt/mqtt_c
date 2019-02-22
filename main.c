#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "mqtt.h"
#include "mqtt_wrapper.h"
#ifdef __cplusplus
}
#endif // __cplusplus

#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>
#include <memory.h>


int sock_send(struct mqtt_context_t *ctx, const struct iovec *iov, int iovcnt){
    return writev(ctx->_fd,iov,iovcnt);
}

int main() {
    mqtt_context context;
    mqtt_init_contex(&context,sock_send);
    mqtt_connect_host(&context,"10.0.9.56",1883,3);
    mqtt_send_connect_pkt(&context,10,"JIMIMAX",1,"/Service/JIMIMAX/will","willPayload",0,MQTT_QOS_LEVEL1, 1,"admin","public");

    char buffer[4];
    while (1){
        int recv = read(context._fd,buffer, sizeof(buffer));
        if(recv == 0){
            LOGE("read eof\r\n");
            break;
        }
        if(recv == -1){
            LOGE("read interupted :%d %s\r\n",errno,strerror(errno));
            continue;
        }
        mqtt_input_data(&context,buffer,recv);
    }

    mqtt_release_contex(&context);
    return 0;
}