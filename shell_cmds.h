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

#if !defined(SHELL_CMDS_H)
#define SHELL_CMDS_H

extern struct sig_pid_param_f pid_p;

extern bool loop_enable;
extern bool update_pid_params;

extern void start_shell(void);

#endif
