$NetBSD$

--- chrome/browser/ui/libgtkui/print_dialog_gtk.cc.orig	2017-02-02 02:02:50.000000000 +0000
+++ chrome/browser/ui/libgtkui/print_dialog_gtk.cc
@@ -348,6 +348,7 @@ void PrintDialogGtk2::ShowDialog(
   // Since we only generate PDF, only show printers that support PDF.
   // TODO(thestig) Add more capabilities to support?
   GtkPrintCapabilities cap = static_cast<GtkPrintCapabilities>(
+      GTK_PRINT_CAPABILITY_GENERATE_PS |
       GTK_PRINT_CAPABILITY_GENERATE_PDF |
       GTK_PRINT_CAPABILITY_PAGE_SET |
       GTK_PRINT_CAPABILITY_COPIES |
