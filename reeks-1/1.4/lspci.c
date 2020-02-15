#include <stdlib.h>
#include <stdio.h>
#include <sys/io.h>

typedef unsigned int uint;

uint PCI_BUSSES = 256;
uint NUM_DEVS = 32;
uint NUM_FUNCS = 8;
uint REG_NR = 0;
unsigned short int IO_DATA_PORT = 0xcf8;
unsigned short int DELAY_PORT = 0x80;

uint build_io_req(uint busnr, uint devnr, uint fnr, uint regnr) {
	return busnr << 16 | devnr << 11 | fnr << 8 | regnr;
}

int main () {
	uint i, j, n;
	
	uint req;
	if (ioperm(IO_DATA_PORT, 5, 1) == -1) {
		perror("ioperm");
		exit(1);
	};

	if (ioperm(DELAY_PORT, 1, 1) == -1) {
		perror("ioperm");
		exit(1);
	};

	printf("VendorID\tDeviceID\n");
	uint res;
	for (i = 0; i < PCI_BUSSES; i++) {
		for (j = 0; j < NUM_DEVS; j++) {
			for (n = 0; n < NUM_FUNCS; n++) {
				req = build_io_req(i, j, n, 0);

				outl_p(req, IO_DATA_PORT);

				res = inl(IO_DATA_PORT);

				// dev aanwezig
				if (res != 0xffffffff) {
					printf("%d\t%d\t%d\n", res, res >> 16, res & ((1 << 16) - 1));
				}
			}
		}
	}
	return 0;
}
