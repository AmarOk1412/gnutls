/*
 * Copyright (C) 2017 Red Hat, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * GnuTLS is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * GnuTLS is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GnuTLS; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

/* This tests whether a client will reject a server advertising
 * an MD5 signature algorithm */

#if defined(_WIN32) || !defined(ENABLE_SSL2)

/* socketpair isn't supported on Win32. */
int main(int argc, char **argv)
{
	exit(77);
}

#else

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <gnutls/gnutls.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#include "utils.h"
#include "cert-common.h"

pid_t child;

static void tls_log_func(int level, const char *str)
{
	fprintf(stderr, "%s |<%d>| %s", child ? "server" : "client", level,
		str);
}


static unsigned char tls1_hello[] = { /* server hello etc. */
0x16, 0x03, 0x03, 0x00, 0x39, 0x02, 0x00, 0x00, 0x35, 0x03, 0x03, 0x95, 0x66, 0x0A, 0x55, 0xBC,
0x04, 0x84, 0xBD, 0x7E, 0xA6, 0xCE, 0x2B, 0x1C, 0x7B, 0x72, 0x28, 0x0D, 0x8E, 0x2C, 0x34, 0xC8,
0xFF, 0x73, 0xBA, 0x6C, 0x9F, 0x20, 0xB9, 0x18, 0xCD, 0x22, 0xCB, 0x00, 0x00, 0x9E, 0x00, 0x00,
0x0D, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x00, 0x01, 0x00, 0x00, 0x23, 0x00, 0x00, 0x16, 0x03,
0x03, 0x10, 0xE3, 0x0B, 0x00, 0x10, 0xDF, 0x00, 0x10, 0xDC, 0x00, 0x05, 0x4F, 0x30, 0x82, 0x05,
0x4B, 0x30, 0x82, 0x04, 0x33, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x10, 0x4C, 0x8E, 0x18, 0x71,
0x4B, 0x34, 0xE7, 0x5E, 0x8D, 0xAE, 0xFB, 0xE8, 0xF6, 0x4C, 0x3A, 0x82, 0x30, 0x0D, 0x06, 0x09,
0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x30, 0x81, 0x90, 0x31, 0x0B,
0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x47, 0x42, 0x31, 0x1B, 0x30, 0x19, 0x06,
0x03, 0x55, 0x04, 0x08, 0x13, 0x12, 0x47, 0x72, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20, 0x4D, 0x61,
0x6E, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x04,
0x07, 0x13, 0x07, 0x53, 0x61, 0x6C, 0x66, 0x6F, 0x72, 0x64, 0x31, 0x1A, 0x30, 0x18, 0x06, 0x03,
0x55, 0x04, 0x0A, 0x13, 0x11, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x43, 0x41, 0x20, 0x4C,
0x69, 0x6D, 0x69, 0x74, 0x65, 0x64, 0x31, 0x36, 0x30, 0x34, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13,
0x2D, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x52, 0x53, 0x41, 0x20, 0x44, 0x6F, 0x6D, 0x61,
0x69, 0x6E, 0x20, 0x56, 0x61, 0x6C, 0x69, 0x64, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x53, 0x65,
0x63, 0x75, 0x72, 0x65, 0x20, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x43, 0x41, 0x30, 0x1E,
0x17, 0x0D, 0x31, 0x36, 0x30, 0x37, 0x30, 0x37, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5A, 0x17,
0x0D, 0x31, 0x37, 0x30, 0x39, 0x30, 0x35, 0x32, 0x33, 0x35, 0x39, 0x35, 0x39, 0x5A, 0x30, 0x59,
0x31, 0x21, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x13, 0x18, 0x44, 0x6F, 0x6D, 0x61, 0x69,
0x6E, 0x20, 0x43, 0x6F, 0x6E, 0x74, 0x72, 0x6F, 0x6C, 0x20, 0x56, 0x61, 0x6C, 0x69, 0x64, 0x61,
0x74, 0x65, 0x64, 0x31, 0x1D, 0x30, 0x1B, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x13, 0x14, 0x50, 0x6F,
0x73, 0x69, 0x74, 0x69, 0x76, 0x65, 0x53, 0x53, 0x4C, 0x20, 0x57, 0x69, 0x6C, 0x64, 0x63, 0x61,
0x72, 0x64, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x0C, 0x2A, 0x2E, 0x62,
0x61, 0x64, 0x73, 0x73, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0D, 0x06,
0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0F,
0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xC2, 0x04, 0xEC, 0xF8, 0x8C, 0xEE,
0x04, 0xC2, 0xB3, 0xD8, 0x50, 0xD5, 0x70, 0x58, 0xCC, 0x93, 0x18, 0xEB, 0x5C, 0xA8, 0x68, 0x49,
0xB0, 0x22, 0xB5, 0xF9, 0x95, 0x9E, 0xB1, 0x2B, 0x2C, 0x76, 0x3E, 0x6C, 0xC0, 0x4B, 0x60, 0x4C,
0x4C, 0xEA, 0xB2, 0xB4, 0xC0, 0x0F, 0x80, 0xB6, 0xB0, 0xF9, 0x72, 0xC9, 0x86, 0x02, 0xF9, 0x5C,
0x41, 0x5D, 0x13, 0x2B, 0x7F, 0x71, 0xC4, 0x4B, 0xBC, 0xE9, 0x94, 0x2E, 0x50, 0x37, 0xA6, 0x67,
0x1C, 0x61, 0x8C, 0xF6, 0x41, 0x42, 0xC5, 0x46, 0xD3, 0x16, 0x87, 0x27, 0x9F, 0x74, 0xEB, 0x0A,
0x9D, 0x11, 0x52, 0x26, 0x21, 0x73, 0x6C, 0x84, 0x4C, 0x79, 0x55, 0xE4, 0xD1, 0x6B, 0xE8, 0x06,
0x3D, 0x48, 0x15, 0x52, 0xAD, 0xB3, 0x28, 0xDB, 0xAA, 0xFF, 0x6E, 0xFF, 0x60, 0x95, 0x4A, 0x77,
0x6B, 0x39, 0xF1, 0x24, 0xD1, 0x31, 0xB6, 0xDD, 0x4D, 0xC0, 0xC4, 0xFC, 0x53, 0xB9, 0x6D, 0x42,
0xAD, 0xB5, 0x7C, 0xFE, 0xAE, 0xF5, 0x15, 0xD2, 0x33, 0x48, 0xE7, 0x22, 0x71, 0xC7, 0xC2, 0x14,
0x7A, 0x6C, 0x28, 0xEA, 0x37, 0x4A, 0xDF, 0xEA, 0x6C, 0xB5, 0x72, 0xB4, 0x7E, 0x5A, 0xA2, 0x16,
0xDC, 0x69, 0xB1, 0x57, 0x44, 0xDB, 0x0A, 0x12, 0xAB, 0xDE, 0xC3, 0x0F, 0x47, 0x74, 0x5C, 0x41,
0x22, 0xE1, 0x9A, 0xF9, 0x1B, 0x93, 0xE6, 0xAD, 0x22, 0x06, 0x29, 0x2E, 0xB1, 0xBA, 0x49, 0x1C,
0x0C, 0x27, 0x9E, 0xA3, 0xFB, 0x8B, 0xF7, 0x40, 0x72, 0x00, 0xAC, 0x92, 0x08, 0xD9, 0x8C, 0x57,
0x84, 0x53, 0x81, 0x05, 0xCB, 0xE6, 0xFE, 0x6B, 0x54, 0x98, 0x40, 0x27, 0x85, 0xC7, 0x10, 0xBB,
0x73, 0x70, 0xEF, 0x69, 0x18, 0x41, 0x07, 0x45, 0x55, 0x7C, 0xF9, 0x64, 0x3F, 0x3D, 0x2C, 0xC3,
0xA9, 0x7C, 0xEB, 0x93, 0x1A, 0x4C, 0x86, 0xD1, 0xCA, 0x85, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3,
0x82, 0x01, 0xD5, 0x30, 0x82, 0x01, 0xD1, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18,
0x30, 0x16, 0x80, 0x14, 0x90, 0xAF, 0x6A, 0x3A, 0x94, 0x5A, 0x0B, 0xD8, 0x90, 0xEA, 0x12, 0x56,
0x73, 0xDF, 0x43, 0xB4, 0x3A, 0x28, 0xDA, 0xE7, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04,
0x16, 0x04, 0x14, 0x9D, 0xEE, 0xC1, 0x7B, 0x81, 0x0B, 0x3A, 0x47, 0x69, 0x71, 0x18, 0x7D, 0x11,
0x37, 0x93, 0xBC, 0xA5, 0x1B, 0x3F, 0xFB, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01,
0xFF, 0x04, 0x04, 0x03, 0x02, 0x05, 0xA0, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01,
0xFF, 0x04, 0x02, 0x30, 0x00, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x25, 0x04, 0x16, 0x30, 0x14,
0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05,
0x05, 0x07, 0x03, 0x02, 0x30, 0x4F, 0x06, 0x03, 0x55, 0x1D, 0x20, 0x04, 0x48, 0x30, 0x46, 0x30,
0x3A, 0x06, 0x0B, 0x2B, 0x06, 0x01, 0x04, 0x01, 0xB2, 0x31, 0x01, 0x02, 0x02, 0x07, 0x30, 0x2B,
0x30, 0x29, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x01, 0x16, 0x1D, 0x68, 0x74,
0x74, 0x70, 0x73, 0x3A, 0x2F, 0x2F, 0x73, 0x65, 0x63, 0x75, 0x72, 0x65, 0x2E, 0x63, 0x6F, 0x6D,
0x6F, 0x64, 0x6F, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x43, 0x50, 0x53, 0x30, 0x08, 0x06, 0x06, 0x67,
0x81, 0x0C, 0x01, 0x02, 0x01, 0x30, 0x54, 0x06, 0x03, 0x55, 0x1D, 0x1F, 0x04, 0x4D, 0x30, 0x4B,
0x30, 0x49, 0xA0, 0x47, 0xA0, 0x45, 0x86, 0x43, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x63,
0x72, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x6F, 0x64, 0x6F, 0x63, 0x61, 0x2E, 0x63, 0x6F, 0x6D, 0x2F,
0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x52, 0x53, 0x41, 0x44, 0x6F, 0x6D, 0x61, 0x69, 0x6E, 0x56,
0x61, 0x6C, 0x69, 0x64, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x53, 0x65, 0x63, 0x75, 0x72, 0x65, 0x53,
0x65, 0x72, 0x76, 0x65, 0x72, 0x43, 0x41, 0x2E, 0x63, 0x72, 0x6C, 0x30, 0x81, 0x85, 0x06, 0x08,
0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x01, 0x01, 0x04, 0x79, 0x30, 0x77, 0x30, 0x4F, 0x06, 0x08,
0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x02, 0x86, 0x43, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F,
0x2F, 0x63, 0x72, 0x74, 0x2E, 0x63, 0x6F, 0x6D, 0x6F, 0x64, 0x6F, 0x63, 0x61, 0x2E, 0x63, 0x6F,
0x6D, 0x2F, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x52, 0x53, 0x41, 0x44, 0x6F, 0x6D, 0x61, 0x69,
0x6E, 0x56, 0x61, 0x6C, 0x69, 0x64, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x53, 0x65, 0x63, 0x75, 0x72,
0x65, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x43, 0x41, 0x2E, 0x63, 0x72, 0x74, 0x30, 0x24, 0x06,
0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x86, 0x18, 0x68, 0x74, 0x74, 0x70, 0x3A,
0x2F, 0x2F, 0x6F, 0x63, 0x73, 0x70, 0x2E, 0x63, 0x6F, 0x6D, 0x6F, 0x64, 0x6F, 0x63, 0x61, 0x2E,
0x63, 0x6F, 0x6D, 0x30, 0x23, 0x06, 0x03, 0x55, 0x1D, 0x11, 0x04, 0x1C, 0x30, 0x1A, 0x82, 0x0C,
0x2A, 0x2E, 0x62, 0x61, 0x64, 0x73, 0x73, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x82, 0x0A, 0x62, 0x61,
0x64, 0x73, 0x73, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86,
0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x75, 0x48, 0x83, 0x88,
0x9C, 0x55, 0x24, 0x37, 0x30, 0x07, 0xEB, 0x26, 0x68, 0xC8, 0x79, 0x1C, 0x5C, 0xAE, 0x9A, 0x02,
0x9A, 0xB5, 0x52, 0x75, 0x44, 0xAC, 0xA9, 0xED, 0x59, 0x65, 0xD0, 0xC6, 0x47, 0x26, 0x04, 0x8D,
0x57, 0x89, 0x16, 0x2E, 0x71, 0x18, 0x48, 0x98, 0x68, 0x1C, 0xF6, 0x31, 0xF5, 0x26, 0x4B, 0xE8,
0x81, 0x44, 0xB1, 0xFF, 0x5C, 0x65, 0x3D, 0x78, 0x54, 0x94, 0xC3, 0x86, 0x9D, 0x48, 0x96, 0xE8,
0x32, 0xAF, 0xE1, 0x8F, 0x94, 0x47, 0xBE, 0x37, 0x8C, 0xC3, 0xED, 0x4D, 0x97, 0xBB, 0xC6, 0x2A,
0x37, 0x72, 0x01, 0x3A, 0x8F, 0x82, 0xA4, 0x34, 0x44, 0xC4, 0xC4, 0xF8, 0x50, 0x24, 0x48, 0x9E,
0x19, 0xF0, 0xEC, 0xE1, 0xC6, 0x13, 0x44, 0x26, 0xB6, 0x65, 0xE1, 0x62, 0x49, 0x87, 0xA4, 0xF4,
0xD8, 0xC4, 0x39, 0x3C, 0x7D, 0x42, 0xC8, 0xA4, 0x2A, 0x54, 0x05, 0xA0, 0xDC, 0x0A, 0xF8, 0x2B,
0x22, 0x94, 0x93, 0x78, 0x4E, 0x6A, 0x36, 0x1B, 0xD2, 0xE7, 0xE9, 0xAE, 0x84, 0xED, 0x13, 0x1D,
0xA1, 0xF7, 0xA2, 0x83, 0x81, 0x03, 0x4C, 0x9E, 0x21, 0xFB, 0xBF, 0xA8, 0x30, 0xFE, 0xEB, 0x00,
0x68, 0xB1, 0x7F, 0xBA, 0x5D, 0xE2, 0x5D, 0xFF, 0x41, 0x1F, 0xD6, 0xF5, 0xA6, 0x5C, 0x8A, 0xEF,
0x81, 0x80, 0xC8, 0xF1, 0x52, 0x00, 0x17, 0x9D, 0xD1, 0x96, 0x1A, 0x7D, 0x5E, 0xD2, 0x83, 0xB3,
0x82, 0xC2, 0x3D, 0x46, 0x83, 0xA5, 0x1E, 0xB4, 0x36, 0x35, 0x38, 0xC4, 0x7A, 0x2E, 0xDF, 0x0B,
0xA1, 0x98, 0x63, 0x58, 0x0B, 0x1E, 0xD0, 0x6D, 0x83, 0x1F, 0xF1, 0x72, 0x4D, 0x09, 0xAC, 0x96,
0x1A, 0x0B, 0xE5, 0xF6, 0x34, 0x4C, 0xAB, 0xBC, 0xBC, 0x99, 0x5B, 0x82, 0x59, 0xE6, 0x6C, 0xD3,
0xDB, 0x98, 0xE0, 0xCE, 0x95, 0x3B, 0xCF, 0x4E, 0x17, 0xC3, 0xEE, 0x3A, 0x00, 0x06, 0x0C, 0x30,
0x82, 0x06, 0x08, 0x30, 0x82, 0x03, 0xF0, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x10, 0x2B, 0x2E,
0x6E, 0xEA, 0xD9, 0x75, 0x36, 0x6C, 0x14, 0x8A, 0x6E, 0xDB, 0xA3, 0x7C, 0x8C, 0x07, 0x30, 0x0D,
0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0C, 0x05, 0x00, 0x30, 0x81, 0x85,
0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x47, 0x42, 0x31, 0x1B, 0x30,
0x19, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x12, 0x47, 0x72, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20,
0x4D, 0x61, 0x6E, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03,
0x55, 0x04, 0x07, 0x13, 0x07, 0x53, 0x61, 0x6C, 0x66, 0x6F, 0x72, 0x64, 0x31, 0x1A, 0x30, 0x18,
0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x11, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x43, 0x41,
0x20, 0x4C, 0x69, 0x6D, 0x69, 0x74, 0x65, 0x64, 0x31, 0x2B, 0x30, 0x29, 0x06, 0x03, 0x55, 0x04,
0x03, 0x13, 0x22, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x52, 0x53, 0x41, 0x20, 0x43, 0x65,
0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x41, 0x75, 0x74, 0x68,
0x6F, 0x72, 0x69, 0x74, 0x79, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x34, 0x30, 0x32, 0x31, 0x32, 0x30,
0x30, 0x30, 0x30, 0x30, 0x30, 0x5A, 0x17, 0x0D, 0x32, 0x39, 0x30, 0x32, 0x31, 0x31, 0x32, 0x33,
0x35, 0x39, 0x35, 0x39, 0x5A, 0x30, 0x81, 0x90, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
0x06, 0x13, 0x02, 0x47, 0x42, 0x31, 0x1B, 0x30, 0x19, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x12,
0x47, 0x72, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20, 0x4D, 0x61, 0x6E, 0x63, 0x68, 0x65, 0x73, 0x74,
0x65, 0x72, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x07, 0x53, 0x61, 0x6C,
0x66, 0x6F, 0x72, 0x64, 0x31, 0x1A, 0x30, 0x18, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x11, 0x43,
0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x43, 0x41, 0x20, 0x4C, 0x69, 0x6D, 0x69, 0x74, 0x65, 0x64,
0x31, 0x36, 0x30, 0x34, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x2D, 0x43, 0x4F, 0x4D, 0x4F, 0x44,
0x4F, 0x20, 0x52, 0x53, 0x41, 0x20, 0x44, 0x6F, 0x6D, 0x61, 0x69, 0x6E, 0x20, 0x56, 0x61, 0x6C,
0x69, 0x64, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x53, 0x65, 0x63, 0x75, 0x72, 0x65, 0x20, 0x53,
0x65, 0x72, 0x76, 0x65, 0x72, 0x20, 0x43, 0x41, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0D, 0x06, 0x09,
0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0F, 0x00,
0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0x8E, 0xC2, 0x02, 0x19, 0xE1, 0xA0, 0x59,
0xA4, 0xEB, 0x38, 0x35, 0x8D, 0x2C, 0xFD, 0x01, 0xD0, 0xD3, 0x49, 0xC0, 0x64, 0xC7, 0x0B, 0x62,
0x05, 0x45, 0x16, 0x3A, 0xA8, 0xA0, 0xC0, 0x0C, 0x02, 0x7F, 0x1D, 0xCC, 0xDB, 0xC4, 0xA1, 0x6D,
0x77, 0x03, 0xA3, 0x0F, 0x86, 0xF9, 0xE3, 0x06, 0x9C, 0x3E, 0x0B, 0x81, 0x8A, 0x9B, 0x49, 0x1B,
0xAD, 0x03, 0xBE, 0xFA, 0x4B, 0xDB, 0x8C, 0x20, 0xED, 0xD5, 0xCE, 0x5E, 0x65, 0x8E, 0x3E, 0x0D,
0xAF, 0x4C, 0xC2, 0xB0, 0xB7, 0x45, 0x5E, 0x52, 0x2F, 0x34, 0xDE, 0x48, 0x24, 0x64, 0xB4, 0x41,
0xAE, 0x00, 0x97, 0xF7, 0xBE, 0x67, 0xDE, 0x9E, 0xD0, 0x7A, 0xA7, 0x53, 0x80, 0x3B, 0x7C, 0xAD,
0xF5, 0x96, 0x55, 0x6F, 0x97, 0x47, 0x0A, 0x7C, 0x85, 0x8B, 0x22, 0x97, 0x8D, 0xB3, 0x84, 0xE0,
0x96, 0x57, 0xD0, 0x70, 0x18, 0x60, 0x96, 0x8F, 0xEE, 0x2D, 0x07, 0x93, 0x9D, 0xA1, 0xBA, 0xCA,
0xD1, 0xCD, 0x7B, 0xE9, 0xC4, 0x2A, 0x9A, 0x28, 0x21, 0x91, 0x4D, 0x6F, 0x92, 0x4F, 0x25, 0xA5,
0xF2, 0x7A, 0x35, 0xDD, 0x26, 0xDC, 0x46, 0xA5, 0xD0, 0xAC, 0x59, 0x35, 0x8C, 0xFF, 0x4E, 0x91,
0x43, 0x50, 0x3F, 0x59, 0x93, 0x1E, 0x6C, 0x51, 0x21, 0xEE, 0x58, 0x14, 0xAB, 0xFE, 0x75, 0x50,
0x78, 0x3E, 0x4C, 0xB0, 0x1C, 0x86, 0x13, 0xFA, 0x6B, 0x98, 0xBC, 0xE0, 0x3B, 0x94, 0x1E, 0x85,
0x52, 0xDC, 0x03, 0x93, 0x24, 0x18, 0x6E, 0xCB, 0x27, 0x51, 0x45, 0xE6, 0x70, 0xDE, 0x25, 0x43,
0xA4, 0x0D, 0xE1, 0x4A, 0xA5, 0xED, 0xB6, 0x7E, 0xC8, 0xCD, 0x6D, 0xEE, 0x2E, 0x1D, 0x27, 0x73,
0x5D, 0xDC, 0x45, 0x30, 0x80, 0xAA, 0xE3, 0xB2, 0x41, 0x0B, 0xAF, 0xBD, 0x44, 0x87, 0xDA, 0xB9,
0xE5, 0x1B, 0x9D, 0x7F, 0xAE, 0xE5, 0x85, 0x82, 0xA5, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x82,
0x01, 0x65, 0x30, 0x82, 0x01, 0x61, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30,
0x16, 0x80, 0x14, 0xBB, 0xAF, 0x7E, 0x02, 0x3D, 0xFA, 0xA6, 0xF1, 0x3C, 0x84, 0x8E, 0xAD, 0xEE,
0x38, 0x98, 0xEC, 0xD9, 0x32, 0x32, 0xD4, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16,
0x04, 0x14, 0x90, 0xAF, 0x6A, 0x3A, 0x94, 0x5A, 0x0B, 0xD8, 0x90, 0xEA, 0x12, 0x56, 0x73, 0xDF,
0x43, 0xB4, 0x3A, 0x28, 0xDA, 0xE7, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF,
0x04, 0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x12, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF,
0x04, 0x08, 0x30, 0x06, 0x01, 0x01, 0xFF, 0x02, 0x01, 0x00, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D,
0x25, 0x04, 0x16, 0x30, 0x14, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01, 0x06,
0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x02, 0x30, 0x1B, 0x06, 0x03, 0x55, 0x1D, 0x20,
0x04, 0x14, 0x30, 0x12, 0x30, 0x06, 0x06, 0x04, 0x55, 0x1D, 0x20, 0x00, 0x30, 0x08, 0x06, 0x06,
0x67, 0x81, 0x0C, 0x01, 0x02, 0x01, 0x30, 0x4C, 0x06, 0x03, 0x55, 0x1D, 0x1F, 0x04, 0x45, 0x30,
0x43, 0x30, 0x41, 0xA0, 0x3F, 0xA0, 0x3D, 0x86, 0x3B, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F,
0x63, 0x72, 0x6C, 0x2E, 0x63, 0x6F, 0x6D, 0x6F, 0x64, 0x6F, 0x63, 0x61, 0x2E, 0x63, 0x6F, 0x6D,
0x2F, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x52, 0x53, 0x41, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66,
0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x41, 0x75, 0x74, 0x68, 0x6F, 0x72, 0x69, 0x74, 0x79,
0x2E, 0x63, 0x72, 0x6C, 0x30, 0x71, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x01, 0x01,
0x04, 0x65, 0x30, 0x63, 0x30, 0x3B, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x02,
0x86, 0x2F, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x63, 0x72, 0x74, 0x2E, 0x63, 0x6F, 0x6D,
0x6F, 0x64, 0x6F, 0x63, 0x61, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F,
0x52, 0x53, 0x41, 0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74, 0x43, 0x41, 0x2E, 0x63, 0x72,
0x74, 0x30, 0x24, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x86, 0x18, 0x68,
0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x6F, 0x63, 0x73, 0x70, 0x2E, 0x63, 0x6F, 0x6D, 0x6F, 0x64,
0x6F, 0x63, 0x61, 0x2E, 0x63, 0x6F, 0x6D, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7,
0x0D, 0x01, 0x01, 0x0C, 0x05, 0x00, 0x03, 0x82, 0x02, 0x01, 0x00, 0x4E, 0x2B, 0x76, 0x4F, 0x92,
0x1C, 0x62, 0x36, 0x89, 0xBA, 0x77, 0xC1, 0x27, 0x05, 0xF4, 0x1C, 0xD6, 0x44, 0x9D, 0xA9, 0x9A,
0x3E, 0xAA, 0xD5, 0x66, 0x66, 0x01, 0x3E, 0xEA, 0x49, 0xE6, 0xA2, 0x35, 0xBC, 0xFA, 0xF6, 0xDD,
0x95, 0x8E, 0x99, 0x35, 0x98, 0x0E, 0x36, 0x18, 0x75, 0xB1, 0xDD, 0xDD, 0x50, 0x72, 0x7C, 0xAE,
0xDC, 0x77, 0x88, 0xCE, 0x0F, 0xF7, 0x90, 0x20, 0xCA, 0xA3, 0x67, 0x2E, 0x1F, 0x56, 0x7F, 0x7B,
0xE1, 0x44, 0xEA, 0x42, 0x95, 0xC4, 0x5D, 0x0D, 0x01, 0x50, 0x46, 0x15, 0xF2, 0x81, 0x89, 0x59,
0x6C, 0x8A, 0xDD, 0x8C, 0xF1, 0x12, 0xA1, 0x8D, 0x3A, 0x42, 0x8A, 0x98, 0xF8, 0x4B, 0x34, 0x7B,
0x27, 0x3B, 0x08, 0xB4, 0x6F, 0x24, 0x3B, 0x72, 0x9D, 0x63, 0x74, 0x58, 0x3C, 0x1A, 0x6C, 0x3F,
0x4F, 0xC7, 0x11, 0x9A, 0xC8, 0xA8, 0xF5, 0xB5, 0x37, 0xEF, 0x10, 0x45, 0xC6, 0x6C, 0xD9, 0xE0,
0x5E, 0x95, 0x26, 0xB3, 0xEB, 0xAD, 0xA3, 0xB9, 0xEE, 0x7F, 0x0C, 0x9A, 0x66, 0x35, 0x73, 0x32,
0x60, 0x4E, 0xE5, 0xDD, 0x8A, 0x61, 0x2C, 0x6E, 0x52, 0x11, 0x77, 0x68, 0x96, 0xD3, 0x18, 0x75,
0x51, 0x15, 0x00, 0x1B, 0x74, 0x88, 0xDD, 0xE1, 0xC7, 0x38, 0x04, 0x43, 0x28, 0xE9, 0x16, 0xFD,
0xD9, 0x05, 0xD4, 0x5D, 0x47, 0x27, 0x60, 0xD6, 0xFB, 0x38, 0x3B, 0x6C, 0x72, 0xA2, 0x94, 0xF8,
0x42, 0x1A, 0xDF, 0xED, 0x6F, 0x06, 0x8C, 0x45, 0xC2, 0x06, 0x00, 0xAA, 0xE4, 0xE8, 0xDC, 0xD9,
0xB5, 0xE1, 0x73, 0x78, 0xEC, 0xF6, 0x23, 0xDC, 0xD1, 0xDD, 0x6C, 0x8E, 0x1A, 0x8F, 0xA5, 0xEA,
0x54, 0x7C, 0x96, 0xB7, 0xC3, 0xFE, 0x55, 0x8E, 0x8D, 0x49, 0x5E, 0xFC, 0x64, 0xBB, 0xCF, 0x3E,
0xBD, 0x96, 0xEB, 0x69, 0xCD, 0xBF, 0xE0, 0x48, 0xF1, 0x62, 0x82, 0x10, 0xE5, 0x0C, 0x46, 0x57,
0xF2, 0x33, 0xDA, 0xD0, 0xC8, 0x63, 0xED, 0xC6, 0x1F, 0x94, 0x05, 0x96, 0x4A, 0x1A, 0x91, 0xD1,
0xF7, 0xEB, 0xCF, 0x8F, 0x52, 0xAE, 0x0D, 0x08, 0xD9, 0x3E, 0xA8, 0xA0, 0x51, 0xE9, 0xC1, 0x87,
0x74, 0xD5, 0xC9, 0xF7, 0x74, 0xAB, 0x2E, 0x53, 0xFB, 0xBB, 0x7A, 0xFB, 0x97, 0xE2, 0xF8, 0x1F,
0x26, 0x8F, 0xB3, 0xD2, 0xA0, 0xE0, 0x37, 0x5B, 0x28, 0x3B, 0x31, 0xE5, 0x0E, 0x57, 0x2D, 0x5A,
0xB8, 0xAD, 0x79, 0xAC, 0x5E, 0x20, 0x66, 0x1A, 0xA5, 0xB9, 0xA6, 0xB5, 0x39, 0xC1, 0xF5, 0x98,
0x43, 0xFF, 0xEE, 0xF9, 0xA7, 0xA7, 0xFD, 0xEE, 0xCA, 0x24, 0x3D, 0x80, 0x16, 0xC4, 0x17, 0x8F,
0x8A, 0xC1, 0x60, 0xA1, 0x0C, 0xAE, 0x5B, 0x43, 0x47, 0x91, 0x4B, 0xD5, 0x9A, 0x17, 0x5F, 0xF9,
0xD4, 0x87, 0xC1, 0xC2, 0x8C, 0xB7, 0xE7, 0xE2, 0x0F, 0x30, 0x19, 0x37, 0x86, 0xAC, 0xE0, 0xDC,
0x42, 0x03, 0xE6, 0x94, 0xA8, 0x9D, 0xAE, 0xFD, 0x0F, 0x24, 0x51, 0x94, 0xCE, 0x92, 0x08, 0xD1,
0xFC, 0x50, 0xF0, 0x03, 0x40, 0x7B, 0x88, 0x59, 0xED, 0x0E, 0xDD, 0xAC, 0xD2, 0x77, 0x82, 0x34,
0xDC, 0x06, 0x95, 0x02, 0xD8, 0x90, 0xF9, 0x2D, 0xEA, 0x37, 0xD5, 0x1A, 0x60, 0xD0, 0x67, 0x20,
0xD7, 0xD8, 0x42, 0x0B, 0x45, 0xAF, 0x82, 0x68, 0xDE, 0xDD, 0x66, 0x24, 0x37, 0x90, 0x29, 0x94,
0x19, 0x46, 0x19, 0x25, 0xB8, 0x80, 0xD7, 0xCB, 0xD4, 0x86, 0x28, 0x6A, 0x44, 0x70, 0x26, 0x23,
0x62, 0xA9, 0x9F, 0x86, 0x6F, 0xBF, 0xBA, 0x90, 0x70, 0xD2, 0x56, 0x77, 0x85, 0x78, 0xEF, 0xEA,
0x25, 0xA9, 0x17, 0xCE, 0x50, 0x72, 0x8C, 0x00, 0x3A, 0xAA, 0xE3, 0xDB, 0x63, 0x34, 0x9F, 0xF8,
0x06, 0x71, 0x01, 0xE2, 0x82, 0x20, 0xD4, 0xFE, 0x6F, 0xBD, 0xB1, 0x00, 0x05, 0x78, 0x30, 0x82,
0x05, 0x74, 0x30, 0x82, 0x04, 0x5C, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x10, 0x27, 0x66, 0xEE,
0x56, 0xEB, 0x49, 0xF3, 0x8E, 0xAB, 0xD7, 0x70, 0xA2, 0xFC, 0x84, 0xDE, 0x22, 0x30, 0x0D, 0x06,
0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0C, 0x05, 0x00, 0x30, 0x6F, 0x31, 0x0B,
0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x53, 0x45, 0x31, 0x14, 0x30, 0x12, 0x06,
0x03, 0x55, 0x04, 0x0A, 0x13, 0x0B, 0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x41,
0x42, 0x31, 0x26, 0x30, 0x24, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x13, 0x1D, 0x41, 0x64, 0x64, 0x54,
0x72, 0x75, 0x73, 0x74, 0x20, 0x45, 0x78, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x20, 0x54, 0x54,
0x50, 0x20, 0x4E, 0x65, 0x74, 0x77, 0x6F, 0x72, 0x6B, 0x31, 0x22, 0x30, 0x20, 0x06, 0x03, 0x55,
0x04, 0x03, 0x13, 0x19, 0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x45, 0x78, 0x74,
0x65, 0x72, 0x6E, 0x61, 0x6C, 0x20, 0x43, 0x41, 0x20, 0x52, 0x6F, 0x6F, 0x74, 0x30, 0x1E, 0x17,
0x0D, 0x30, 0x30, 0x30, 0x35, 0x33, 0x30, 0x31, 0x30, 0x34, 0x38, 0x33, 0x38, 0x5A, 0x17, 0x0D,
0x32, 0x30, 0x30, 0x35, 0x33, 0x30, 0x31, 0x30, 0x34, 0x38, 0x33, 0x38, 0x5A, 0x30, 0x81, 0x85,
0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x47, 0x42, 0x31, 0x1B, 0x30,
0x19, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x12, 0x47, 0x72, 0x65, 0x61, 0x74, 0x65, 0x72, 0x20,
0x4D, 0x61, 0x6E, 0x63, 0x68, 0x65, 0x73, 0x74, 0x65, 0x72, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03,
0x55, 0x04, 0x07, 0x13, 0x07, 0x53, 0x61, 0x6C, 0x66, 0x6F, 0x72, 0x64, 0x31, 0x1A, 0x30, 0x18,
0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x11, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x43, 0x41,
0x20, 0x4C, 0x69, 0x6D, 0x69, 0x74, 0x65, 0x64, 0x31, 0x2B, 0x30, 0x29, 0x06, 0x03, 0x55, 0x04,
0x03, 0x13, 0x22, 0x43, 0x4F, 0x4D, 0x4F, 0x44, 0x4F, 0x20, 0x52, 0x53, 0x41, 0x20, 0x43, 0x65,
0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x41, 0x75, 0x74, 0x68,
0x6F, 0x72, 0x69, 0x74, 0x79, 0x30, 0x82, 0x02, 0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48,
0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x02, 0x0F, 0x00, 0x30, 0x82, 0x02,
0x0A, 0x02, 0x82, 0x02, 0x01, 0x00, 0x91, 0xE8, 0x54, 0x92, 0xD2, 0x0A, 0x56, 0xB1, 0xAC, 0x0D,
0x24, 0xDD, 0xC5, 0xCF, 0x44, 0x67, 0x74, 0x99, 0x2B, 0x37, 0xA3, 0x7D, 0x23, 0x70, 0x00, 0x71,
0xBC, 0x53, 0xDF, 0xC4, 0xFA, 0x2A, 0x12, 0x8F, 0x4B, 0x7F, 0x10, 0x56, 0xBD, 0x9F, 0x70, 0x72,
0xB7, 0x61, 0x7F, 0xC9, 0x4B, 0x0F, 0x17, 0xA7, 0x3D, 0xE3, 0xB0, 0x04, 0x61, 0xEE, 0xFF, 0x11,
0x97, 0xC7, 0xF4, 0x86, 0x3E, 0x0A, 0xFA, 0x3E, 0x5C, 0xF9, 0x93, 0xE6, 0x34, 0x7A, 0xD9, 0x14,
0x6B, 0xE7, 0x9C, 0xB3, 0x85, 0xA0, 0x82, 0x7A, 0x76, 0xAF, 0x71, 0x90, 0xD7, 0xEC, 0xFD, 0x0D,
0xFA, 0x9C, 0x6C, 0xFA, 0xDF, 0xB0, 0x82, 0xF4, 0x14, 0x7E, 0xF9, 0xBE, 0xC4, 0xA6, 0x2F, 0x4F,
0x7F, 0x99, 0x7F, 0xB5, 0xFC, 0x67, 0x43, 0x72, 0xBD, 0x0C, 0x00, 0xD6, 0x89, 0xEB, 0x6B, 0x2C,
0xD3, 0xED, 0x8F, 0x98, 0x1C, 0x14, 0xAB, 0x7E, 0xE5, 0xE3, 0x6E, 0xFC, 0xD8, 0xA8, 0xE4, 0x92,
0x24, 0xDA, 0x43, 0x6B, 0x62, 0xB8, 0x55, 0xFD, 0xEA, 0xC1, 0xBC, 0x6C, 0xB6, 0x8B, 0xF3, 0x0E,
0x8D, 0x9A, 0xE4, 0x9B, 0x6C, 0x69, 0x99, 0xF8, 0x78, 0x48, 0x30, 0x45, 0xD5, 0xAD, 0xE1, 0x0D,
0x3C, 0x45, 0x60, 0xFC, 0x32, 0x96, 0x51, 0x27, 0xBC, 0x67, 0xC3, 0xCA, 0x2E, 0xB6, 0x6B, 0xEA,
0x46, 0xC7, 0xC7, 0x20, 0xA0, 0xB1, 0x1F, 0x65, 0xDE, 0x48, 0x08, 0xBA, 0xA4, 0x4E, 0xA9, 0xF2,
0x83, 0x46, 0x37, 0x84, 0xEB, 0xE8, 0xCC, 0x81, 0x48, 0x43, 0x67, 0x4E, 0x72, 0x2A, 0x9B, 0x5C,
0xBD, 0x4C, 0x1B, 0x28, 0x8A, 0x5C, 0x22, 0x7B, 0xB4, 0xAB, 0x98, 0xD9, 0xEE, 0xE0, 0x51, 0x83,
0xC3, 0x09, 0x46, 0x4E, 0x6D, 0x3E, 0x99, 0xFA, 0x95, 0x17, 0xDA, 0x7C, 0x33, 0x57, 0x41, 0x3C,
0x8D, 0x51, 0xED, 0x0B, 0xB6, 0x5C, 0xAF, 0x2C, 0x63, 0x1A, 0xDF, 0x57, 0xC8, 0x3F, 0xBC, 0xE9,
0x5D, 0xC4, 0x9B, 0xAF, 0x45, 0x99, 0xE2, 0xA3, 0x5A, 0x24, 0xB4, 0xBA, 0xA9, 0x56, 0x3D, 0xCF,
0x6F, 0xAA, 0xFF, 0x49, 0x58, 0xBE, 0xF0, 0xA8, 0xFF, 0xF4, 0xB8, 0xAD, 0xE9, 0x37, 0xFB, 0xBA,
0xB8, 0xF4, 0x0B, 0x3A, 0xF9, 0xE8, 0x43, 0x42, 0x1E, 0x89, 0xD8, 0x84, 0xCB, 0x13, 0xF1, 0xD9,
0xBB, 0xE1, 0x89, 0x60, 0xB8, 0x8C, 0x28, 0x56, 0xAC, 0x14, 0x1D, 0x9C, 0x0A, 0xE7, 0x71, 0xEB,
0xCF, 0x0E, 0xDD, 0x3D, 0xA9, 0x96, 0xA1, 0x48, 0xBD, 0x3C, 0xF7, 0xAF, 0xB5, 0x0D, 0x22, 0x4C,
0xC0, 0x11, 0x81, 0xEC, 0x56, 0x3B, 0xF6, 0xD3, 0xA2, 0xE2, 0x5B, 0xB7, 0xB2, 0x04, 0x22, 0x52,
0x95, 0x80, 0x93, 0x69, 0xE8, 0x8E, 0x4C, 0x65, 0xF1, 0x91, 0x03, 0x2D, 0x70, 0x74, 0x02, 0xEA,
0x8B, 0x67, 0x15, 0x29, 0x69, 0x52, 0x02, 0xBB, 0xD7, 0xDF, 0x50, 0x6A, 0x55, 0x46, 0xBF, 0xA0,
0xA3, 0x28, 0x61, 0x7F, 0x70, 0xD0, 0xC3, 0xA2, 0xAA, 0x2C, 0x21, 0xAA, 0x47, 0xCE, 0x28, 0x9C,
0x06, 0x45, 0x76, 0xBF, 0x82, 0x18, 0x27, 0xB4, 0xD5, 0xAE, 0xB4, 0xCB, 0x50, 0xE6, 0x6B, 0xF4,
0x4C, 0x86, 0x71, 0x30, 0xE9, 0xA6, 0xDF, 0x16, 0x86, 0xE0, 0xD8, 0xFF, 0x40, 0xDD, 0xFB, 0xD0,
0x42, 0x88, 0x7F, 0xA3, 0x33, 0x3A, 0x2E, 0x5C, 0x1E, 0x41, 0x11, 0x81, 0x63, 0xCE, 0x18, 0x71,
0x6B, 0x2B, 0xEC, 0xA6, 0x8A, 0xB7, 0x31, 0x5C, 0x3A, 0x6A, 0x47, 0xE0, 0xC3, 0x79, 0x59, 0xD6,
0x20, 0x1A, 0xAF, 0xF2, 0x6A, 0x98, 0xAA, 0x72, 0xBC, 0x57, 0x4A, 0xD2, 0x4B, 0x9D, 0xBB, 0x10,
0xFC, 0xB0, 0x4C, 0x41, 0xE5, 0xED, 0x1D, 0x3D, 0x5E, 0x28, 0x9D, 0x9C, 0xCC, 0xBF, 0xB3, 0x51,
0xDA, 0xA7, 0x47, 0xE5, 0x84, 0x53, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x81, 0xF4, 0x30, 0x81,
0xF1, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0xAD, 0xBD,
0x98, 0x7A, 0x34, 0xB4, 0x26, 0xF7, 0xFA, 0xC4, 0x26, 0x54, 0xEF, 0x03, 0xBD, 0xE0, 0x24, 0xCB,
0x54, 0x1A, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0xBB, 0xAF, 0x7E,
0x02, 0x3D, 0xFA, 0xA6, 0xF1, 0x3C, 0x84, 0x8E, 0xAD, 0xEE, 0x38, 0x98, 0xEC, 0xD9, 0x32, 0x32,
0xD4, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03, 0x02, 0x01,
0x86, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x05, 0x30, 0x03, 0x01,
0x01, 0xFF, 0x30, 0x11, 0x06, 0x03, 0x55, 0x1D, 0x20, 0x04, 0x0A, 0x30, 0x08, 0x30, 0x06, 0x06,
0x04, 0x55, 0x1D, 0x20, 0x00, 0x30, 0x44, 0x06, 0x03, 0x55, 0x1D, 0x1F, 0x04, 0x3D, 0x30, 0x3B,
0x30, 0x39, 0xA0, 0x37, 0xA0, 0x35, 0x86, 0x33, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x63,
0x72, 0x6C, 0x2E, 0x75, 0x73, 0x65, 0x72, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2E, 0x63, 0x6F, 0x6D,
0x2F, 0x41, 0x64, 0x64, 0x54, 0x72, 0x75, 0x73, 0x74, 0x45, 0x78, 0x74, 0x65, 0x72, 0x6E, 0x61,
0x6C, 0x43, 0x41, 0x52, 0x6F, 0x6F, 0x74, 0x2E, 0x63, 0x72, 0x6C, 0x30, 0x35, 0x06, 0x08, 0x2B,
0x06, 0x01, 0x05, 0x05, 0x07, 0x01, 0x01, 0x04, 0x29, 0x30, 0x27, 0x30, 0x25, 0x06, 0x08, 0x2B,
0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x86, 0x19, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F,
0x6F, 0x63, 0x73, 0x70, 0x2E, 0x75, 0x73, 0x65, 0x72, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2E, 0x63,
0x6F, 0x6D, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0C, 0x05,
0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x64, 0xBF, 0x83, 0xF1, 0x5F, 0x9A, 0x85, 0xD0, 0xCD, 0xB8,
0xA1, 0x29, 0x57, 0x0D, 0xE8, 0x5A, 0xF7, 0xD1, 0xE9, 0x3E, 0xF2, 0x76, 0x04, 0x6E, 0xF1, 0x52,
0x70, 0xBB, 0x1E, 0x3C, 0xFF, 0x4D, 0x0D, 0x74, 0x6A, 0xCC, 0x81, 0x82, 0x25, 0xD3, 0xC3, 0xA0,
0x2A, 0x5D, 0x4C, 0xF5, 0xBA, 0x8B, 0xA1, 0x6D, 0xC4, 0x54, 0x09, 0x75, 0xC7, 0xE3, 0x27, 0x0E,
0x5D, 0x84, 0x79, 0x37, 0x40, 0x13, 0x77, 0xF5, 0xB4, 0xAC, 0x1C, 0xD0, 0x3B, 0xAB, 0x17, 0x12,
0xD6, 0xEF, 0x34, 0x18, 0x7E, 0x2B, 0xE9, 0x79, 0xD3, 0xAB, 0x57, 0x45, 0x0C, 0xAF, 0x28, 0xFA,
0xD0, 0xDB, 0xE5, 0x50, 0x95, 0x88, 0xBB, 0xDF, 0x85, 0x57, 0x69, 0x7D, 0x92, 0xD8, 0x52, 0xCA,
0x73, 0x81, 0xBF, 0x1C, 0xF3, 0xE6, 0xB8, 0x6E, 0x66, 0x11, 0x05, 0xB3, 0x1E, 0x94, 0x2D, 0x7F,
0x91, 0x95, 0x92, 0x59, 0xF1, 0x4C, 0xCE, 0xA3, 0x91, 0x71, 0x4C, 0x7C, 0x47, 0x0C, 0x3B, 0x0B,
0x19, 0xF6, 0xA1, 0xB1, 0x6C, 0x86, 0x3E, 0x5C, 0xAA, 0xC4, 0x2E, 0x82, 0xCB, 0xF9, 0x07, 0x96,
0xBA, 0x48, 0x4D, 0x90, 0xF2, 0x94, 0xC8, 0xA9, 0x73, 0xA2, 0xEB, 0x06, 0x7B, 0x23, 0x9D, 0xDE,
0xA2, 0xF3, 0x4D, 0x55, 0x9F, 0x7A, 0x61, 0x45, 0x98, 0x18, 0x68, 0xC7, 0x5E, 0x40, 0x6B, 0x23,
0xF5, 0x79, 0x7A, 0xEF, 0x8C, 0xB5, 0x6B, 0x8B, 0xB7, 0x6F, 0x46, 0xF4, 0x7B, 0xF1, 0x3D, 0x4B,
0x04, 0xD8, 0x93, 0x80, 0x59, 0x5A, 0xE0, 0x41, 0x24, 0x1D, 0xB2, 0x8F, 0x15, 0x60, 0x58, 0x47,
0xDB, 0xEF, 0x6E, 0x46, 0xFD, 0x15, 0xF5, 0xD9, 0x5F, 0x9A, 0xB3, 0xDB, 0xD8, 0xB8, 0xE4, 0x40,
0xB3, 0xCD, 0x97, 0x39, 0xAE, 0x85, 0xBB, 0x1D, 0x8E, 0xBC, 0xDC, 0x87, 0x9B, 0xD1, 0xA6, 0xEF,
0xF1, 0x3B, 0x6F, 0x10, 0x38, 0x6F, 0x16, 0x03, 0x03, 0x02, 0x0F, 0x0C, 0x00, 0x02, 0x0B, 0x00,
0x80, 0xBB, 0xBC, 0x2D, 0xCA, 0xD8, 0x46, 0x74, 0x90, 0x7C, 0x43, 0xFC, 0xF5, 0x80, 0xE9, 0xCF,
0xDB, 0xD9, 0x58, 0xA3, 0xF5, 0x68, 0xB4, 0x2D, 0x4B, 0x08, 0xEE, 0xD4, 0xEB, 0x0F, 0xB3, 0x50,
0x4C, 0x6C, 0x03, 0x02, 0x76, 0xE7, 0x10, 0x80, 0x0C, 0x5C, 0xCB, 0xBA, 0xA8, 0x92, 0x26, 0x14,
0xC5, 0xBE, 0xEC, 0xA5, 0x65, 0xA5, 0xFD, 0xF1, 0xD2, 0x87, 0xA2, 0xBC, 0x04, 0x9B, 0xE6, 0x77,
0x80, 0x60, 0xE9, 0x1A, 0x92, 0xA7, 0x57, 0xE3, 0x04, 0x8F, 0x68, 0xB0, 0x76, 0xF7, 0xD3, 0x6C,
0xC8, 0xF2, 0x9B, 0xA5, 0xDF, 0x81, 0xDC, 0x2C, 0xA7, 0x25, 0xEC, 0xE6, 0x62, 0x70, 0xCC, 0x9A,
0x50, 0x35, 0xD8, 0xCE, 0xCE, 0xEF, 0x9E, 0xA0, 0x27, 0x4A, 0x63, 0xAB, 0x1E, 0x58, 0xFA, 0xFD,
0x49, 0x88, 0xD0, 0xF6, 0x5D, 0x14, 0x67, 0x57, 0xDA, 0x07, 0x1D, 0xF0, 0x45, 0xCF, 0xE1, 0x6B,
0x9B, 0x00, 0x01, 0x02, 0x00, 0x80, 0x55, 0xFB, 0xB6, 0x92, 0x5A, 0x0C, 0x93, 0x56, 0xE8, 0x1C,
0xCD, 0x23, 0xFC, 0xB9, 0xBE, 0x98, 0x2D, 0x01, 0x4F, 0x35, 0xCE, 0x37, 0xD0, 0xF8, 0xA0, 0x0C,
0x42, 0x3B, 0x27, 0x25, 0x10, 0x04, 0x46, 0x02, 0x19, 0x1F, 0xDC, 0xDA, 0x08, 0x33, 0x42, 0x63,
0x5F, 0x3C, 0x82, 0x1F, 0xFA, 0x46, 0x9E, 0x34, 0xCB, 0x30, 0xED, 0x55, 0x11, 0xD7, 0x00, 0x0C,
0x76, 0x0A, 0x48, 0x0C, 0x1D, 0x7A, 0x13, 0x3D, 0xC6, 0x41, 0xD5, 0x7B, 0xD5, 0x2A, 0xE0, 0xA3,
0xB5, 0xAB, 0x0E, 0xBC, 0xD3, 0x17, 0x14, 0xFD, 0x21, 0x8C, 0x78, 0xB1, 0x4D, 0xF2, 0x5A, 0x44,
0x89, 0x4E, 0x82, 0x3D, 0x8B, 0xAC, 0x5D, 0x49, 0xB2, 0x19, 0x21, 0x67, 0xCA, 0x70, 0xC7, 0x8D,
0x35, 0x89, 0xD1, 0x9D, 0x34, 0xD4, 0x7A, 0xAF, 0x1D, 0x9D, 0x86, 0x0F, 0xEF, 0x5A, 0x4F, 0x9E,
0xDA, 0xF5, 0x02, 0x7E, 0x22, 0x67, 0x01, 0x01, 0x01, 0x00, 0x7B, 0x59, 0x4A, 0x29, 0xF9, 0x9F,
0x53, 0x41, 0x8B, 0xE6, 0x79, 0xE9, 0xF5, 0x73, 0x7C, 0xC8, 0xE9, 0x40, 0x1F, 0x39, 0x08, 0x06,
0x84, 0x3D, 0x80, 0x6B, 0xF0, 0x0C, 0x54, 0xC2, 0x18, 0xD6, 0xAD, 0x53, 0xFD, 0x7C, 0xB3, 0x8B,
0xCE, 0x70, 0x4F, 0xCF, 0xFC, 0xEF, 0xC8, 0x2D, 0xE4, 0xD0, 0xC2, 0x7B, 0x80, 0x57, 0xA2, 0x5F,
0x7F, 0x36, 0xB0, 0xBC, 0x16, 0xB7, 0xC1, 0xC2, 0x12, 0x96, 0x94, 0x88, 0x9D, 0x68, 0xD3, 0xDF,
0xB1, 0x3C, 0xA0, 0x93, 0x3A, 0x4D, 0x90, 0x66, 0x3F, 0xB0, 0x89, 0xE7, 0x21, 0x58, 0xF0, 0x39,
0x8B, 0x83, 0x32, 0x93, 0xCB, 0xA9, 0x57, 0x2B, 0x0A, 0x02, 0x5E, 0xF9, 0xE6, 0x10, 0x9A, 0x7F,
0x2A, 0x23, 0x3D, 0x5C, 0x3F, 0x38, 0x3A, 0x40, 0x0C, 0x1B, 0x7C, 0x0D, 0x5D, 0x7C, 0xF1, 0xB2,
0x88, 0x71, 0xA2, 0x57, 0x02, 0x94, 0x12, 0x45, 0x60, 0xEC, 0xC2, 0xB2, 0xCB, 0x31, 0xF2, 0xF8,
0x1B, 0xDD, 0xEA, 0xF6, 0x40, 0xF6, 0x07, 0xEA, 0xC2, 0x71, 0xA6, 0x07, 0xC8, 0x45, 0xE4, 0xE2,
0x1D, 0x49, 0x53, 0x40, 0x5B, 0x57, 0x68, 0xA4, 0x81, 0x1C, 0xCB, 0x33, 0x95, 0x12, 0x0F, 0x4D,
0xAD, 0x9B, 0x70, 0x53, 0xDC, 0x8B, 0xE6, 0x24, 0x0D, 0x39, 0x83, 0x0E, 0x17, 0xFB, 0x13, 0xF2,
0x45, 0x2C, 0x52, 0x44, 0xA3, 0x22, 0x0D, 0x32, 0xDD, 0x21, 0x05, 0x2B, 0xA1, 0xAC, 0xC4, 0x84,
0xD7, 0x18, 0x2F, 0xA2, 0xB4, 0xF5, 0x4E, 0xED, 0x72, 0x56, 0xB7, 0x87, 0x81, 0xF8, 0x51, 0x30,
0x01, 0xBE, 0x17, 0x31, 0xEE, 0x21, 0xBA, 0x16, 0xAF, 0xEB, 0x0A, 0x54, 0x69, 0x84, 0xB3, 0xDC,
0xCA, 0x04, 0xBB, 0x49, 0x84, 0x4C, 0x0F, 0xCE, 0x20, 0xE2, 0x71, 0xF3, 0x43, 0x26, 0x1D, 0xA4,
0xAF, 0xAE, 0x3F, 0x9E, 0xA3, 0x44, 0xDC, 0xAE, 0xB7, 0x5B, 0x16, 0x03, 0x03, 0x00, 0x04, 0x0E,
0x00, 0x00, 0x00};

static void server(int sd)
{
	char buf[1024];
	int ret;
	struct pollfd pfd;

	/* send a TLS 1.x hello with duplicate extensions */
	
	ret = send(sd, tls1_hello, sizeof(tls1_hello), 0);
	if (ret < 0)
		fail("error sending hello\n");

	pfd.fd = sd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	do {
		ret = poll(&pfd, 1, 10000);
	} while (ret == -1 && errno == EINTR);

	if (ret == -1 || ret == 0) {
		fail("timeout waiting for reply\n");
	}

	success("sent hello\n");
	ret = recv(sd, buf, sizeof(buf), 0);
	if (ret < 0)
		fail("error receiving alert\n");

	success("all ok\n");

	close(sd);
}

static void client(int sd)
{
	gnutls_certificate_credentials_t x509_cred;
	gnutls_session_t session;
	int ret;

	/* this must be called once in the program
	 */
	global_init();

	gnutls_global_set_log_function(tls_log_func);
	if (debug)
		gnutls_global_set_log_level(6);

	gnutls_certificate_allocate_credentials(&x509_cred);
	gnutls_certificate_set_x509_trust_mem(x509_cred, &ca3_cert,
					      GNUTLS_X509_FMT_PEM);

	gnutls_certificate_set_x509_key_mem(x509_cred, &server_ca3_localhost_cert,
					    &server_ca3_key,
					    GNUTLS_X509_FMT_PEM);

	gnutls_init(&session, GNUTLS_CLIENT);

	/* avoid calling all the priority functions, since the defaults
	 * are adequate.
	 */
	gnutls_priority_set_direct(session, "NORMAL:-VERS-ALL:+VERS-TLS1.2:+DHE-RSA:+AES-128-GCM", NULL);
	gnutls_handshake_set_timeout(session, 20 * 1000);
	gnutls_server_name_set(session, GNUTLS_NAME_DNS, "localhost", strlen("localhost"));

	gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, x509_cred);

	gnutls_transport_set_int(session, sd);
	do {
		ret = gnutls_handshake(session);
	} while(ret == GNUTLS_E_INTERRUPTED || ret == GNUTLS_E_AGAIN);

	if (ret != GNUTLS_E_UNSUPPORTED_SIGNATURE_ALGORITHM) {
		fail("server: Handshake returned unexpectedly: %s\n", gnutls_strerror(ret));
	}

	gnutls_alert_send_appropriate(session, ret);
	
	close(sd);
	gnutls_deinit(session);

	gnutls_certificate_free_credentials(x509_cred);

	gnutls_global_deinit();

	if (debug)
		success("server: finished\n");
}


void doit(void)
{
	int sockets[2];
	int err;

	signal(SIGPIPE, SIG_IGN);

	err = socketpair(AF_UNIX, SOCK_STREAM, 0, sockets);
	if (err == -1) {
		perror("socketpair");
		fail("socketpair failed\n");
		return;
	}

	child = fork();
	if (child < 0) {
		perror("fork");
		fail("fork");
		return;
	}

	if (child) {
		int status;

		client(sockets[1]);
		wait(&status);
		check_wait_status(status);
	} else {
		server(sockets[0]);
		_exit(0);
	}
}

#endif				/* _WIN32 */
