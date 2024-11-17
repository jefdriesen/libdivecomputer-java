/*
 * libdivecomputer
 *
 * Copyright (C) 2024 Jef Driesen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include "exception.h"

void
dc_exception_throw (JNIEnv *env, dc_status_t status)
{
	const char *exception = "java/lang/Exception";
	const char *message = "Unknown error";

	if (status == DC_STATUS_SUCCESS || status == DC_STATUS_DONE) {
		return;
	}

	switch (status) {
	case DC_STATUS_UNSUPPORTED:
		exception = "java/lang/UnsupportedOperationException";
		message = "Unsupported operation";
		break;
	case DC_STATUS_INVALIDARGS:
		exception = "java/lang/IllegalArgumentException";
		message = "Invalid arguments";
		break;
	case DC_STATUS_NOMEMORY:
		message = "Out of memory";
		break;
	case DC_STATUS_NODEVICE:
		message = "No device found";
		break;
	case DC_STATUS_NOACCESS:
		message = "Access denied";
		break;
	case DC_STATUS_IO:
		message = "Input/output error";
		break;
	case DC_STATUS_TIMEOUT:
		message = "Timeout";
		break;
	case DC_STATUS_PROTOCOL:
		message = "Protocol error";
		break;
	case DC_STATUS_DATAFORMAT:
		message = "Data format error";
		break;
	case DC_STATUS_CANCELLED:
		message = "Cancelled";
		break;
	}

	jclass cls = (*env)->FindClass(env, exception);
	(*env)->ThrowNew(env, cls, message);
}
