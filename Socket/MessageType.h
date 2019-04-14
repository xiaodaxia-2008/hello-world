#include <stdarg.h>

#define MAX_AXIS_NUM 10

typedef enum
{
    MsgTypeRobotStates = 13,
    MsgTypeTrajPtFull = 14,
    MsgTypeJointFeedback = 15,
    MsgTypeTrajPtPos = 16,
    MsgTypeMotionCtrl = 2001,
} MsgType;

typedef enum
{
    StartTrajMode = 200121,
    StopTrajMode = 200122,
    StopMotion = 200111,
} MotionCommand;

// Define message prefix, including length of the message
struct _MsgPrefix
{
    int len;
} __attribute((__packed__));
typedef struct _MsgPrefix MsgPrefix;

// Define message header, including message type (robot status, streaming control type, realtime position control type, I/O write/read type etc.)
struct _MsgHeader
{
    MsgType type;
    int reserved1; // for future use
    int reserved2; // for futrue use
} __attribute__((__packed__));
typedef struct _MsgHeader MsgHeader;

// Define robot states message
struct _MsgRobotStates
{
    int drivers_powered;
    int e_stopped;
    int error_code;
    int in_error;
    int in_motion;
    int mode;
    int motion_possible;
} __attribute__((__packed__));
typedef struct _MsgRobotStates MsgRobotStates;

struct _MsgJointFeedback
{
    int robot_id;
    int valid_fields;
    float timestamp;
    float pos[MAX_AXIS_NUM];
    float vel[MAX_AXIS_NUM];
    float acc[MAX_AXIS_NUM];
} __attribute__((__packed__));
typedef struct _MsgJointFeedback MsgJointFeedback;

struct _MsgMotionCtrl
{
    int robot_id;
    int seq;
    int command;
    int data[10]; // for future use
} __attribute__((__packed__));
typedef struct _MsgMotionCtrl MsgMotionCtrl;

// Define trajectory point message with sequence, timestamp, positions, velocities, accelerations
struct _MsgTrajPtFull
{
    int robot_id; // default 0
    int seq;
    int valid_fields;
    float timestamp;
    float pos[MAX_AXIS_NUM];
    float vel[MAX_AXIS_NUM];
    float acc[MAX_AXIS_NUM];
} __attribute((__packed__));
typedef struct _MsgTrajPtFull MsgTrajPtFull;

// Define trajectory point message only with sequence and position
struct _MsgTrajPtPos
{
    int seq;
    float pos[MAX_AXIS_NUM];
} __attribute((__packed__));
typedef struct _MsgTrajPtPos MsgTrajPtPos;

typedef union {
    MsgRobotStates robot_states;
    MsgTrajPtFull traj_point_full;
    MsgTrajPtPos traj_point_pos;
    MsgMotionCtrl motion_ctrl;
    MsgJointFeedback joint_feedback;
} MsgBody;

struct _Msg
{
    MsgPrefix prefix;
    MsgHeader header;
    MsgBody body;
} __attribute__((__packed__));
typedef struct _Msg Msg;

int BuildMsg(Msg *msg, int type, ...)
{
    float *fptr;
    memset(msg, 0x00, sizeof(Msg));
    msg->header.type = type;
    va_list vaptr;
    va_start(vaptr, type);

    switch (type)
    {
    case MsgTypeTrajPtFull:
        msg->prefix.len = sizeof(MsgTrajPtFull) + sizeof(MsgHeader);
        msg->body.traj_point_full.seq = va_arg(vaptr, int);
        msg->body.traj_point_full.valid_fields = 1 | 2 | 4 | 8;
        msg->body.traj_point_full.timestamp = *va_arg(vaptr, float *);
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.traj_point_full.pos[i] = fptr[i];
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.traj_point_full.vel[i] = fptr[i];
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.traj_point_full.acc[i] = fptr[i];
        break;
    case MsgTypeTrajPtPos:
        msg->prefix.len = sizeof(MsgTrajPtPos) + sizeof(MsgHeader);
        msg->body.traj_point_pos.seq = va_arg(vaptr, int);
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.traj_point_pos.pos[i] = fptr[i];
        break;

    case MsgTypeMotionCtrl:
        msg->prefix.len = sizeof(MsgMotionCtrl) + sizeof(MsgHeader);
        msg->body.motion_ctrl.robot_id = va_arg(vaptr, int);
        msg->body.motion_ctrl.seq = va_arg(vaptr, int);
        msg->body.motion_ctrl.command = va_arg(vaptr, int);
        break;
    case MsgTypeJointFeedback:
        msg->prefix.len = sizeof(MsgJointFeedback) + sizeof(MsgHeader);
        msg->body.joint_feedback.robot_id = va_arg(vaptr, int);
        msg->body.joint_feedback.valid_fields = va_arg(vaptr, int);
        msg->body.joint_feedback.timestamp = *va_arg(vaptr, float *);
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.joint_feedback.pos[i] = fptr[i];
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.joint_feedback.vel[i] = fptr[i];
        fptr = va_arg(vaptr, float *);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            msg->body.joint_feedback.acc[i] = fptr[i];
        break;
    case MsgTypeRobotStates:
        msg->prefix.len = sizeof(MsgRobotStates) + sizeof(MsgHeader);
        msg->body.robot_states.drivers_powered = va_arg(vaptr, int);
        msg->body.robot_states.e_stopped = va_arg(vaptr, int);
        msg->body.robot_states.error_code = va_arg(vaptr, int);
        msg->body.robot_states.in_error = va_arg(vaptr, int);
        msg->body.robot_states.in_motion = va_arg(vaptr, int);
        msg->body.robot_states.mode = va_arg(vaptr, int);
        msg->body.robot_states.motion_possible = va_arg(vaptr, int);
        break;
    default:
        printf("Error: unknow message type (%i)\n", type);
        break;
    }
    va_end(vaptr);
    return 0;
}

void ShowMsg(Msg *msg)
{
    printf("\n==========================\n");
    switch (msg->header.type)
    {
    case MsgTypeRobotStates:
        printf("Robot states, type(%i), length(%i):\n", msg->header.type, msg->prefix.len);
        printf("Drivers' powered: %i\n", msg->body.robot_states.drivers_powered);
        break;
    case MsgTypeTrajPtFull:
        printf("Robot trajectory point full, type(%i), length(%i):\n", msg->header.type, msg->prefix.len);
        printf("sequence No.: %i\n", msg->body.traj_point_full.seq);
        printf("valid fields: %i\n", msg->body.traj_point_full.valid_fields);
        printf("timestamp: %fs\n", msg->body.traj_point_full.timestamp);
        printf("positions:\n\t");
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.traj_point_full.pos[i]);
        printf("\nvelocities:\n\t");
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.traj_point_full.vel[i]);
        printf("\naccelerations:\n\t");
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.traj_point_full.acc[i]);
        break;
    case MsgTypeTrajPtPos:
        printf("Robot trajectory point position, type(%i), length(%i):\n", msg->header.type, msg->prefix.len);
        printf("sequence No.: %i\n", msg->body.traj_point_pos.seq);
        printf("positions:\n\t");
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.traj_point_pos.pos[i]);
        break;
    case MsgTypeJointFeedback:
        printf("Robot joint feedback, type(%i), length(%i):\n", msg->header.type, msg->prefix.len);
        printf("Timestamp: %f", msg->body.joint_feedback.timestamp);
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.joint_feedback.pos[i]);
        printf("\nvelocities:\n\t");
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.joint_feedback.vel[i]);
        printf("\naccelerations:\n\t");
        for (int i = 0; i < MAX_AXIS_NUM; i++)
            printf("%f  ", msg->body.joint_feedback.acc[i]);
        break;
    default:
        printf("Unknow message, type(%i), length(%i):\n", msg->header.type, msg->prefix.len);
        break;
    }
    printf("\n===========================\n");
}

int RecvMsg(int sock_fd, Msg *msg)
{
    int valread = 0;
    valread = read(sock_fd, msg, sizeof(msg->prefix));
    valread += read(sock_fd, &msg->header, sizeof(msg->prefix) + msg->prefix.len);
    return valread;
}