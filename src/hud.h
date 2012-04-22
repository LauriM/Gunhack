#ifndef HUD_H
#define HUD_H

//TODO: rename to hudPrompt
int hudPrompt(char text[250]);
void hudMessage(char text[250]);
void hudRender();

int hudMenu(const char* title,const char** options,const char* prompt);
#endif
