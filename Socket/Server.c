// Server side C/C++ program to demonstrate Socket programming
#include "Network.h"
#include "MessageType.h"
#include <math.h>
#include <pthread.h>

#define MOTIONPORT 50240
#define STATEPORT 50241

typedef struct
{
    Msg msg;
    uint8_t updated;
} MotionDataMsg;

void StartStateServer();
void StartMotionServer(MotionDataMsg *args);

int main()
{
    int valread, valsent;
    MotionDataMsg motiondata;
    memset(&motiondata, 0x00, sizeof(MotionDataMsg));

    pthread_t thrd_state, thrd_motion;

    pthread_create(&thrd_state, NULL, (void *)StartStateServer, NULL);
    printf("starting state socket server...\n");
    pthread_create(&thrd_motion, NULL, (void *)StartMotionServer, &motiondata);
    printf("starting motion socket server...\n");

    // if the send frequency is very high, some messages will be skipped.
    while (1)
    {
        if (motiondata.updated)
        {
            ShowMsg(&motiondata.msg);
            motiondata.updated = 0;
        }
        // usleep(10); // 1ms
    }

    // int seq = 111;
    // float pos[MAX_AXIS_NUM] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    // BuildMsg(&msg, MsgTypeTrajPtPos, seq, pos);
    // ShowMsg(&msg);

    // valsent = send(motion_client, &msg, msg.prefix.len + sizeof(msg.prefix.len), 0);
    // printf("sent bytes: %i\n", valsent);

    pthread_join(thrd_state, NULL);
    pthread_join(thrd_motion, NULL);
    printf("Exited\n");
    return 0;
}

void StartStateServer()
{
    Msg msg_states;

    int state_server, state_client;
    int bytesent;
    int robot_id = 0, valid_fields = 1 | 2 | 4;
    float timestamp = 0.0;
    float pos[MAX_AXIS_NUM] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float vel[MAX_AXIS_NUM] = {0.0}, acc[MAX_AXIS_NUM] = {0.0};
    state_server = SocketServer(STATEPORT);
    state_client = Accept(state_server);
    printf("Accepted state observing socket...\n");
    while (1)
    {
        BuildMsg(&msg_states, MsgTypeRobotStates, 1, 0, 0, 0, 0, 0, 1);
        bytesent = send(state_client, &msg_states, msg_states.prefix.len + sizeof(msg_states.prefix), 0);
        if (bytesent < 0)
            break;
        BuildMsg(&msg_states, MsgTypeJointFeedback, robot_id, valid_fields, &timestamp, pos, vel, acc);
        bytesent = send(state_client, &msg_states, msg_states.prefix.len + sizeof(msg_states.prefix), 0);
        if (bytesent < 0)
            break;
        usleep(2000);
        timestamp += 0.002;
    }
    close(state_client);
    close(state_server);
}

void StartMotionServer(MotionDataMsg *args)
{
    int valread;
    int motion_server, motion_client;
    Msg msg_temp;
    motion_server = SocketServer(MOTIONPORT);
    motion_client = Accept(motion_server);
    printf("Accepted motion socket...\n");
    while (1)
    {
        valread = RecvMsg(motion_client, &msg_temp);
        if (valread > 0)
        {
            // printf("received bytes: %i\n", valread);
            // ShowMsg(&msg);
            memcpy(&args->msg, &msg_temp, sizeof(Msg));
            args->updated = 1;
            // ShowMsg(&msg_temp);
            valread = 0;
        }
    }

    close(motion_client);
    close(motion_server);
}