#include <stdlib.h>
#include <stdio.h>
#include <sys/io.h>

typedef unsigned int uint;

uint PCI_BUSSES = 256;
uint NUM_DEVS = 32;
uint NUM_FUNCS = 8;
uint OFFSETA = 0;
uint OFFSETB = 8;
unsigned short int IO_REQ_PORT = 0xcf8;
unsigned short int IO_RES_PORT =  0xcfc;

uint build_io_req(uint busnr, uint devnr, uint fnr, uint offset) {
	return 0x80000000 | busnr << 16 | devnr << 11 | fnr << 8 | offset;
}

int main () {
	uint busnr, devnr, fnr, req;

	if (ioperm(IO_REQ_PORT, 4, 1) == -1) {
		perror("ioperm");
		exit(1);
	}

	if (ioperm(IO_RES_PORT, 4, 1) == -1) {
		perror("ioperm");
		exit(1);
	}

	uint resOffsetA, resOffsetB;
	for (busnr = 0; busnr < PCI_BUSSES; busnr++) {
		for (devnr = 0; devnr < NUM_DEVS; devnr++) {
			for (fnr = 0; fnr < NUM_FUNCS; fnr++) {
				// poll for Device ID and Vendor ID
				req = build_io_req(busnr, devnr, fnr, OFFSETA);
				outl(req, IO_REQ_PORT);
				resOffsetA = inl(IO_RES_PORT);

				// dev aanwezig
				if (resOffsetA != 0xffffffff) {
					// poll for Class code and Revision ID
					req = build_io_req(busnr, devnr, fnr, OFFSETB);
					outl(req, IO_REQ_PORT);
					resOffsetB = inl(IO_RES_PORT);

					// 1. Bus number
					// 2. Device number
					// 3. Function number
					// 4. Class code
					// 5. Device ID
					// 6. Vendor ID
					// 7. if not 0 -> Revision ID
					printf("%02d:%02x.%d %04x: %x:%x", 
						busnr,
						devnr,
						fnr,
						resOffsetB >> 16,
						resOffsetA & ((1 << 16) - 1),
						resOffsetB >> 16
					);

					if (resOffsetB & ((1 << 8) - 1)) {
						printf(" (rev %02d)", resOffsetB & ((1 << 8) - 1));
					} 
					printf("\n");
				}
			}
		}
	}
	return 0;
}
