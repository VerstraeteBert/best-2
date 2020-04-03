#include <unistd.h>
#include <stdio.h>
//	Fork maakt een kinderproces aan met exact dezelfde code als de parent (ook zelfde variabele inhoud)
//	return waarde van fork 
//		-> pid van kindproces
//		-> 0 indien kindproces is
// kinderprocessen kunnen opnieuw processen maken$
//
// vb:
// ...
// fork()
// fork()
// fork()
// ...
// zal in totaal 8 kindprocessen starten
int main () {
	int pid;
	if ((pid = fork()) == 0) {
		// kind
		printf("Child has PID=%d\n", getpid());
	} else {
		// 
		printf("Parent has PID=%d\n", getpid());
	}
	return 0;
}
