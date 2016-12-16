$NetBSD$

--- source/Plugins/Process/Linux/NativeProcessLinux.cpp.orig	2016-12-14 14:10:24.000000000 +0000
+++ source/Plugins/Process/Linux/NativeProcessLinux.cpp
@@ -50,7 +50,9 @@
 // System includes - They have to be included after framework includes because
 // they define some
 // macros which collide with variable names in other modules
+#if !defined(__NetBSD__)
 #include <linux/unistd.h>
+#endif
 #include <sys/socket.h>
 
 #include <sys/syscall.h>
@@ -74,6 +76,7 @@ using namespace llvm;
 
 // Private bits we only need internally.
 
+#if !defined(__NetBSD__)
 static bool ProcessVmReadvSupported() {
   static bool is_supported;
   static std::once_flag flag;
@@ -107,6 +110,7 @@ static bool ProcessVmReadvSupported() {
 
   return is_supported;
 }
+#endif
 
 namespace {
 void MaybeLogLaunchInfo(const ProcessLaunchInfo &info) {
@@ -155,6 +159,7 @@ void PtraceDisplayBytes(int &req, void *
 
   if (verbose_log) {
     switch (req) {
+#if !defined(__NetBSD__)
     case PTRACE_POKETEXT: {
       DisplayBytes(buf, &data, 8);
       verbose_log->Printf("PTRACE_POKETEXT %s", buf.GetData());
@@ -170,6 +175,7 @@ void PtraceDisplayBytes(int &req, void *
       verbose_log->Printf("PTRACE_POKEUSER %s", buf.GetData());
       break;
     }
+#endif
     case PTRACE_SETREGS: {
       DisplayBytes(buf, data, data_size);
       verbose_log->Printf("PTRACE_SETREGS %s", buf.GetData());
@@ -180,6 +186,7 @@ void PtraceDisplayBytes(int &req, void *
       verbose_log->Printf("PTRACE_SETFPREGS %s", buf.GetData());
       break;
     }
+#if !defined(__NetBSD__)
     case PTRACE_SETSIGINFO: {
       DisplayBytes(buf, data, sizeof(siginfo_t));
       verbose_log->Printf("PTRACE_SETSIGINFO %s", buf.GetData());
@@ -191,6 +198,7 @@ void PtraceDisplayBytes(int &req, void *
       verbose_log->Printf("PTRACE_SETREGSET %s", buf.GetData());
       break;
     }
+#endif
     default: {}
     }
   }
@@ -224,6 +232,7 @@ static Error EnsureFDFlags(int fd, int f
 // Public Static Methods
 // -----------------------------------------------------------------------------
 
+#if !defined(__NetBSD__)
 Error NativeProcessProtocol::Launch(
     ProcessLaunchInfo &launch_info,
     NativeProcessProtocol::NativeDelegate &native_delegate, MainLoop &mainloop,
@@ -266,7 +275,9 @@ Error NativeProcessProtocol::Launch(
 
   return error;
 }
+#endif
 
+#if !defined(__NetBSD__)
 Error NativeProcessProtocol::Attach(
     lldb::pid_t pid, NativeProcessProtocol::NativeDelegate &native_delegate,
     MainLoop &mainloop, NativeProcessProtocolSP &native_process_sp) {
@@ -295,6 +306,7 @@ Error NativeProcessProtocol::Attach(
   native_process_sp = native_process_linux_sp;
   return error;
 }
+#endif
 
 // -----------------------------------------------------------------------------
 // Public Instance Methods
@@ -346,7 +358,11 @@ Error NativeProcessLinux::LaunchInferior
   MaybeLogLaunchInfo(launch_info);
 
   ::pid_t pid =
+#if !defined(__NetBSD__)
       ProcessLauncherLinux().LaunchProcess(launch_info, error).GetProcessId();
+#else
+      0;
+#endif
   if (error.Fail())
     return error;
 
@@ -458,7 +474,9 @@ Error NativeProcessLinux::LaunchInferior
 
         // Attach to the requested process.
         // An attach will cause the thread to stop with a SIGSTOP.
+#if !defined(__NetBSD__)
         error = PtraceWrapper(PTRACE_ATTACH, tid);
+#endif
         if (error.Fail()) {
           // No such thread. The thread may have exited.
           // More error handling may be needed.
@@ -528,6 +546,7 @@ Error NativeProcessLinux::SetDefaultPtra
 
   // Have the child raise an event on exit.  This is used to keep the child in
   // limbo until it is destroyed.
+#if !defined(__NetBSD__)
   ptrace_opts |= PTRACE_O_TRACEEXIT;
 
   // Have the tracer trace threads which spawn in the inferior process.
@@ -540,6 +559,7 @@ Error NativeProcessLinux::SetDefaultPtra
   ptrace_opts |= PTRACE_O_TRACEEXEC;
 
   return PtraceWrapper(PTRACE_SETOPTIONS, pid, nullptr, (void *)ptrace_opts);
+#endif
 }
 
 static ExitType convert_pid_status_to_exit_type(int status) {
@@ -835,6 +855,7 @@ void NativeProcessLinux::MonitorSIGTRAP(
 
   assert(info.si_signo == SIGTRAP && "Unexpected child signal!");
 
+#if !defined(__NetBSD__)
   switch (info.si_code) {
   // TODO: these two cases are required if we want to support tracing of the
   // inferiors' children.  We'd need this to debug a monitor.
@@ -1038,6 +1059,7 @@ void NativeProcessLinux::MonitorSIGTRAP(
                   __FUNCTION__, GetID(), thread.GetID(), info.si_code);
     break;
   }
+#endif
 }
 
 void NativeProcessLinux::MonitorTrace(NativeThreadLinux &thread) {
@@ -1110,6 +1132,7 @@ void NativeProcessLinux::MonitorSignal(c
   //
   // Similarly, ACK signals generated by this monitor.
 
+#if !defined(__NetBSD__)
   // Handle the signal.
   if (info.si_code == SI_TKILL || info.si_code == SI_USER) {
     if (log)
@@ -1194,6 +1217,7 @@ void NativeProcessLinux::MonitorSignal(c
     // Done handling.
     return;
   }
+#endif
 
   if (log)
     log->Printf("NativeProcessLinux::%s() received signal %s", __FUNCTION__,
@@ -2132,7 +2156,11 @@ NativeProcessLinux::GetCrashReasonForSIG
 
 Error NativeProcessLinux::ReadMemory(lldb::addr_t addr, void *buf, size_t size,
                                      size_t &bytes_read) {
+#if !defined(__NetBSD__)
   if (ProcessVmReadvSupported()) {
+#else
+  {
+#endif
     // The process_vm_readv path is about 50 times faster than ptrace api. We
     // want to use
     // this syscall if it is supported.
@@ -2145,7 +2173,11 @@ Error NativeProcessLinux::ReadMemory(lld
     remote_iov.iov_base = reinterpret_cast<void *>(addr);
     remote_iov.iov_len = size;
 
+#if !defined(__NetBSD__)
     bytes_read = process_vm_readv(pid, &local_iov, 1, &remote_iov, 1, 0);
+#else
+    bytes_read = 0;
+#endif
     const bool success = bytes_read == size;
 
     Log *log(GetLogIfAllCategoriesSet(LIBLLDB_LOG_PROCESS));
@@ -2175,6 +2207,7 @@ Error NativeProcessLinux::ReadMemory(lld
                 (void *)addr, buf, size);
 
   for (bytes_read = 0; bytes_read < size; bytes_read += remainder) {
+#if !defined(__NetBSD__)
     Error error = NativeProcessLinux::PtraceWrapper(
         PTRACE_PEEKDATA, GetID(), (void *)addr, nullptr, 0, &data);
     if (error.Fail()) {
@@ -2182,6 +2215,7 @@ Error NativeProcessLinux::ReadMemory(lld
         ProcessPOSIXLog::DecNestLevel();
       return error;
     }
+#endif
 
     remainder = size - bytes_read;
     remainder = remainder > k_ptrace_word_size ? k_ptrace_word_size : remainder;
@@ -2248,6 +2282,7 @@ Error NativeProcessLinux::WriteMemory(ll
         log->Printf("NativeProcessLinux::%s() [%p]:0x%lx (0x%lx)", __FUNCTION__,
                     (void *)addr, *(const unsigned long *)src, data);
 
+#if !defined(__NetBSD__)
       error = NativeProcessLinux::PtraceWrapper(PTRACE_POKEDATA, GetID(),
                                                 (void *)addr, (void *)data);
       if (error.Fail()) {
@@ -2255,6 +2290,7 @@ Error NativeProcessLinux::WriteMemory(ll
           ProcessPOSIXLog::DecNestLevel();
         return error;
       }
+#endif
     } else {
       unsigned char buff[8];
       size_t bytes_read;
@@ -2293,19 +2329,33 @@ Error NativeProcessLinux::WriteMemory(ll
 }
 
 Error NativeProcessLinux::GetSignalInfo(lldb::tid_t tid, void *siginfo) {
+#if !defined(__NetBSD__)
   return PtraceWrapper(PTRACE_GETSIGINFO, tid, nullptr, siginfo);
+#else
+  Error error;
+  return error;
+#endif
 }
 
 Error NativeProcessLinux::GetEventMessage(lldb::tid_t tid,
                                           unsigned long *message) {
+#if !defined(__NetBSD__)
   return PtraceWrapper(PTRACE_GETEVENTMSG, tid, nullptr, message);
+#else
+  Error error;
+  return error;
+#endif
 }
 
 Error NativeProcessLinux::Detach(lldb::tid_t tid) {
   if (tid == LLDB_INVALID_THREAD_ID)
     return Error();
-
+#if !defined(__NetBSD__)
   return PtraceWrapper(PTRACE_DETACH, tid);
+#else
+  Error error;
+  return error;
+#endif
 }
 
 bool NativeProcessLinux::HasThreadNoLock(lldb::tid_t thread_id) {
@@ -2649,7 +2699,11 @@ void NativeProcessLinux::SigchldHandler(
   // Process all pending waitpid notifications.
   while (true) {
     int status = -1;
+#if !defined(__NetBSD__)
     ::pid_t wait_pid = waitpid(-1, &status, __WALL | __WNOTHREAD | WNOHANG);
+#else
+    ::pid_t wait_pid = waitpid(-1, &status, /* __WALL | __WNOTHREAD | */ WNOHANG);
+#endif
 
     if (wait_pid == 0)
       break; // We are done.
@@ -2713,12 +2767,16 @@ Error NativeProcessLinux::PtraceWrapper(
   PtraceDisplayBytes(req, data, data_size);
 
   errno = 0;
+#if !defined(__NetBSD__)
   if (req == PTRACE_GETREGSET || req == PTRACE_SETREGSET)
     ret = ptrace(static_cast<__ptrace_request>(req), static_cast<::pid_t>(pid),
                  *(unsigned int *)addr, data);
   else
     ret = ptrace(static_cast<__ptrace_request>(req), static_cast<::pid_t>(pid),
                  addr, data);
+#else
+  ret = 0;
+#endif
 
   if (ret == -1)
     error.SetErrorToErrno();