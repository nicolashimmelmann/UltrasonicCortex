/*
 * Utils.cpp
 *
 *  Created on: 10.08.2017
 *      Author: nicolas
 */

#include "include/Utils.h"

void Utils::sort(uint16_t * arr, uint16_t size) {
	for (uint16_t n = size; n>1; --n)
	{
		for (uint16_t i=0; i<n-1; ++i)
		{
			if (arr[i] > arr[i+1])
			{
				uint16_t tmp = arr[i+1];
				arr[i+1] = arr[i];
				arr[i] = tmp;
			}
	    }
	}
}



