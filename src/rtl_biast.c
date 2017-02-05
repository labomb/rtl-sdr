/*
 * rtl-sdr, turns your Realtek RTL2832 based DVB dongle into a SDR receiver
 *
 * rtl_biast - bias tee support for RTL-SDR.com V3 dongles
 *
 * Copyright (C) 2012 by Steve Markgraf <steve@steve-m.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#include "getopt/getopt.h"
#endif

#include "rtl-sdr.h"

void usage(void)
{
	fprintf(stderr,
		"rtl_biast, a tool for turning the RTL-SDR.com \n"
		"bias tee ON and OFF. Example to turn on the \n"
		"bias tee for device 0:  rtl_biast -d 0 -b 1\n\n"
		"Usage:\n"
		"\t[-d device_index (default: 0)]\n"
		"\t[-b bias_onoff (0 or 1, default: 0)]\n");
	exit(1);
}

int main(int argc, char **argv)
{
	int opt;
	uint32_t dev_index = 0, bias_onoff = 0;
	rtlsdr_dev_t *dev = NULL;

	while ((opt = getopt(argc, argv, "d:b:h?")) != -1) {
		switch (opt) {
		case 'd':
			dev_index = atoi(optarg);
			break;
		case 'b':
			bias_onoff = atoi(optarg);
			break;
		default:
			usage();
			break;
		}
	}

	if ((bias_onoff != 0) && (bias_onoff != 1)) {
		fprintf(stderr, "Invalid -b option.\n");
		usage();
	}

	if (rtlsdr_open(&dev, dev_index) < 0) {
		fprintf(stderr, "Error opening RTLSDR device: %s\n", strerror(errno));
		exit(1);
	}

	rtlsdr_set_bias_tee(dev, bias_onoff);

	// Don't do close_bt, do a normal close to leave bias-t in requested state
	rtlsdr_close(dev);

	return 0;
}
