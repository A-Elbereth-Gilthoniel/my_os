#include "macros.h"
#include "terminal.h"
#include "commands.h"
#include "heap.h"
#include "multithreads.h"
#include <string.h>


extern terminal* monitor;

#define DEF_CMD(name, code) \
if (strcmp(word, name) == 0)         \
{                                    \
    code                             \
}

void command_handler(void)
{
    while (1)
    {
        if (strlen(monitor->command) != 0)
        {
            char* cmd = monitor->command;
            char* word = get_next_word(&cmd);
#include "cmd.h"
            monitor->command[0] = '\0';
        }
        schedule();
    }
}

//============================================

char* get_next_word(char** text)
{
    if (!*text)
        return NULL;

    while (**text == ' ')
        (*text)++;

    if (**text == '\0')
        return NULL;

    char* counter = *text;
    while (*counter != ' ' && *counter != '\0') counter++;

    size_t len = counter - *text;
    char* word = alloc(len + 1);

    memcpy(word, *text, len);
    word[len] = '\0';

    *text = *counter ? counter + 1 : counter;;
    return word;
}

#undef DEF_CMD
