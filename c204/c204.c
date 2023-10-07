/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	// Variable for storing the top of the stack.
	char stackTop;

	// While the stack is not empty, save the top of the stack to stackTop and pop it.
	while ( !Stack_IsEmpty( stack ) ) {
		Stack_Top( stack, &stackTop );
		Stack_Pop( stack );
		// If the top of the stack is a left parenthesis, break the loop.
		if ( stackTop == '(' ) {
			// Don't save the left parenthesis to the postfix expression.
			break;
		}
		// Save the poped top of the stack (math operator) to the actual position in the postfix exp.
		// Increase the actual postion afrer saving the operator.
		postfixExpression[(*postfixExpressionLength)++] = stackTop;
	}
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	// If the stack is empty, push the operator to the stack.
	if ( Stack_IsEmpty( stack ) ) {
		Stack_Push( stack, c );
		return;
	}

	char stackTop;
	Stack_Top( stack, &stackTop );
	// If the stack top is a left parenthesis, push the operator to the stack.
	if ( stackTop == '(' ) {
		Stack_Push( stack, c );
		return;
	} else if ((stackTop == '+' || stackTop == '-') && ( c == '*' || c == '/' )) {
		// If the stack top is a math operator with lower priority, push the operator to the stack.
		// a + b * c = a b c * + = a (b c *) +
		Stack_Push( stack, c );
		return;
	} else {
		// If the stack top is a math operator with higher priority, pop the stack top and do the operation again.
		// a * b + c = a b * c + = ((a b *) c +)
		postfixExpression[(*postfixExpressionLength)++] = stackTop;
		Stack_Pop( stack );
		doOperation( stack, c, postfixExpression, postfixExpressionLength );
	}

}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	// Allocation of memory for the output string.
	char *postfixExpression = (char *) malloc( MAX_LEN * sizeof( char ) );

	// If the allocation failed, return NULL.
	if ( !postfixExpression ) {
		return NULL;
	}

	// Allocation of memory for the stack of the operators.
	Stack *stack = (Stack *) malloc( sizeof( Stack ) );
	// If the allocation failed, return NULL.
	if ( !stack ) {
		free( postfixExpression );
		return NULL;
	}

	Stack_Init( stack );
	
	// Variable for storing the actual character from the infix expression.
	char c = *infixExpression;
	unsigned int postfixExpressionLength = 0;
	// 
	while (c != '\0') {
		// If the character is a letter or a number, add it to the postfix expression.
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
			postfixExpression[postfixExpressionLength++] = c;
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') {
			// If the character is a math operator call doOperation() to decide, 
			// whether store it to operators stack or add it direct to postfix expresson.
			doOperation(stack, c, postfixExpression, &postfixExpressionLength);
		} 
		else if (c == '(') {
			// If the character is a left parenthesis, push it to the stack.
			Stack_Push(stack, c);
		} 
		else if (c == ')') {
			// If the character is a right parenthesis, call untilLeftPar() to pop the stack until left parenthesis.
			untilLeftPar(stack, postfixExpression, &postfixExpressionLength);
		} 
		else if (c == '=') {
			// If the character is an equal sign, pop all the operators from the stack
			// and add it to the postfix expression.
			while (!Stack_IsEmpty(stack)) {
				Stack_Top(stack, &(postfixExpression[postfixExpressionLength++]));
				Stack_Pop(stack);
			}
			// After adding all the operators, add the equal sign to the postfix expression.
			postfixExpression[postfixExpressionLength++] = '=';
			// Break the loop.
			break;
		}
		// Move to the next character in the infix expression.
		c = *(++infixExpression);
		// infixExpression++;
		// c = *infixExpression;
	}

	// Add the null terminator to the end of the postfix expression.
	postfixExpression[postfixExpressionLength] = '\0';
	free(stack);

	return postfixExpression;
	
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

	// value = xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx - 32-bit integer (4 bytes)
	// Loop through the 4 bytes, from the most significant to the least significant
	for (int i = 3; i >= 0; i--) {

		// value >> (i * 8) - bit shift to the right by i * 8 bits
		// 0xFF - 8-bit mask (00000000 00000000 00000000 11111111)
		// Using bitwise AND to get the last 8 bits of the shifted value
        char byte = (value >> (i * 8)) & 0xFF;

		// Push the byte to the stack
        Stack_Push(stack, byte);
    }
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

	// *value = 0; // Initialize the value to 0 before populating with bytes
    // for (int i = 3; i >= 0; i--) {
    //     char byte;
    //     Stack_Top(stack, &byte); // Get the top byte from the stack
    //     Stack_Pop(stack); // Remove the top byte from the stack
    //     *value |= (byte << (i * 8)); // Combine the byte into the 4-byte integer
    // }
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {

	return NULL;
}

/* Konec c204.c */
