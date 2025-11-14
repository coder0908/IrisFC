/*
 * main.c
 *
 *  Created on: Oct 8, 2025
 *      Author: unRR
 */

#include "init.h"
#include "main.h"

int main()
{
	pp_init();
	if (!setup()) {

		return -1;
	}
	int i=0;
	for (;;) {
		i++;
		loop();
	}

	return 0;
}


/*

 */

