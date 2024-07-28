#include <sys/shell.h>
#include <lib/libc.h>
#include <lib/printf.h>
#include <lib/string.h>
#include <sys/version.h>
#include <drivers/rtc.h>
#include <drivers/vga.h>

int about_cmd();
int prompt_cmd();
int credits_cmd();
int help_cmd();
int uname_cmd();
int echo_cmd();
int date_cmd();
int clear_cmd();

char shell_prompt[256];
char shell_input[256];

int shell_cmd_no = 8;

char *shell_cmds[] = {
    "about",
    "prompt",
    "credits",
    "help",
    "uname",
    "echo",
    "date",
    "clear"
};

char *shell_cmd_descriptions[] = {
    "Shows info about tangoOS",
    "Changes the prompt of the shell",
    "ASCII names of the people that created tangoOS",
    "Shows this screen",
    "Prints system information",
    "Echoes back the arguments (akin to the voices in your head)",
    "Prints the RTC date",
    "Clears the screen"
};

int *shell_cmd_handlers[] = {
    about_cmd,
    prompt_cmd,
    credits_cmd,
    help_cmd,
    uname_cmd,
    echo_cmd,
    date_cmd,
    clear_cmd
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

int help_cmd() {
    printf("Available commands:\n");
    for (int i = 0; i < shell_cmd_no; i++) {
        printf("%s - %s\n", shell_cmds[i], shell_cmd_descriptions[i]);
    }
    return 0;
}

int uname_cmd() {
    if (strlen(shell_input) == 5) {
        printf("%s\n", __kernel_name);
    } else if (shell_input[6] == '-') {
        switch (shell_input[7]) {
            case 'a':
                printf("%s tango %d.%d-%s %d.%d (%s %s) %s tangoOS\n", __kernel_name, __kernel_version_major, __kernel_version_minor, __kernel_arch, __kernel_version_major, __kernel_version_minor, __kernel_build_date, __kernel_build_time, __kernel_arch);
                break;
            case 's':
                printf("%s\n", __kernel_name);
                break;
            case 'n':
                printf("tango\n");
                break;
            case 'r':
                printf("%d.%d-%s\n", __kernel_version_major, __kernel_version_minor, __kernel_arch);
                break;
            case 'v':
                printf("%d.%d (%s %s)\n", __kernel_version_major, __kernel_version_minor, __kernel_build_date, __kernel_build_time);
                break;
            case 'm':
                printf("%s\n", __kernel_arch);
                break;
            case 'p':
                printf("unknown\n");
                break;
            case 'i':
                printf("unknown\n");
                break;
            case 'o':
                printf("tangoOS\n");
                break;
            case 'h':
                printf("Usage: uname [OPTION]...\nPrint certain system information. With no OPTION, same as -s.\n\n -a\tprint all information, in the following order:\n -s\tprint the kernel name\n -n\tprint the network node hostname\n -r\tprint the kernel release\n -v\tprint the kernel version\n -m\tprint the machine hardware name\n -p\tprint the processor type (non-portable)\n -i\tprint the hardware platform (non-portable)\n -o\tprint the operating system\n -h\tdisplay this help and exit\n");
                break;
            default:
                printf("uname: invalid option\n");
                return 1;
        }
    } else {
        printf("uname: invalid option\n");
        return 1;
    }
    return 0;
}

int echo_cmd() {
    printf("%s\n", shell_input + 5);
    return 0;
}

int date_cmd() {
    if (strlen(shell_input) == 4) {
        printf("%s %s %d %d:%d:%d %d%d\n", rtc_days[rtc_day_of_week()], rtc_months[rtc_month() - 1], rtc_day(), rtc_hour(), rtc_minute(), rtc_second(), rtc_century(), rtc_year());
    } else if (shell_input[5] == '-') {
        switch (shell_input[6]) {
            case 'a':
                printf("%s %s %d %d:%d:%d %d%d\n", rtc_days[rtc_day_of_week()], rtc_months[rtc_month() - 1], rtc_day(), rtc_hour(), rtc_minute(), rtc_second(), rtc_century(), rtc_year());
                break;
            case 'D':
                printf("%s\n", rtc_days[rtc_day_of_week()]);
                break;
            case 'M':
                printf("%s\n", rtc_months[rtc_month() - 1]);
                break;
            case 'd':
                printf("%d\n", rtc_day());
                break;
            case 'H':
                printf("%d\n", rtc_hour());
                break;
            case 'm':
                printf("%d\n", rtc_minute());
                break;
            case 's':
                printf("%d\n", rtc_second());
                break;
            case 'c':
                printf("%d\n", rtc_century());
                break;
            case 'y':
                printf("%d\n", rtc_year());
                break;
            case 'h':
                printf("Usage: date [OPTION]...\nPrint certain parts of the RTC time. With no OPTION, same as -a.\n\n -a\tprint all information, in the following order:\n -D\tprint the day of the week\n -M\tprint the month name\n -d\tprint the day of the month\n -H\tprint the hour\n -m\tprint the minute\n -s\tprint the second\n -y\tprint the year\n -c\tprint the century\n -h\tdisplay this help and exit\n");
                break;
            default:
                printf("date: invalid option\n");
                return 1;
        }
    } else {
        printf("date: invalid option\n");
        return 1;
    }
    return 0;
}

int clear_cmd() {
    vga_clear();
    return 0;
}

/*
 * shell_loop - the main loop of the shell
 */
int shell_loop(int val) {
    if (val != 0) {
        printf("\033[91m%d ", val);
    }
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
                return handler();
            } else {
                return 1;
            }
        }
    }

    printf("\033[91m%s: command not found\033[0m\n", shell_input);
    return 1;
}

/*
 * shell_entry - entry point for the shell
 */
void shell_entry(void) {
    char defaultPrompt[] = "\033[91m#\033[0m ";
    memcpy(shell_prompt, defaultPrompt, sizeof(defaultPrompt));
    
    int ret = 0;
    for (;;) { 
        ret = shell_loop(ret);
    }
}