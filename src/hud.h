#ifndef HUD_H
#define HUD_H

int hudPrompt(char text[250]);
void hudRender();

int hudMenu(const char* title,char** options,const char* prompt);

void hudMenuInit();
void hudMenuWrite(char *line);
int hudMenuFinish();
#endif
