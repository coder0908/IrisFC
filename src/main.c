/*
 * main.c
 *
 *  Created on: Oct 8, 2025
 *      Author: coder0908
 */

#include "app/app.h"
#include "main.h"



int main()
{
	platform_hal_init();
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



