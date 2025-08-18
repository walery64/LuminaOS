/** *****************************************************************************
 *
 *  Copyright (C) 2025 Water.Gate Software.
 *
 *  License:      GNU GENERAL PUBLIC LICENSE
 *                Version 3, 29 June 2007
 *
 *  Project:      LuminaOS
 *  File:         task.c
 *  Create on:    18 Aug 2025 12:10:20
 *  Author:       Utkin W.K.
 *
 *  Description:
 *
 * **************************************************************************** */

/* Includes ------------------------------------------------------------------- */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <lumina_os.h>
#include <lumina_sprintf.h>

extern SYSTEM_VARIABLE_typedef          SYSTEM_VAR;

/** *****************************************************************************
 * TASK_CREATE
 * @brief  Функция создания задачи
 * @param  NameTask - имя задачи
 * @param  PriorityTask - приоритет задачи
 * @param  BonusTask - бонус приоритета процесса за итерацию
 * @param  PointerTask - указатель на задачу
 * @retval int32_t
 *         if (retval < 0) - ошибка создания задачи
 *         else        PID - идентификатор созданной задачи
 * @note
 *
 * **************************************************************************** */
int32_t task_create (
                            const char*             Name,           /* Имя задачи */
                            POINTER_TASK_typedef    pTask,          /* Указатель на задачу */
                            uint32_t                PID_Process,    /* Идентификатор задачи */
                            uint32_t                PriorityTask,   /* Приоритет задачи */
                            uint32_t                BonusTask,      /* Бонус приоритета процесса за итерацию */
                            uint32_t                SignalRUN,      /* Флаг запуска задачи */
                            uint32_t                SignalSTOP,     /* Флаг остановки задачи */
                            uint32_t                Hibernation,    /* Значение гибернации */
                            uint32_t                FlagEmpty       /* Флаг пустоты очереди */
                        )
{
    uint32_t    _return_code = 0;
    uint32_t    _length;

    /* Проверим на возможность создания задачи */
    /* Количество задач должно быть меньше максимального.
     * указатель должен быть ненулевой */
    if ((PID_Process < PID_END) && (pTask != NULL))
    {
        /** *****************************************************************************
         * Создаем задачу
         * **************************************************************************** */
        /* Увеличим счетчик зарегистрированных задач */
        SYSTEM_VAR.AMOUNT_PROCESS ++;
        /* Определим указатель на процедуру задачи */
        SYSTEM_VAR.TASK[PID_Process].POINTER_TASK = pTask;
        /* Определим приоритет задачи */
        SYSTEM_VAR.TASK[PID_Process].PRIORITY.STATIC = PriorityTask;
        /* Определим бонус приоритета процесса за итерацию */
        SYSTEM_VAR.TASK[PID_Process].PRIORITY.BONUS = BonusTask;
        /* Определим статус задачи */
        SYSTEM_VAR.TASK[PID_Process].SIGNAL.RUNNING = SignalRUN;
        SYSTEM_VAR.TASK[PID_Process].SIGNAL.STOPPED = SignalSTOP;
        SYSTEM_VAR.TASK[PID_Process].SIGNAL.HIBERNATION = Hibernation;
        /* В очередь запишем флаг пустоты как он определен при создании задачи */
        SYSTEM_VAR.TASK[PID_Process].INPUT_QUEUE.FLAG_EMPTY = FlagEmpty;

        /* Сбросим код ошибки задачи */
        SYSTEM_VAR.TASK[PID_Process].ERROR = 0;

        /* Запишем идентификатор задачи */
        SYSTEM_VAR.TASK[PID_Process].ID = PID_Process;

        /* Запишем имя задачи */
//        SYSTEM_VAR.TASK[PID_Process].NAME = Name;
        _length = strlen(Name);
        strncpy(SYSTEM_VAR.TASK[PID_Process].NAME, Name, 16);
        if (_length >= 16)  {SYSTEM_VAR.TASK[PID_Process].NAME[16] = 0;}
        else                {SYSTEM_VAR.TASK[PID_Process].NAME[_length + 1] = 0;}
        /* Возвращаем PID созданной задачи */
        _return_code = PID_Process;
    }
    else
    {
        /** *****************************************************************************
         * Задача не может быть создана, возвращаем код ошибки
         * **************************************************************************** */
        _return_code = -1;
    }

    return _return_code;
}

/** *****************************************************************************
 * @brief   Функция перевода задачи в состояние остановки
 *
 * @param  delay - задержка в мсек
 * **************************************************************************** */
__attribute__((section(".ramfunc"), optimize("O0")))
void task_delay (uint32_t delay)
{
    /* Останавливаем задачу на "delay" мсек */
    SYSTEM_VAR.TASK[SYSTEM_VAR.CURRENT_PROCESS_ID].SIGNAL.HIBERNATION = delay;
}

/** *****************************************************************************
 * @brief   Функция простоя
 * **************************************************************************** */
__attribute__((section(".ramfunc"), optimize("O0")))
void idle (void)
{
//    static uint32_t _counter = 0;

//    if (_counter == 0)  {logging ("System START"); _counter ++;}

}



/** *****************************************************************************
 * *** END OF FILE «task.c» ***
 * **************************************************************************** */

