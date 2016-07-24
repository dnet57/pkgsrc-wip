$NetBSD$

--- third_party/WebKit/Source/platform/heap/StackFrameDepth.cpp.orig	2016-06-24 01:02:46.000000000 +0000
+++ third_party/WebKit/Source/platform/heap/StackFrameDepth.cpp
@@ -6,6 +6,10 @@
 
 #include "public/platform/Platform.h"
 
+#if OS(FREEBSD)
+#include <pthread_np.h>
+#endif
+
 #if OS(WIN)
 #include <stddef.h>
 #include <windows.h>
@@ -73,7 +77,7 @@ size_t StackFrameDepth::getUnderestimate
     // FIXME: On Mac OSX and Linux, this method cannot estimate stack size
     // correctly for the main thread.
 
-#if defined(__GLIBC__) || OS(ANDROID) || OS(FREEBSD)
+#if defined(__GLIBC__) || OS(ANDROID) || OS(FREEBSD) || OS(NETBSD)
     // pthread_getattr_np() can fail if the thread is not invoked by
     // pthread_create() (e.g., the main thread of webkit_unit_tests).
     // If so, a conservative size estimate is returned.
@@ -135,7 +139,7 @@ size_t StackFrameDepth::getUnderestimate
 
 void* StackFrameDepth::getStackStart()
 {
-#if defined(__GLIBC__) || OS(ANDROID) || OS(FREEBSD)
+#if defined(__GLIBC__) || OS(ANDROID) || OS(FREEBSD) || OS(NETBSD)
     pthread_attr_t attr;
     int error;
 #if OS(FREEBSD)