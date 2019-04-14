#include "MessageType.h"
#include <pthread.h>

Msg msg_states;

typedef struct
{
    float pos[MAX_AXIS_NUM];
    float vel[MAX_AXIS_NUM];
    float acc[MAX_AXIS_NUM];
} States;

void __StartStateServer()
{
    int robot_id = 0, valid_fields = 1 | 2 | 4;
    float timestamp = 0.0;
    float pos[MAX_AXIS_NUM] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float vel[MAX_AXIS_NUM] = {0.0}, acc[MAX_AXIS_NUM] = {0.0};
    while (timestamp < 30)
    {
        BuildMsg(&msg_states, MsgTypeJointFeedback, robot_id, valid_fields, &timestamp, pos, vel, acc);
        // usleep(2000); // 2ms
        // timestamp += 0.002;
        sleep(1);
        timestamp += 1.0;
    }
    printf("Stop updating states\n");
}

void StartStateServer()
{
    pthread_t thrd;
    pthread_create(&thrd, NULL, (void *(*)(void *))__StartStateServer, NULL);
    printf("Start state server...\n");
}

std::array<float, MAX_AXIS_NUM> GetPos()
{
    ShowMsg(&msg_states);
    return ConverToVector(msg_states.body.joint_feedback.pos);
}
