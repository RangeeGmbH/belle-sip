/*
 * Copyright (c) 2012-2019 Belledonne Communications SARL.
 *
 * This file is part of belle-sip.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * Copyright (c), MM Weiss
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the MM Weiss nor the names of its contributors
 *     may be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  clock_gettime_stub.c
 *  gcc -Wall -c clock_gettime_stub.c
 *  posix realtime functions; MacOS user space glue
 */

/*  @comment
 *  other possible implementation using intel builtin rdtsc
 *  rdtsc-workaround: http://www.mcs.anl.gov/~kazutomo/rdtsc.html
 *
 *  we could get the ticks by doing this
 *
 *  __asm __volatile("mov %%ebx, %%esi\n\t"
 *  		"cpuid\n\t"
 *  		"xchg %%esi, %%ebx\n\t"
 *  		"rdtsc"
 *  		: "=a" (a),
 *  		  "=d" (d)
 * 	);

 *  we could even replace our tricky sched_yield call by assembly code to get a better accurency,
 *  anyway the following C stub will satisfy 99% of apps using posix clock_gettime call,
 *  moreover, the setter version (clock_settime) could be easly written using mach primitives:
 *  http://www.opensource.apple.com/source/xnu/xnu-${VERSION}/osfmk/man/ (clock_[set|get]_time)
 *
 *  hackers don't be crackers, don't you use a flush toilet?
 *
 *
 *  @see draft: ./posix-realtime-stub/posix-realtime-stub.c
 *
 */

#ifdef __APPLE__

#include <errno.h>
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include "clock_gettime.h"

static mach_timebase_info_data_t __clock_gettime_inf;
static clock_serv_t belle_sip_calandar_clk;
static clock_serv_t belle_sip_system_clk;
static int belle_sip_clock_serv_ready = FALSE;

int bc_clock_gettime(bc_clockid_t clk_id, struct timespec *tp) {
	kern_return_t ret;
	clock_serv_t clk_serv;
	mach_timespec_t tm;
	uint64_t start, end, delta, nano;
	int retval = -1;

	if (!belle_sip_clock_serv_ready) { /*host_get_clock_service is pretty slow*/
		if (KERN_SUCCESS != host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &belle_sip_calandar_clk) ||
		    KERN_SUCCESS != host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &belle_sip_system_clk)) {
			return -1;
		} else {
			belle_sip_clock_serv_ready = TRUE;
		}
	}

	switch (clk_id) {
		case BC_CLOCK_REALTIME:
		case BC_CLOCK_MONOTONIC:
			clk_serv = (clk_id == BC_CLOCK_REALTIME) ? belle_sip_calandar_clk : belle_sip_system_clk;
			if (KERN_SUCCESS == (ret = clock_get_time(clk_serv, &tm))) {
				tp->tv_sec = tm.tv_sec;
				tp->tv_nsec = tm.tv_nsec;
				retval = 0;
			}
			if (KERN_SUCCESS != ret) {
				errno = EINVAL;
				retval = -1;
			}
			break;
		case BC_CLOCK_PROCESS_CPUTIME_ID:
		case BC_CLOCK_THREAD_CPUTIME_ID:
			start = mach_absolute_time();
			if (clk_id == BC_CLOCK_PROCESS_CPUTIME_ID) {
				getpid();
			} else {
				sched_yield();
			}
			end = mach_absolute_time();
			delta = end - start;
			if (0 == __clock_gettime_inf.denom) {
				mach_timebase_info(&__clock_gettime_inf);
			}
			nano = delta * __clock_gettime_inf.numer / __clock_gettime_inf.denom;
			tp->tv_sec = nano * 1e-9;
			tp->tv_nsec = nano - (tp->tv_sec * 1e9);
			retval = 0;
			break;
		default:
			errno = EINVAL;
			retval = -1;
	}
	return retval;
}

#endif // __APPLE__

/* EOF */
