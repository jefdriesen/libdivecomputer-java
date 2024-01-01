/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_libdivecomputer_Device */

#ifndef _Included_org_libdivecomputer_Device
#define _Included_org_libdivecomputer_Device
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_libdivecomputer_Device
 * Method:    Open
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_org_libdivecomputer_Device_Open
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     org_libdivecomputer_Device
 * Method:    Foreach
 * Signature: (JLorg/libdivecomputer/Device/Callback;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_Foreach
  (JNIEnv *, jobject, jlong, jobject);

/*
 * Class:     org_libdivecomputer_Device
 * Method:    SetFingerprint
 * Signature: (J[B)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_SetFingerprint
  (JNIEnv *, jobject, jlong, jbyteArray);

/*
 * Class:     org_libdivecomputer_Device
 * Method:    SetEvents
 * Signature: (JLorg/libdivecomputer/Device/Events;)V
 */
JNIEXPORT void JNICALL Java_org_libdivecomputer_Device_SetEvents
  (JNIEnv *, jobject, jlong, jobject);

#ifdef __cplusplus
}
#endif
#endif
