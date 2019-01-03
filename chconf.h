// Step10V - STEP/DIR to analog amplifier CNC regulator
// Copyright (C) 2018-2019 Charles-Henri Mousset - ch.mousset@gmail.com
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#ifndef CHCONF_H
#define CHCONF_H

#define _CHIBIOS_RT_CONF_
#define _CHIBIOS_RT_CONF_VER_5_0_

#define CHPRINTF_USE_FLOAT          TRUE

#define SHELL_USE_HISTORY           TRUE
#define SHELL_USE_COMPLETION        TRUE
#define SHELL_USE_ESC_SEQ           TRUE
#define SHELL_MAX_ARGUMENTS         12

#define CH_CFG_ST_RESOLUTION                32
#define CH_CFG_ST_FREQUENCY                 10000
#define CH_CFG_INTERVALS_SIZE               32
#define CH_CFG_TIME_TYPES_SIZE              32
#define CH_CFG_ST_TIMEDELTA                 0       // We use the system tick

#define CH_CFG_TIME_QUANTUM                 0       // No round-robin
#define CH_CFG_MEMCORE_SIZE                 0

#define CH_CFG_NO_IDLE_THREAD               FALSE   // main does that

#define CH_CFG_OPTIMIZE_SPEED               TRUE

// Subsystems
#define CH_CFG_USE_TM                       TRUE
#define CH_CFG_USE_REGISTRY                 TRUE
#define CH_CFG_USE_WAITEXIT                 TRUE
#define CH_CFG_USE_SEMAPHORES               TRUE
#define CH_CFG_USE_SEMAPHORES_PRIORITY      FALSE
#define CH_CFG_USE_MUTEXES                  TRUE
#define CH_CFG_USE_MUTEXES_RECURSIVE        FALSE
#define CH_CFG_USE_CONDVARS                 TRUE
#define CH_CFG_USE_CONDVARS_TIMEOUT         TRUE
#define CH_CFG_USE_EVENTS                   TRUE
#define CH_CFG_USE_EVENTS_TIMEOUT           TRUE
#define CH_CFG_USE_MESSAGES                 TRUE
#define CH_CFG_USE_MESSAGES_PRIORITY        FALSE
#define CH_CFG_USE_MAILBOXES                TRUE
#define CH_CFG_USE_MEMCORE                  TRUE
#define CH_CFG_USE_HEAP                     TRUE
#define CH_CFG_USE_MEMPOOLS                 TRUE
#define CH_CFG_USE_OBJ_FIFOS                TRUE
#define CH_CFG_USE_DYNAMIC                  TRUE

#define CH_CFG_USE_FACTORY                  TRUE
#define CH_CFG_FACTORY_MAX_NAMES_LENGTH     8
#define CH_CFG_FACTORY_OBJECTS_REGISTRY     TRUE
#define CH_CFG_FACTORY_GENERIC_BUFFERS      TRUE
#define CH_CFG_FACTORY_SEMAPHORES           TRUE
#define CH_CFG_FACTORY_MAILBOXES            TRUE
#define CH_CFG_FACTORY_OBJ_FIFOS            TRUE

#define CH_DBG_STATISTICS                   FALSE
#define CH_DBG_SYSTEM_STATE_CHECK           FALSE
#define CH_DBG_ENABLE_CHECKS                FALSE
#define CH_DBG_ENABLE_ASSERTS               FALSE
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_DISABLED
#define CH_DBG_TRACE_BUFFER_SIZE            128
#define CH_DBG_ENABLE_STACK_CHECK           FALSE
#define CH_DBG_FILL_THREADS                 FALSE
#define CH_DBG_THREADS_PROFILING            FALSE

#define CH_CFG_SYSTEM_EXTRA_FIELDS
#define CH_CFG_SYSTEM_INIT_HOOK(tp) {}

#define CH_CFG_THREAD_EXTRA_FIELDS
#define CH_CFG_THREAD_INIT_HOOK(tp) {}

#define CH_CFG_THREAD_EXIT_HOOK(tp) {}

#define CH_CFG_CONTEXT_SWITCH_HOOK(ntp, otp) {}

#define CH_CFG_IRQ_PROLOGUE_HOOK() {}

#define CH_CFG_IRQ_EPILOGUE_HOOK() {}

#define CH_CFG_IDLE_ENTER_HOOK() {}

#define CH_CFG_IDLE_LEAVE_HOOK() {}

#define CH_CFG_IDLE_LOOP_HOOK() {}

#define CH_CFG_SYSTEM_TICK_HOOK() {}

#define CH_CFG_SYSTEM_HALT_HOOK(reason) {}

#define CH_CFG_TRACE_HOOK(tep) {}

#endif
