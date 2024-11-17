#include "org_libdivecomputer_Context.h"
#include "exception.h"

#include <libdivecomputer/context.h>

typedef struct jni_context_t {
	JNIEnv *env;
	jobject obj;
	jclass cls;
	jmethodID method;
} jni_context_t;

static void
log_cb (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *message, void *userdata)
{
	jni_context_t *jni = userdata;

	(*jni->env)->CallVoidMethod(jni->env, jni->obj, jni->method,
		loglevel,
		(*jni->env)->NewStringUTF(jni->env, file),
		line,
		(*jni->env)->NewStringUTF(jni->env, function),
		(*jni->env)->NewStringUTF(jni->env, message));

}

JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Context_New
  (JNIEnv *env, jobject obj)
{
	dc_context_t *context = NULL;

	DC_EXCEPTION_THROW(dc_context_new (&context));

	return (jlong) context;
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Context_Free
  (JNIEnv *env, jobject obj, jlong handle)
{
	DC_EXCEPTION_THROW(dc_context_free ((dc_context_t *) handle));
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Context_SetLogLevel
  (JNIEnv *env, jobject obj, jlong handle, jint loglevel)
{
	DC_EXCEPTION_THROW(dc_context_set_loglevel ((dc_context_t *) handle, loglevel));
}

JNIEXPORT void JNICALL Java_org_libdivecomputer_Context_SetLogFunc
  (JNIEnv *env, jobject obj, jlong handle, jobject logfunc)
{
	static jni_context_t jni = {0}; // FIXME: Not thread-safe.

	if (logfunc) {
		jni.env = env;
		jni.obj = (*env)->NewGlobalRef(env, logfunc); // FIXME: Memory leak.
		jni.cls = (*env)->GetObjectClass(env, logfunc);
		jni.method = (*env)->GetMethodID(env, jni.cls, "Log", "(ILjava/lang/String;ILjava/lang/String;Ljava/lang/String;)V");

		DC_EXCEPTION_THROW(dc_context_set_logfunc ((dc_context_t *) handle, log_cb, &jni));
	} else {
		DC_EXCEPTION_THROW(dc_context_set_logfunc ((dc_context_t *) handle, NULL, NULL));
	}
}
