#include <sys/shell.h>
#include <lib/libc.h>
#include <lib/printf.h>

int about_cmd();
int prompt_cmd();
int credits_cmd();

char shell_prompt[256];
char shell_input[256];
int shell_ret_val = 0;

int shell_cmd_no = 3;

char *shell_cmds[] = {
    "about",
    "prompt",
    "credits"
};

int *shell_cmd_handlers[] = {
    about_cmd,
    prompt_cmd,
    credits_cmd
};

int about_cmd() {
    printf("tangoOS is a 32-bit x86 operating system written in C.\nCreated by winksplorer & xrc2. Licensed under MPL-2.0.\n");
    return 0;
}

int prompt_cmd() {
    int len = strlen(shell_input + 7);
    memcpy(shell_prompt, shell_input + 7, len);
    shell_prompt[len] = 0;
    
    return 0;
}

int credits_cmd() {
    printf("\033[32m                                -- CREDITS --                                 \n\n");
    printf("\033[93m               ____  \n__  ___ __ ___|___ \\  Role: tangoOS owner\n\\ \\/ / '__/ __| __) | Other projects: Medlar64, WebWatcher, 9xVM, SVGAIITerminal >  <| | | (__ / __/  About: Just an average coder that codes stuff.\n/_/\\_\\_|  \\___|_____| Links: www.mobren.net\n\n");
    printf("\033[94m          _       _    \n__      _(_)_ __ | | __ Role: tangoOS Collaborator\n\\ \\ /\\ / / | '_ \\| |/ / Other projects: malino, moss, yamjo, tinyutils, 2x\n \\ V  V /| | | | |   <  About: Not a rust programmer.\n  \\_/\\_/ |_|_| |_|_|\\_\\ Links: www.winksplorer.net\n\n\033[0m");
    return 0;
}

/*
 * shell_loop - the main loop of the shell
 */
void shell_loop(void) {
    printf(shell_prompt);
    kbd_ngets(shell_input, 256);

    if (!strcmp(shell_input, "")) {
        return;
    }

    for (int i = 0; i < shell_cmd_no; i++) {
        if (!strncmp(shell_input, shell_cmds[i], strlen(shell_cmds[i]))) {
            int(*handler)();

            handler = shell_cmd_handlers[i];

            if (shell_cmd_handlers[i] != NULL) {
                shell_ret_val = handler();
            } else {
                shell_ret_val = 1;
            }

            return;
        }
    }

    printf("\033[31m%s: command not found\033[0m\n", shell_input);

    //if (!strcmp(shell_input, "")) {
    //} else if (!strncmp(shell_input, "about", 5)) {
    //} else if (!strncmp(shell_input, "prompt", 6)) {
        
    //} else {
    //}
}

/*
 * shell_entry - entry point for the shell
 */
void shell_entry(void) {
    char defaultPrompt[] = "\033[91m#\033[0m ";
    memcpy(shell_prompt, defaultPrompt, sizeof(defaultPrompt));
    for (;;) { 
        shell_loop();
    }
}

// gonna make the credits things here
/*

               ____  \n__  ___ __ ___|___ \\  Role: tangoOS owner\n\ \\/ / '__/ __| __) | Other projects: Medlar64, WebWatcher, 9xVM, SVGAIITerminal\n >  <| | | (__ / __/  About: Just an average coder that codes stuff.\n/_/\\_\\_|  \\___|_____| Links: www.mobren.net\n

          _       _    \n__      _(_)_ __ | | __ Role: tangoOS Collaborator\n\\ \\ /\\ / / | '_ \\| |/ / Other projects: malino, moss, yamjo, tinyutils, 2x\n \\ V  V /| | | | |   <  About: Not a rust programmer.\n  \\_/\\_/ |_|_| |_|_|\\_\\ Links: www.winksplorer.net\n
*/