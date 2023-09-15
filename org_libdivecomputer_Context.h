/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_libdivecomputer_Context */

#ifndef _Included_org_libdivecomputer_Context
#define _Included_org_libdivecomputer_Context
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_libdivecomputer_Context
 * Method:    New
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Context_New
  (JNIEnv *, jobject);

/*
 * Class:     org_libdivecomputer_Context
 * Method:    SetLogLevel
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Context_SetLogLevel
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     org_libdivecomputer_Context
 * Method:    SetLogFunc
 * Signature: (JLorg/libdivecomputer/Context/LogFunc;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Context_SetLogFunc
  (JNIEnv *, jobject, jlong, jobject);

#ifdef __cplusplus
}
#endif
#endif