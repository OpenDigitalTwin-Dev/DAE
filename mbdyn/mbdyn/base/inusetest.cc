/* $Header$ */
/* 
 * MBDyn (C) is a multibody analysis code. 
 * http://www.mbdyn.org
 *
 * Copyright (C) 1996-2023
 *
 * Pierangelo Masarati	<pierangelo.masarati@polimi.it>
 * Paolo Mantegazza	<paolo.mantegazza@polimi.it>
 *
 * Dipartimento di Ingegneria Aerospaziale - Politecnico di Milano
 * via La Masa, 34 - 20156 Milano, Italy
 * http://www.aero.polimi.it
 *
 * Changing this copyright notice is forbidden.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 2 of the License).
 * 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "mbconfig.h"           /* This goes first in every *.c,*.cc file */

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <vector>

#include "myassert.h"
#include "mynewmem.h"

#ifdef USE_MULTITHREAD

#include <unistd.h>
#include "ac/pthread.h"		/* includes POSIX semaphores */

#include "veciter.h"
#include "filename.h"
#include "mbsleep.h"

unsigned dst = 100;
unsigned rst = 10;

class A : public InUse {
private:
	unsigned label;
	unsigned who;
public:
	A(unsigned l) : label(l) {};
	virtual ~A(void) {};

	unsigned Label(void) const { return label; };
	void Set(unsigned w) {
		who = w;

		double d = 1.e-6*dst;
		if (rst) {
			d += 1.e-6 * (rand() % rst);
		}
		mbsleep_t s;
		mbsleep_real2sleep(d, &s);
		if (!MBSLEEP_ISZERO(s)) {
			mbsleep(&s);
		}
	};
	unsigned Get(void) const { return who; };
};

struct Arg {
	unsigned n;
	unsigned cnt;
	bool stop;
	pthread_t t;
	sem_t s;
	MT_VecIter<A *> i;
	unsigned *c;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void
f2(VecIter<A *> &i, unsigned n, unsigned &cnt)
{
	A* pA = NULL;

	cnt = 0;

	if (i.bGetFirst(pA)) {
		do {
			pA->Set(n);
			cnt++;
		} while (i.bGetNext(pA));
	}
}

void *
f(void *p)
{
	Arg *arg = (Arg *)p;

	while (true) {
		sem_wait(&arg->s);

		if (arg->stop) {
			break;
		}

		f2(arg->i, arg->n, arg->cnt);

		pthread_mutex_lock(&mutex);
		--*(arg->c);
		silent_cout("f: count " << *arg->c + 1 << " => " << *arg->c << std::endl);
		if (*arg->c == 0) {
			silent_cout("f: finished" << std::endl);
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

void inusetest(const unsigned size, const unsigned nt)
{
	Arg *arg = NULL;
	arg = new Arg[nt];
	unsigned c;

	A** ppA = new A*[size];
	for (unsigned i = 0; i < size; i++) {
		ppA[i] = new A(i);
	}

	for (unsigned i = 0; i < nt; i++) {
		arg[i].n = i;
		arg[i].i.Init(ppA, size);
		arg[i].c = &c;
		arg[i].cnt = 0;
		arg[i].stop = false;

		if (i == 0) continue;

		sem_init(&arg[i].s, 0, 0);
		pthread_create(&arg[i].t, NULL, f, &arg[i]);
	}

	for (unsigned k = 0; k < 10; k++) {
		arg[0].i.ResetAccessData();
		c = nt - 1;

		for (unsigned i = 1; i < nt; i++) {
			sem_post(&arg[i].s);
		}

		f2(arg[0].i, arg[0].n, arg[0].cnt);

		if (nt > 1) {
			pthread_mutex_lock(&mutex);
			if (c) {
				silent_cout("main: count " << c << std::endl);
				pthread_cond_wait(&cond, &mutex);
				silent_cout("main: wait is over; count" << c << std::endl);
			}
			pthread_mutex_unlock(&mutex);
		}

		c = arg[0].cnt;
		fprintf(stderr, "cnt = {%u", arg[0].cnt);
		for (unsigned i = 1; i < nt; i++) {
			fprintf(stderr, ", %u", arg[i].cnt);
			c += arg[i].cnt;
		}
		fprintf(stderr, "} = %u\n", c);

                std::vector<unsigned> who(nt, 0u);

		for (unsigned i = 0; i < size; i++) {
			who[ppA[i]->Get()]++;
		}
	
		c = who[0];
		fprintf(stderr, "who = {%u", who[0]);
		for (unsigned i = 1; i < nt; i++) {
			fprintf(stderr, ", %u", who[i]);
			c += who[i];
		}
		fprintf(stderr, "} = %u\n", c);
	}

	for (unsigned i = 1; i < nt; i++) {
		arg[i].stop = true;
		sem_post(&arg[i].s);
		pthread_join(arg[i].t, NULL);
	}

	delete[] arg;
}

MBDYN_TESTSUITE_TEST(inusetest, inusetest_1000_2)
{
     inusetest(1000, 2);
}

MBDYN_TESTSUITE_TEST(inusetest, inusetest_2000_4)
{
     inusetest(2000, 4);
}

#endif /* ! USE_MULTITHREAD */

MBDYN_DEFINE_OPERATOR_NEW_DELETE

int main(int argc, char* argv[])
{
     MBDYN_TESTSUITE_INIT(&argc, argv);

     return MBDYN_RUN_ALL_TESTS();
}
