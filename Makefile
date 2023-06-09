all:
	make -C libteletext
	make -C teletext
	make -C tests

install:
	make -C libteletext install
	make -C teletext    install

uninstall:
	make -C libteletext uninstall
	make -C teletext    uninstall

clean:
	make -C libteletext clean
	make -C teletext    clean
	make -C tests       clean

.PHONY: all install uninstall clean
