/** @file
  * Shell I/F defintion.

  * This header file is used to define I/F to Shell

  * Copyright (c) 2024 Shanghai Zijing Xinjie Intelligent Technology Co., Ltd.
  * All rights reserved.

  */

#ifndef SHELL_H
#define SHELL_H

#include "XYP1022.h"

#ifndef SHELL_CMD_SIZE
#define SHELL_CMD_SIZE (128)
#endif

#ifndef SHELL_ARG_MAX
#define SHELL_ARG_MAX (8)
#endif

/*
 * Define shell command here
 */
typedef int (*PFN_Command_Handler)(uint8_t, char **);
typedef struct SHELL_COMMAND_TAG
{
    char *pszCommand;
    PFN_Command_Handler pfnCommand;
    char *pszHelpStr;
} SHELL_COMMAND_T;

extern const SHELL_COMMAND_T gsShellCommands[];
extern const uint32_t gui32ShellCommandsNum;

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
    char *pszPrompt);

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
void Shell_Init(void);

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
    uint32_t ui32Length);

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
    char *apszArgv[]);

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
    uint32_t ui32Length);

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
uint32_t
Shell_AutoComplete(
    char *pszPrefix,
    uint32_t ui32Length);

#endif /* SHELL_H */
