all:
	make -C libteletext
	make -C tools

install:
	make -C libteletext install
	make -C tools install

uninstall:
	make -C libteletext uninstall
	make -C tools uninstall

clean:
	make -C libteletext clean
	make -C tools clean

.PHONY: all install uninstall clean
