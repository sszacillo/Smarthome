#ifndef TASKS_H
#define TASKS_H

void taskReadSensors(void *parameter);
void taskHandleLEDs(void *parameter);
void taskMQTTLoop(void *parameter);

#endif
