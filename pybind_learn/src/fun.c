#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

typedef struct
{
    char name[50];
    int age;
} Person;

Person him;
int ChangePerson(Person *p, const char *name, int age, int ret[5][10])
{
    strcpy(p->name, name);
    p->age = age;
    return 0;
}
void _ChangeAge()
{
    int ret[5][10];
    int time = 0;
    while (time < 20)
    {
        ChangePerson(&him, "xiaozhen", time, ret);
        time += 1;
        sleep(1);
    }
}

int StartPersonServer(int ret[10])
{
    pthread_t thrd;
    pthread_create(&thrd, NULL, (void *(*)(void *))_ChangeAge, NULL);
    printf("Start age changing\n");
    return 0;
}

int GetAge()
{
    return him.age;
}

int _add(int i, int j, ...)
{
    va_list vaptr;
    va_start(vaptr, j);
    int m = va_arg(vaptr, int);
    int sum = i + j + m;
    va_end(vaptr);
    printf("%i\n", sum);
    return sum;
}

int add(int m)
{
    int sum = _add(12, 1, 2);
    printf("sum is %i\n", sum);
    return sum;
}