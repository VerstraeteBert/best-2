#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>


// 1. lokale variabelen die static zijn, behouden hun waarden tussen functie calls
// 2. static globals -> enkel zichtbaar binnen eigen file
// 3. static functies ook niet zichtbaar buiten de file waarin ze gedeclareerd zijn
static int getal;

int main () {
	return 0;
}