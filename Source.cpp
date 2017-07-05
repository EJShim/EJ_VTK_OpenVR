#include "K_IGSTK_Header.h"
#include "E_Manager.h"


int main(int, char *[])
{
	E_Manager::Mgr();
	E_Manager::Mgr()->Start();


	return EXIT_SUCCESS;
}