// Client side C/C++ program to demonstrate Socket programming

#include "Network.h"
#include "MessageType.h"
#include <pthread.h> // receive robot state
#include <semaphore.h>
#include <math.h> // add -lm, when compliling

#define pow powf
#define MOTIONPORT 50240
#define STATEPORT 50241

typedef struct
{
    int sock;
    Msg *msg;
} StateArgs;
void InterpQuintic(float ret[][MAX_AXIS_NUM], float t, float T, float p0_pos[], float p1_pos[], float p0_vel[], float p1_vel[], float p0_acc[], float p1_acc[]);
void ObserveRobotState(StateArgs *args);

int Client()
{
    int sock_motion, sock_state;
    int valsent, valread;

    // char *ip_robot = "192.168.0.103";
    const char *ip_robot = "127.0.0.1";

    // start state observe socket
    Msg msg_state;
    if ((sock_state = SocketConnect(ip_robot, STATEPORT)) < 0)
        exit(EXIT_FAILURE);
    pthread_t thrd_state;
    StateArgs args = {sock_state, &msg_state};
    if ((pthread_create(&thrd_state, NULL, (void *(*)(void *))ObserveRobotState, (void *)&args)) != 0)
        exit(EXIT_FAILURE);

    // start motion socket
    if ((sock_motion = SocketConnect(ip_robot, MOTIONPORT)) < 0)
        exit(EXIT_FAILURE);

    Msg msg;
    int seq = 0;
    float timestamp = 0.0, duration = 2.0;
    float pos[MAX_AXIS_NUM] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0};
    float arr_zero[MAX_AXIS_NUM] = {0.0};
    float ret[3][MAX_AXIS_NUM] = {0.0};
    while (timestamp <= duration)
    {
        InterpQuintic(ret, timestamp, duration, arr_zero, pos, arr_zero, arr_zero, arr_zero, arr_zero);
        BuildMsg(&msg, MsgTypeTrajPtFull, seq, &timestamp, ret[0], ret[1], ret[2]);
        valsent = send(sock_motion, &msg, msg.prefix.len + sizeof(msg.prefix), 0);
        ShowMsg(&msg);
        timestamp += 0.1;
        seq += 1;
        usleep(100); // 0.1ms
    }

    // Msg msgrecv;
    // valread = RecvMsg(sock_motion, &msgrecv);
    // ShowMsg(&msgrecv);
    pthread_join(thrd_state, NULL);
    close(sock_motion);
    return 0;
}

void InterpQuintic(float ret[][MAX_AXIS_NUM], float t, float T, float p0_pos[], float p1_pos[], float p0_vel[], float p1_vel[], float p0_acc[], float p1_acc[])
{
    float h;
    for (int i = 0; i < MAX_AXIS_NUM; ++i)
    {
        h = p1_pos[i] - p0_pos[i];
        float k0 = p0_pos[i];
        float k1 = p0_vel[i];
        float k2 = p0_acc[i] / 2.0;
        float k3 = (20 * h - (8 * p1_vel[i] + 12 * p0_vel[i]) * T - (3 * p0_acc[i] - p1_acc[i]) * pow(T, 2)) / (2 * pow(T, 3));
        float k4 = (-30 * h + (14 * p1_vel[i] + 16 * p0_vel[i]) * T + (3 * p0_acc[i] - 2 * p1_acc[i]) * pow(T, 2)) / (2 * pow(T, 4));
        float k5 = (12 * h - 6 * (p1_vel[i] + p0_vel[i]) * T + (p1_acc[i] - p0_acc[i]) * pow(T, 2)) / (2 * pow(T, 5));
        ret[0][i] = (k5 * pow(t, 5) + k4 * pow(t, 4) + k3 * pow(t, 3) + k2 * pow(t, 2) + k1 * t + k0); // positions
        ret[1][i] = (5 * k5 * pow(t, 4) + 4 * k4 * pow(t, 3) + 3 * k3 * pow(t, 2) + 2 * k2 * t + k1);  // velocities
        ret[2][i] = (20 * k5 * pow(t, 3) + 12 * k4 * pow(t, 2) + 6 * k3 * t + 2 * k2);                 // accelerations
    }
}

void ObserveRobotState(StateArgs *args)
{
    int sock_state = args->sock;
    Msg *msg = args->msg;
    Msg msg_joint;
    while (1)
    {
        // TODO: should use mutex or semaphore here
        RecvMsg(sock_state, &msg_joint);
        if (msg_joint.header.type == MsgTypeJointFeedback)
        {
            memcpy(msg, &msg_joint, sizeof(Msg));
            ShowMsg(msg);
        }
        // usleep(1000); // 2ms
    }
}