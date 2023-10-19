/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	// Inicialize DL list structure from c206.h .
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	
	while (list->firstElement != NULL) {
		// Free each element of list until list is empty.
		DLLElementPtr nextElem = (list->firstElement)->nextElement;
		// Free firs element of list
		free(list->firstElement);
		// Set next element of list as the first element.
		list->firstElement = nextElem;
	}
	// Set list to initial state.
	// list->firstElement = NULL; - is already NULL after previous while loop.
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	// Allocate memory for new element.
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
		// Allocation failed
        DLL_Error(); 
    }

	if (list->firstElement == NULL) {
		// List is an empty, newElement is the first and the last element of list.

		list->lastElement = newElement;

	} else {
		// List is not empty, newElement is the first element of list.
		// Set previous element of first element of list to newElement.

		list->firstElement->previousElement = newElement;
	}

	newElement->data = data;
	// Set next element of newElement to first element of list, 
	// because newElement is the first element of list.
	newElement->nextElement = list->firstElement;
	list->firstElement = newElement;
	// Set previous element of newElement to NULL,
	// because newElement is the first element of list.
	newElement->previousElement = NULL;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	// Allocate memory for new element.
	DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    if (newElement == NULL) {
		// Allocation failed
        DLL_Error(); 
    }

	if (list->lastElement == NULL) {
		// List is an empty, newElement is the first and the last element of list.
		list->firstElement = newElement;

	} else {
		// List is not empty, set next element of last element of list to newElement.
		list->lastElement->nextElement = newElement;
	}

	newElement->data = data;
	// Nevermind if list is empty or not, set newElement as the last element of list.
	list->lastElement = newElement;
	// Set next element of newElement to NULL,
	// because newElement is the last element of list.
	newElement->nextElement = NULL;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement != NULL) {
        *dataPtr = list->firstElement->data;
    } else {
		// List is empty, call DLL_Error().
        DLL_Error();
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->lastElement != NULL) {
        *dataPtr = list->lastElement->data;
    } else {
        DLL_Error();
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement != NULL) {

		// List is not empty.
		DLLElementPtr nextElement = (list->firstElement)->nextElement;

		if (list->activeElement == list->firstElement) {
			// Active element is being deleted.
			list->activeElement = NULL;
		}
		free(list->firstElement);
		list->firstElement = nextElement;

		if (list->firstElement == NULL) {
			// List is empty after deletion.
			list->lastElement = NULL;
		} else {
			// Set previous element of new first element of list to NULL,
			// because new first element of list is the first element of list.
			list->firstElement->previousElement = NULL;
		}
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->lastElement != NULL) {

		// List is not empty.
		DLLElementPtr previousElement = (list->lastElement)->previousElement;

		if (list->activeElement == list->lastElement) {
			// Active element is being deleted.
			list->activeElement = NULL;
		}
		free(list->lastElement);
		list->lastElement = previousElement;

		if (list->lastElement == NULL) {
			// List is empty after deletion.
			list->firstElement = NULL;
		} else {
			// Set next element of new last element of list to NULL,
			// because new last element of list is the last element of list.
			list->lastElement->nextElement = NULL;
		}
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	// ... activeElement -> nextActiveElement -> nextNextElement ... 

	// List is active and active element is not the last element of list.
	if (list->activeElement != NULL && list->activeElement->nextElement != NULL) {

		DLLElementPtr nextActiveElement = list->activeElement->nextElement;
		DLLElementPtr nextNextElement = nextActiveElement->nextElement;

		if (nextNextElement == NULL) {
			// Element we want to delete is the last one.
			// ... activeElement -> DELETED
			list->lastElement = list->activeElement;
		} else {
			// Element we want to delete is NOT the last one.
			// ... activeElement -> DELETED -> nextNextElement -> ...
			nextNextElement->previousElement = list->activeElement;
		}

		list->activeElement->nextElement = nextNextElement;
		free(nextActiveElement);
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	// ... previousPreviousElement -> previousActiveElement -> activeElement ...

	// List is active and active element is not the first element of list.
	if (list->activeElement != NULL && list->activeElement->previousElement != NULL) {

		DLLElementPtr previousActiveElement = list->activeElement->previousElement;
		DLLElementPtr previousPreviousElement = previousActiveElement->previousElement;

		if (previousPreviousElement == NULL) {
			// Element we want to delete is the first one.
			// DELETED -> activeElement ...
			list->firstElement = list->activeElement;
		} else {
			// Element we want to delete is NOT the first one.
			// ... previousPreviousElement -> DELETED -> activeElement ...
			previousPreviousElement->nextElement = list->activeElement;
		}

		list->activeElement->previousElement = previousPreviousElement;
		free(previousActiveElement);
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

	// List is active.
	if (list->activeElement != NULL) {

		// Allocate memory for new element.
		DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
		if (newElement == NULL) {
			// Allocation failed
			DLL_Error(); 
		}
		// Shortcut for next element of active element.
		DLLElementPtr nextElement = list->activeElement->nextElement;

		if (nextElement == NULL) {
			// Active element IS the last one.
			list->lastElement = newElement;
		} else {
			// Active element IS NOT the last one.
			nextElement->previousElement = newElement;
		}
		// Set next element of active element to new element.
		list->activeElement->nextElement = newElement;
		// Set previous element of new element to active element.
		newElement->previousElement = list->activeElement;
		// Set next element of new element to next element of active element.
		newElement->nextElement = nextElement;
		// Set data of new element.
		newElement->data = data;
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	// List is active.
	if (list->activeElement != NULL) {

		// Allocate memory for new element.
		DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
		if (newElement == NULL) {
			// Allocation failed
			DLL_Error(); 
		}
		// Shortcut for previous element of active element.
		DLLElementPtr previousElement = list->activeElement->previousElement;

		if (previousElement == NULL) {
			// Active element IS the first one.
			list->firstElement = newElement;
		} else {
			// Active element IS NOT the first one.
			previousElement->nextElement = newElement;
		}
		// Set previous element of active element to new element.
		list->activeElement->previousElement = newElement;
		// Set next element of new element to active element.
		newElement->nextElement = list->activeElement;
		// Set previous element of new element to previous element of active element.
		newElement->previousElement = previousElement;
		// Set data of new element.
		newElement->data = data;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement != NULL) {
		*dataPtr = list->activeElement->data;
	} else {
		DLL_Error();
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL) {
		if (list->activeElement->nextElement != NULL) {
			list->activeElement = list->activeElement->nextElement;
		} else {
			list->activeElement = NULL;
		}
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL){
		if (list->activeElement->previousElement != NULL){
			list->activeElement = list->activeElement->previousElement;
		} else {
			list->activeElement = NULL;
		}
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return (list->activeElement != NULL);
}

/* Konec c206.c */
