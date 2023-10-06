#include <libdivecomputer/parser.h>

#include "org_libdivecomputer_Parser.h"

typedef struct jni_parser_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID time;
	jmethodID depth;
	jmethodID pressure;
	jmethodID temperature;
	jmethodID event;
	jmethodID rbt;
	jmethodID heartbeat;
	jmethodID bearing;
	jmethodID vendor;
	jmethodID setpoint;
	jmethodID ppo2;
	jmethodID cns;
	jmethodID deco;
	jmethodID gasmix;
} jni_parser_t;

static void
sample_cb (dc_sample_type_t type, const dc_sample_value_t *value, void *userdata)
{
	jni_parser_t *jni = userdata;

	switch (type) {
	case DC_SAMPLE_TIME:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->time,
			value->time);
		break;
	case DC_SAMPLE_DEPTH:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->depth,
			value->depth);
		break;
	case DC_SAMPLE_PRESSURE:
			(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->pressure,
			value->pressure.tank,
			value->pressure.value);
		break;
	case DC_SAMPLE_TEMPERATURE:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->temperature,
			value->temperature);
		break;
	case DC_SAMPLE_EVENT:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->event,
			value->event.type,
			value->event.time,
			value->event.flags,
			value->event.value);
		break;
	case DC_SAMPLE_RBT:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->rbt,
			value->rbt);
		break;
	case DC_SAMPLE_HEARTBEAT:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->heartbeat,
			value->heartbeat);
		break;
	case DC_SAMPLE_BEARING:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->bearing,
			value->bearing);
		break;
	case DC_SAMPLE_VENDOR:
#if 0
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->vendor,
			value->vendor.type,
			value->vendor.data,
			value->vendor.size);
#endif
		break;
	case DC_SAMPLE_SETPOINT:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->setpoint,
			value->setpoint);
		break;
	case DC_SAMPLE_PPO2:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->ppo2,
			value->ppo2.sensor,
			value->ppo2.value);
		break;
	case DC_SAMPLE_CNS:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->cns,
			value->cns);
		break;
	case DC_SAMPLE_DECO:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->deco,
			value->deco.type,
			value->deco.time,
			value->deco.depth,
			value->deco.tts);
		break;
	case DC_SAMPLE_GASMIX:
		(*jni->env)->CallVoidMethod(jni->env, jni->obj,
			jni->gasmix,
			value->gasmix);
		break;
	default:
		break;
	}
}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Parser_New
  (JNIEnv *env, jobject obj, jlong device, jbyteArray data)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_parser_t *parser = NULL;

	// Get the pointer and length.
	jboolean isCopy = 0;
	jint len = (*env)->GetArrayLength(env, data);
	jbyte *buf = (*env)->GetByteArrayElements(env, data, &isCopy);

	status = dc_parser_new (&parser,
		(dc_device_t *) device,
		buf,
		len);

	// Release the pointer.
	(*env)->ReleaseByteArrayElements(env, data, buf, JNI_ABORT);

	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) parser;
}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Parser_New2
  (JNIEnv *env, jobject obj, jlong context, jlong descriptor, jbyteArray data)
{
	dc_status_t status = DC_STATUS_SUCCESS;
	dc_parser_t *parser = NULL;

	// Get the pointer and length.
	jboolean isCopy = 0;
	jint len = (*env)->GetArrayLength(env, data);
	jbyte *buf = (*env)->GetByteArrayElements(env, data, &isCopy);

	status = dc_parser_new2 (&parser,
		(dc_context_t *) context,
		(dc_descriptor_t *) descriptor,
		buf,
		len);

	// Release the pointer.
	(*env)->ReleaseByteArrayElements(env, data, buf, JNI_ABORT);

	if (status != DC_STATUS_SUCCESS) {
		return 0;
	}

	return (jlong) parser;
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_Foreach
  (JNIEnv *env, jobject obj, jlong handle, jobject callback)
{
	jni_parser_t jni = {0};
	jni.env = env;
	jni.obj = (*env)->NewGlobalRef(env, callback);
	jni.cls = (*env)->GetObjectClass(env, callback);
	jni.time        = (*env)->GetMethodID(env, jni.cls, "Time", "(I)V");
	jni.depth       = (*env)->GetMethodID(env, jni.cls, "Depth", "(D)V");
	jni.pressure    = (*env)->GetMethodID(env, jni.cls, "Pressure", "(ID)V");
	jni.temperature = (*env)->GetMethodID(env, jni.cls, "Temperature", "(D)V");
	jni.event       = (*env)->GetMethodID(env, jni.cls, "Event", "(IIII)V");
	jni.rbt         = (*env)->GetMethodID(env, jni.cls, "Rbt", "(I)V");
	jni.heartbeat   = (*env)->GetMethodID(env, jni.cls, "Heartbeat", "(I)V");
	jni.bearing     = (*env)->GetMethodID(env, jni.cls, "Bearing", "(I)V");
	jni.vendor      = (*env)->GetMethodID(env, jni.cls, "Vendor", "(I[B)V");
	jni.setpoint    = (*env)->GetMethodID(env, jni.cls, "Setpoint", "(D)V");
	jni.ppo2        = (*env)->GetMethodID(env, jni.cls, "Ppo2", "(ID)V");
	jni.cns         = (*env)->GetMethodID(env, jni.cls, "Cns", "(D)V");
	jni.deco        = (*env)->GetMethodID(env, jni.cls, "Deco", "(IIDI)V");
	jni.gasmix      = (*env)->GetMethodID(env, jni.cls, "Gasmix", "(I)V");

	dc_parser_samples_foreach((dc_parser_t *) handle, sample_cb, &jni);

	(*env)->DeleteGlobalRef(env, jni.obj);
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Parser_GetDatetime
  (JNIEnv *env, jobject obj, jlong handle, jobject value)
{
	dc_datetime_t datetime = {0};
	dc_parser_get_datetime ((dc_parser_t *) handle, &datetime);

	jclass cls = (*env)->GetObjectClass(env, value);
	jfieldID fid_year = (*env)->GetFieldID(env, cls, "year", "I");
	jfieldID fid_month = (*env)->GetFieldID(env, cls, "month", "I");
	jfieldID fid_day = (*env)->GetFieldID(env, cls, "day", "I");
	jfieldID fid_hour = (*env)->GetFieldID(env, cls, "hour", "I");
	jfieldID fid_minute = (*env)->GetFieldID(env, cls, "minute", "I");
	jfieldID fid_second = (*env)->GetFieldID(env, cls, "second", "I");
	jfieldID fid_timezone = (*env)->GetFieldID(env, cls, "timezone", "I");

	(*env)->SetIntField(env, value, fid_year, datetime.year);
	(*env)->SetIntField(env, value, fid_month, datetime.month);
	(*env)->SetIntField(env, value, fid_day, datetime.day);
	(*env)->SetIntField(env, value, fid_hour, datetime.hour);
	(*env)->SetIntField(env, value, fid_minute, datetime.minute);
	(*env)->SetIntField(env, value, fid_second, datetime.second);
	(*env)->SetIntField(env, value, fid_timezone, datetime.timezone);
}
