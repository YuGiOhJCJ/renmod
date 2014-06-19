dist_doc_DATA=AUTHORS ChangeLog COPYING INSTALL NEWS README TODO
bin_PROGRAMS=renmod grenmod
renmod_SOURCES=renmod.c librenmod.c
renmod_LDFLAGS=-lmikmod
renmod_CFLAGS=-Wall -ansi -std=c89 -g
grenmod_SOURCES=grenmod.c librenmod.c
grenmod_LDFLAGS=-lmikmod @GTK_LIBS@
grenmod_CFLAGS=-Wall -ansi -std=c89 -g @GTK_CFLAGS@
.PHONY: edit
TEXTEDITOR=gvim -p
edit:
	$(TEXTEDITOR) *.c *.h ChangeLog Makefile.am configure.ac
maintainer-clean-local:
	rm -rfv aclocal.m4
	rm -rfv compile
	rm -rfv configure
	rm -rfv depcomp
	rm -rfv install-sh
	rm -rfv Makefile.in
	rm -rfv missing
