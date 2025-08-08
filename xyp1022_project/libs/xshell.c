/*
 * Copyright (c) 2020-2023, TaoTe Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author          Notes
 * 2022-03-17   wujing          first version
 */

#include "xshell.h"
#include "xprintf.h"

#ifdef USE_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif // #ifdef USE_OS_FREERTOS

#ifndef SHELL_PROMPT
#define SHELL_PROMPT "xsh"
#endif

#ifndef SHELL_DEFAULT_PROMPT
#define SHELL_DEFAULT_PROMPT "$"
#endif

static char maszShellPrompt[8] = {0};

static char maszShellLineHistory[SHELL_CMD_SIZE] = {0};

typedef struct SHELL_TAG
{
    char aszLine[SHELL_CMD_SIZE];
    enum
    {
        eWaitIdle = 0,
        eWaitSpecKey,
        eWaitFuncKey,
    } eKeyState;
    uint16_t ui16LinePosition;
    uint16_t ui16LineCurrentPosition;
} SHELL_T;

static char
ShellGetOneCharacter(void)
{
    char szData = -1;

    szData = (char)HW_UART_Read(UART);

    return szData;
}

/*************************************************************************
 * Shell_SetPrompt
 * @Description
 *   Set Shell prompt
 * @parameter
 *   pszPrompt: prompt string
 * @Return
 *   NULL
 **************************************************************************/
void Shell_SetPrompt(
    char *pszPrompt)
{
    if (strlen(pszPrompt) > (ARRAY_SIZE(maszShellPrompt) - 1)) {
        strcpy(maszShellPrompt, SHELL_DEFAULT_PROMPT);
        maszShellPrompt[strlen(SHELL_DEFAULT_PROMPT)] = 0;
    } else {
        strcpy(maszShellPrompt, pszPrompt);
        maszShellPrompt[strlen(pszPrompt)] = 0;
    }
}

/*************************************************************************
 * Shell_Init
 * @Description
 *   Init Shell when uart is connected and user input something
 *   Should output some shell prompt, like "sofw_shell>"
 * @parameter
 *   void
 * @Return
 *   NULL
 **************************************************************************/
void Shell_Init(void)
{
    Shell_SetPrompt(SHELL_PROMPT);
}

/*************************************************************************
 * Shell_GetCommand
 * @Description
 *   Get command string input by user
 * @parameter
 *   pszData: data buffer
 *   ui32Length: buffer length
 * @Return
 *   uint32_t: data length
 **************************************************************************/
uint32_t
Shell_GetCommand(
    char *pszData,
    uint32_t ui32Length)
{
    SHELL_T sShell;
    SHELL_T *psShell = &sShell;

    memset(&sShell, 0, sizeof(SHELL_T));

    if (maszShellPrompt[0] == 0) {
        Shell_SetPrompt(SHELL_PROMPT);
    }

    xprintf("%s > ", maszShellPrompt);

    while (1) {
        char szData = ShellGetOneCharacter();

        if (szData < 0) {
            continue;
        }

        if (szData == 0x1b) {
            /* handle control key */
            psShell->eKeyState = eWaitSpecKey;
            continue;
        } else if (psShell->eKeyState == eWaitSpecKey) {
            if (szData == 0x5b) {
                psShell->eKeyState = eWaitFuncKey;
                continue;
            } else {
                psShell->eKeyState = eWaitIdle;
            }
        } else if (psShell->eKeyState == eWaitFuncKey) {
            psShell->eKeyState = eWaitIdle;

            if (szData == 0x41) {
                /* up key */
                for (uint16_t i = 0; i < psShell->ui16LinePosition; i++) {
                    xprintf("\b \b");
                }
                strcpy(psShell->aszLine, maszShellLineHistory);
                psShell->ui16LinePosition = strlen(psShell->aszLine);
                psShell->ui16LineCurrentPosition = psShell->ui16LinePosition;
                xprintf("%s", psShell->aszLine);
                continue;
            } else if (szData == 0x42) {
                /* down key */
                for (uint16_t i = 0; i < psShell->ui16LinePosition; i++) {
                    xprintf("\b \b");
                }
                psShell->ui16LinePosition = 0;
                psShell->ui16LineCurrentPosition = psShell->ui16LinePosition;
                continue;
            } else if (szData == 0x43) {
                /* right key */
                if (psShell->ui16LineCurrentPosition < psShell->ui16LinePosition) {
                    xprintf("%c", psShell->aszLine[psShell->ui16LineCurrentPosition]);
                    psShell->ui16LineCurrentPosition++;
                }
                continue;
            } else if (szData == 0x44) {
                /* left key */
                if (psShell->ui16LineCurrentPosition) {
                    xprintf("\b");
                    psShell->ui16LineCurrentPosition--;
                }
                continue;
            }
        }

        if (szData == '\t') {
            /* handle tab key */
            if (psShell->ui16LinePosition > 0) {
                psShell->ui16LinePosition = Shell_AutoComplete(psShell->aszLine, psShell->ui16LinePosition);
                psShell->ui16LineCurrentPosition = psShell->ui16LinePosition;
                continue;
            } else {
                strcpy(psShell->aszLine, "help");
                psShell->ui16LinePosition = strlen(psShell->aszLine);
                psShell->aszLine[psShell->ui16LinePosition] = 0;
                psShell->ui16LineCurrentPosition = psShell->ui16LinePosition;
                break;
            }
        } else if (szData == 0x08 || szData == 0x7f) {
            /* handle backspace key */
            if (psShell->ui16LineCurrentPosition == 0) {
                continue;
            }

            psShell->ui16LinePosition--;
            psShell->ui16LineCurrentPosition--;

            if (psShell->ui16LineCurrentPosition < psShell->ui16LinePosition) {
                memmove(&psShell->aszLine[psShell->ui16LineCurrentPosition],
                        &psShell->aszLine[psShell->ui16LineCurrentPosition + 1],
                        psShell->ui16LinePosition - psShell->ui16LineCurrentPosition);
                psShell->aszLine[psShell->ui16LinePosition] = 0;

                xprintf("\b%s ", &psShell->aszLine[psShell->ui16LineCurrentPosition]);

                /* move the cursor to the origin ui32Position */
                for (size_t i = psShell->ui16LineCurrentPosition; i <= psShell->ui16LinePosition; i++) {
                    xprintf("\b");
                }
            } else {
                xprintf("\b \b");
                psShell->aszLine[psShell->ui16LinePosition] = 0;
            }

            continue;
        } else if (szData == '\r' || szData == '\n') {
            /* handle enter key */
            psShell->aszLine[psShell->ui16LinePosition] = 0;
            break;
        }

        /* normal character */
        if (psShell->ui16LineCurrentPosition < psShell->ui16LinePosition) {
            memmove(&psShell->aszLine[psShell->ui16LineCurrentPosition + 1],
                    &psShell->aszLine[psShell->ui16LineCurrentPosition],
                    psShell->ui16LinePosition - psShell->ui16LineCurrentPosition);
            psShell->aszLine[psShell->ui16LineCurrentPosition] = szData;
            xprintf("%s", &psShell->aszLine[psShell->ui16LineCurrentPosition]);

            /* move the cursor to new ui32Position */
            for (size_t i = psShell->ui16LineCurrentPosition; i < psShell->ui16LinePosition; i++) {
                xprintf("\b");
            }
        } else {
            psShell->aszLine[psShell->ui16LinePosition] = szData;
            xprintf("%c", szData);
        }

        psShell->ui16LinePosition++;
        psShell->ui16LineCurrentPosition++;
        if (psShell->ui16LinePosition >= SHELL_CMD_SIZE) {
            psShell->ui16LinePosition = 0;
            psShell->ui16LineCurrentPosition = 0;
        }
    }

    if (psShell->ui16LinePosition > 0) {
        strcpy(maszShellLineHistory, psShell->aszLine);
        maszShellLineHistory[psShell->ui16LinePosition] = 0;
    }

    xprintf("\n");

    uint32_t ui32CopyLength = 0;

    if (pszData != NULL) {
        ui32CopyLength = MIN(ui32Length - 1, psShell->ui16LinePosition);
        memcpy((void *)pszData, (void *)psShell->aszLine, ui32CopyLength);
    }

    return ui32CopyLength;
}

/*************************************************************************
 * Shell_SplitCommand
 * @Description
 *   Split shell command
 * @parameter
 *   pszData: data buffer
 *   ui32Length: buffer length
 *   apszArgv: argv after split
 * @Return
 *   uint32_t: argv number
 **************************************************************************/
uint32_t
Shell_SplitCommand(
    char *pszData,
    uint32_t ui32Length,
    char *apszArgv[])
{
    char *pszPtr = pszData;
    uint32_t ui32Position = 0;
    uint8_t ui8Argc = 0;

    while (ui32Position < ui32Length) {
        /* strip bank and tab */
        while ((*pszPtr == ' ' || *pszPtr == '\t') && (ui32Position < ui32Length)) {
            *pszPtr = '\0';
            pszPtr++;
            ui32Position++;
        }

        if (ui8Argc >= SHELL_ARG_MAX) {
            xprintf("Too many args ! We only Use:\n");
            for (size_t i = 0; i < ui8Argc; i++) {
                xprintf("%s ", apszArgv[i]);
            }
            xprintf("\n");
            break;
        }

        if (ui32Position >= ui32Length) {
            break;
        }

        /* handle string */
        if (*pszPtr == '"') {
            pszPtr++;
            ui32Position++;
            apszArgv[ui8Argc] = pszPtr;
            ui8Argc++;

            /* skip this string */
            while ((*pszPtr != '"') && (ui32Position < ui32Length)) {
                if (*pszPtr == '\\') {
                    if (*(pszPtr + 1) == '"') {
                        pszPtr++;
                        ui32Position++;
                    }
                }
                pszPtr++;
                ui32Position++;
            }
            if (ui32Position >= ui32Length) {
                break;
            }

            /* skip '"' */
            *pszPtr = '\0';
            pszPtr++;
            ui32Position++;
        } else {
            apszArgv[ui8Argc] = pszPtr;
            ui8Argc++;
            while ((*pszPtr != ' ' && *pszPtr != '\t') && (ui32Position < ui32Length)) {
                pszPtr++;
                ui32Position++;
            }
            if (ui32Position >= ui32Length) {
                break;
            }
        }
    }

    return ui8Argc;
}

/*************************************************************************
 * Shell_HandleCommand
 * @Description
 *   Handle command string input by user
 * @parameter
 *   pui8pszDataData: data buffer
 *   ui32Length: data length
 * @Return
 *   NULL
 **************************************************************************/
void Shell_HandleCommand(
    char *pszData,
    uint32_t ui32Length)
{
    uint8_t ui8Argc;
    char *apszArgv[SHELL_ARG_MAX];

    if (ui32Length < 1) {
        return;
    }

    ui8Argc = Shell_SplitCommand(pszData, ui32Length, apszArgv);

    for (size_t i = 0; i < gui32ShellCommandsNum; i++) {
        if (strcmp(apszArgv[0], gsShellCommands[i].pszCommand) == 0) {
#ifdef USE_OS_FREERTOS
            taskENTER_CRITICAL();
#endif
            gsShellCommands[i].pfnCommand(ui8Argc, apszArgv);
#ifdef USE_OS_FREERTOS
            taskEXIT_CRITICAL();
#endif
            return;
        }
    }

    xprintf("Unknown command!\n");
}

/*************************************************************************
 * Shell_AutoComplete
 * @Description
 *   Auto complete command
 * @parameter
 *   pszPrefix: prefix string
 *   ui32Length: prefix string length
 * @Return
 *   uint32_t: completed command string length
 **************************************************************************/
__attribute__((weak))
uint32_t
Shell_AutoComplete(
    char *pszPrefix,
    uint32_t ui32Length)
{
    char *pszCmd = NULL;

    if (ui32Length == 0) {
        return ui32Length;
    }

    for (size_t i = 0; i < gui32ShellCommandsNum; i++) {
        if (strncmp(pszPrefix, gsShellCommands[i].pszCommand, ui32Length) == 0) {
            pszCmd = gsShellCommands[i].pszCommand;
            strcpy(&pszPrefix[ui32Length], &pszCmd[ui32Length]);
            xprintf("%s ", &pszCmd[ui32Length]);
            ui32Length = strlen(pszCmd); // add space character in the tail
            pszPrefix[ui32Length] = ' ';
            ui32Length++;
            break;
        }
    }

    return ui32Length;
}
