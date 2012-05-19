#ifndef HUD_H
#define HUD_H

#define hudMenuWriteF(p_msg,...) {char msg[100]; snprintf(msg,100,p_msg,__VA_ARGS__);hudMenuWrite(msg);}

int hudPrompt(char text[250]);
void hudRender();

int hudMenu(const char* title,char** options,const char* prompt);

void hudMenuInit();
void hudMenuWrite(char *line);
int hudMenuFinish();
#endif
