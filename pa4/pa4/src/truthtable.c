#include <stdio.h>
#include <stdlib.h>

typedef enum { PASS, NOT, AND, NAND, NOR, OR, XOR, DECODER, MULTIPLEXER } kind_t;

// The master Variable table is made up of Variables. A single variable contains the variable name,
// the index of it in the current Table, and the value it is currently holding.
struct Variable {
	char *VarName;
	int index;
	int value;
};

// The master Variable Table. Will hold all our variables for our gate to refer to.
struct VarTable {
	struct Variable	*Vars;
	int	InputEnd;
	int	OutputEnd;
	int	TempEnd;
};

/** Gates of the ciruit.
 * Next : Holds the location of the Gate to be completed next.
 * **Inparam : Array that points to the variables in the master table that it relies on for input.
 * **outparam : Same as inparam but for output.
 * NumOfParam : Keeps track on how many variables it deals with. Mostly important for MUX and DEC.
 * type : 0-11 value that will allow us to quickly decifer what gate it is.
 */
struct Gate {
	int **inparam;
	int **outparam;
	struct Gate *next;
	int NumOfIn;
	int NumOfOut;
	kind_t type;
};

// ====================================== Utility Functions ====================================================

/** String Compare Method */
int StrComp(char *p1, char *p2)
{
	char c1, c2;
	do {
		c1 = *p1++;
		c2 = *p2++;
		if(c1 == '\0')
			return c1 - c2;
	} while(c1 == c2);

	return c1 - c2;
}

/** String Copy */
void StrCopy(char *src, char *dest)
{
	char c1;
	do {
		c1 = *src++;
		*dest++ = c1;
	} while(c1 != '\0');
}

void FreeTable(struct VarTable Table)
{
	int i;
	for(i = 0; i < Table.TempEnd; ++i)
		free(Table.Vars[i].VarName);

	free(Table.Vars);
}

void FreeGates(struct Gate *List)
{
	struct Gate *temp;
	while(List != NULL){
		temp = List;
		List = List->next;
		free(temp);
	}
}

/** Math Power Function. Base ^ Exponent */
int Pow(int Base, int Exponent)
{
	int i, total = Base;
	if(Exponent == 0) return 1;
	for(i = 1; i < Exponent; ++i)
		total *= Base;
	return total;
}

/** Print the Variable Table */
void PrintTableVars(struct VarTable Table)
{
	int i;
	puts("All found Varibales:");
	for(i = 0; i < Table.TempEnd; ++i)
		printf("%s\n", Table.Vars[i].VarName);
	puts("End of Variable Dump");
}

/** Print the values of the variables in the table */
void PrintTableValues(struct VarTable Table)
{
	int i;
	for(i = 0; i < Table.InputEnd; ++i)
		printf("%d ", Table.Vars[i].value);

	printf("| ");

	for(; i < Table.OutputEnd; ++i){
		if(i+1 == Table.OutputEnd) 
			printf("%d",Table.Vars[i].value);
		
		else printf("%d ", Table.Vars[i].value);
		
	}
	printf("\n");
}

void PrintAllTableValues(struct VarTable Table)
{
	int i;
	for(i = 0; i < Table.InputEnd; ++i)
		printf("%d ", Table.Vars[i].value);

	printf("| ");

	for(; i < Table.OutputEnd; ++i){
		 printf("%d ", Table.Vars[i].value);
	}

	printf("| ");

	for(; i < Table.TempEnd; ++i)
		printf("%d ", Table.Vars[i].value);
	printf("\n");
}

void PrintGates(struct Gate *First)
{
	while(First != NULL){
		printf("NumofIn: %d NumofOut: %d, type %d\n", First->NumOfIn, First->NumOfOut, First->type);
		First = First->next;
	}
}

// ====================================== End Of Utility Functions ====================================================

// ======================================= Start of Main Functions ====================================================
/** Read the first two lines of the input file, getting all the basic Input/Output Variables adding them to the Table. */
void ReadIOVars(struct VarTable *Table, FILE **fp)
{
	// Scan the 'INPUT' string
	char IOBUF[7];
	fscanf(*fp, "%s", IOBUF);
	int i, NumOfInputs;
	fscanf(*fp, "%d", &NumOfInputs);
	
	Table->InputEnd = NumOfInputs;
	Table->Vars = malloc(sizeof(struct Variable) * NumOfInputs);
	
	// Read the Input Vars and store them in the Variable struct
	for(i = 0; i < NumOfInputs; ++i) {
		Table->Vars[i].VarName = malloc(sizeof(char) * 17);
		fscanf(*fp, "%16s", Table->Vars[i].VarName);
		Table->Vars[i].index = i;
		Table->Vars[i].value = 0;
	}

	// Scan the 'OUTPUT' string and reuse the NumOfInputs variable for the NumOfOutputs
	fscanf(*fp, "%s", IOBUF);
	fscanf(*fp, "%d", &NumOfInputs);

	// Allocate more space for the output variables
	Table->Vars = realloc(Table->Vars, sizeof(struct Variable) * (NumOfInputs + Table->InputEnd));
	Table->OutputEnd = NumOfInputs + Table->InputEnd;

	// Read the Output Vars and store them in the Variable struct
	for(; i < Table->OutputEnd; ++i) {
		Table->Vars[i].VarName = malloc(sizeof(char) * 17);
		fscanf(*fp, "%16s", Table->Vars[i].VarName);
		Table->Vars[i].index = i;
		Table->Vars[i].value = 0;
	}
	return;
}

/** Continue from where I/O Vars left off. Searching through the directives to find any temporary variables appending
 * them to the Variable Table. */
void Search_For_Temps(struct VarTable *Table, FILE *fp)
{
	char BUFFER[17];
	int NumOfIn, NumOfOut;
	Table->TempEnd = Table->OutputEnd;
	while(fscanf(fp, "%16s", BUFFER) != EOF) {
		// Check if its a 'NOT' or 'PASS' Gate
		if( (BUFFER[0] == 'N' && BUFFER[2] == 'T') || (BUFFER[0] == 'P')) {
			NumOfIn = 1;
			NumOfOut = 1;
		}
		// Check if it's Decoder or Multiplexer 
		else if( (BUFFER[0] == 'D') || (BUFFER[0] == 'M') ) {
			fscanf(fp, "%d", &NumOfIn);

			if(BUFFER[0] == 'D')
				NumOfOut = Pow(2, NumOfIn);
			else { 
				NumOfOut = 1;
				NumOfIn += Pow(2, NumOfIn);
			}
			
		}
		// If it's not the previous choices, it must have only 2 inputs and 1 output.
		else {
			NumOfIn = 2;
			NumOfOut = 1;
		}
		
		// Go through our current Variables and see if we already have that Variable.
		// If not, append it to our table.
		int i, j;
		for(i = 0; i < (NumOfIn + NumOfOut); ++i) {
			fscanf(fp, "%16s", BUFFER);
			if( !(BUFFER[0] == '1' || BUFFER[0] == '0' || BUFFER[0] == '_') ){
				for(j = 0; j < Table->TempEnd; ++j){
					if(!StrComp(BUFFER, Table->Vars[j].VarName))
						break;
				}
				if(j == Table->TempEnd) {
					Table->Vars = realloc(Table->Vars, sizeof(struct Variable) * (1 + Table->TempEnd));
					Table->Vars[Table->TempEnd].VarName = malloc(sizeof(BUFFER));
					StrCopy(BUFFER, Table->Vars[Table->TempEnd].VarName);
					Table->Vars[Table->TempEnd].index = Table->TempEnd;
					Table->Vars[Table->TempEnd].value = 0;
					++(Table->TempEnd);
				}
			}
		}
	}
	return;
}

/** Create the Logic Gates using a Linked List Data Structure to link them together. */
void CreateGates(struct Gate **First, struct VarTable Table, int *binary, FILE *fp)
{
	struct Gate **Indirect = First;
	char BUFFER[17];
	int temp;
	kind_t type;
	// Skip the first two lines of the file.
	char SKIP[16384];
	fgets(SKIP, 16384, fp);
	fgets(SKIP, 16384, fp);

	while(fscanf(fp, "%16s", BUFFER) != EOF) {
		// Find what type of gate it is
		switch(BUFFER[0]){
			case 'D':
				type = DECODER;
				break;
			case 'M':
				type = MULTIPLEXER;
				break;
			case 'N':
				if(BUFFER[2] == 'T') type = NOT;
				else if(BUFFER[2] == 'N') type = NAND;
				else type = NOR;
				break;
			case 'P':
				type = PASS;
				break;
			case 'O':
				type = OR;
				break;
			case 'A':
				type = AND;
				break;
			case 'X':
				type = XOR;
				break;
		}

		*Indirect = malloc(sizeof(struct Gate));
		(*Indirect)->next = NULL;
		(*Indirect)->type = type;
		
		// Based on it's type we can easily decifer how many inputs and outputs the gate will have.
		if(type < 2){
			(*Indirect)->NumOfIn = 1;
			(*Indirect)->NumOfOut = 1;
		}
		else if(type < 7){
			(*Indirect)->NumOfIn = 2;
			(*Indirect)->NumOfOut = 1;
		}
		// A decoder and multiplexer requires a little more work to work out how many ins and outs it has.
		else {
			int inputs;
			fscanf(fp, "%d", &inputs);
			if(type == DECODER){
				(*Indirect)->NumOfIn = inputs;
				(*Indirect)->NumOfOut = Pow(2, inputs);
			}
			else {
				(*Indirect)->NumOfIn = (inputs+ Pow(2,inputs));
				(*Indirect)->NumOfOut = 1;
				temp = inputs;
			}
		}

		// Begin allocating space for our input parameters. The array will be int pointers so that we can refer to the
		// master variable table and quickly pull what value is currently stored there.
		(*Indirect)->inparam = malloc(sizeof(int*) * ((*Indirect)->NumOfIn));
		int i, j;
		// Scan through the file and store those variables
		for(i = 0; i < (*Indirect)->NumOfIn; ++i){
			fscanf(fp, "%16s", BUFFER);
			// If it is a '0' , '1' , or '_' then point to the special case "binary" array. This array contains 0, 1,
			// and -1 for their respective symbols.
			if(BUFFER[0] == '0') (*Indirect)->inparam[i] = &(binary[0]);
			else if(BUFFER[0] == '1') (*Indirect)->inparam[i] = &(binary[1]);
			else if (BUFFER[0] == '_') (*Indirect)->inparam[i] = &(binary[2]);
			else {
				for(j = 0; j < Table.TempEnd; ++j) {
					if(!StrComp(BUFFER, Table.Vars[j].VarName))
						(*Indirect)->inparam[i] = &(Table.Vars[j].value);
				}
			}
		}

		// Allocate space for pointers to our output variables using the same ideas as above.
		(*Indirect)->outparam = malloc(sizeof(int*) * ((*Indirect)->NumOfOut));
		for(i = 0; i < (*Indirect)->NumOfOut; ++i){
			fscanf(fp, "%16s", BUFFER);
			if(BUFFER[0] == '0') (*Indirect)->outparam[i] = &(binary[0]);
			else if(BUFFER[0] == '1') (*Indirect)->outparam[i] = &(binary[1]);
			else if (BUFFER[0] == '_') (*Indirect)->outparam[i] = &(binary[2]);
			else {
				for(j = 0; j < Table.TempEnd; ++j) {
					if(!StrComp(BUFFER, Table.Vars[j].VarName)){
						(*Indirect)->outparam[i] = &(Table.Vars[j].value);
						break;
					}
				}
			}
		}

		// Just to make life simpler, I want the input # of the Multiplexer, not input total.
		if(temp != 0) (*Indirect)->NumOfIn = temp;

		// Once finished, move on to the next directive.
		Indirect = &((*Indirect)->next);
	}
		return;
}

/** This function runs through the gates and attempts to solve for the truthtable. */
void DoCircuit(struct Gate *First, struct VarTable Table)
{
	// Keep looping until we have no more gates.
	while(First != NULL){
		switch(First->type){
			// PASS
			case 0:
				First->outparam[0][0] = First->inparam[0][0];
				break;
			
			// NOT
			case 1:
				First->outparam[0][0] = (First->inparam[0][0] == 1) ? 0 : 1;
				break;

			// AND
			case 2:
				if(First->inparam[0][0] == 1 && First->inparam[1][0] == 1)
					First->outparam[0][0] = 1;
				else
					First->outparam[0][0] = 0;
				break;

			// NAND
			case 3:
				if(First->inparam[0][0] == 1 && First->inparam[1][0] == 1)
					First->outparam[0][0] = 0;
				else
					First->outparam[0][0] = 1;
				break;

			// NOR
			case 4:
				if(First->inparam[0][0] == 1 || First->inparam[1][0] == 1)
					First->outparam[0][0] = 0;
				else
					First->outparam[0][0] = 1;
				break;

			// OR
			case 5:
				if(First->inparam[0][0] == 1 || First->inparam[1][0] == 1)
					First->outparam[0][0] = 1;
				else
					First->outparam[0][0] = 0;
				break;

			// XOR
			case 6:
				if( (First->inparam[0][0] == 1 && First->inparam[1][0] == 0) || 
					(First->inparam[0][0] == 0 && First->inparam[1][0] == 1))
						First->outparam[0][0] = 1;
				else
					First->outparam[0][0] = 0;
				break;

			// DECODER
			case 7:
			{
				int i, incrementer = 1, bit = 0;
				for(i = 0; i < First->NumOfIn; ++i){
					if(First->inparam[i][0] == 1)
						bit += Pow(2, First->NumOfIn - incrementer);

					++incrementer;
				}
				// Discard our answer if that position holds a -1 representing a '_'
				if(!(First->outparam[bit][0] == -1)) First->outparam[bit][0] = 1;
				break;
			}	
			// MULTIPLEXER
			case 8:
			{
				int i, selectorindex = (Pow(2, First->NumOfIn));
				int totalinputs = selectorindex + First->NumOfIn;
				int row = 0, incrementer = 1;
				for(i = selectorindex; i < totalinputs; ++i){
					if(First->inparam[i][0] == 1)
						row += Pow(2, First->NumOfIn - incrementer);

					++incrementer;
				}
				First->outparam[0][0] = First->inparam[row][0];
				break;
			}
		}
		// Go to the following gate.
		First = First->next;
	}
	PrintTableValues(Table);
	return;
}

void SortGates(struct Gate **First, struct VarTable Table)
{
	int i;
	while(*First != NULL) {
		// Iterate through all inputs of the gate and check for Multiplexer special case
		int NumOfIn = ((*First)->type == 8) ? ((*First)->NumOfIn + Pow(2, (*First)->NumOfIn)) : (*First)->NumOfIn;

		for(i = 0; i < NumOfIn; ++i){
			int *TempAddr = (*First)->inparam[i], j;
			// Go through all the Temporary Variables
			for(j = Table.OutputEnd; j < Table.TempEnd; ++j){
				int *TableAddr = &(Table.Vars[j].value);
				if(TempAddr == TableAddr && Table.Vars[j].value == 0){

					//puts("POINTERS MATCH!");
					Table.Vars[j].value = 1;
					struct Gate **swap = &((*First)->next);

					while(*swap != NULL){
						int k, found = 0;
						//printf("This gate is of type %d\n", (*swap)->type);

						for(k = 0; k < ((*swap)->NumOfOut); ++k){
							if((*swap)->outparam[k][0] == 1){
								found = 1;
								break
							}
						}

						if(found){
							// Swap the pointers...
							struct Gate *temp = *First;
							*First = *swap;
							struct Gate *temp2 = (*swap)->next;
							(*swap)->next = temp;
							*swap = temp2;
							break;
						}
						else
							swap = &((*swap)->next);
					}
				}
			}
		}
		//printf("going to next gate of type %d\n", (*First)->type);
		First = &((*First)->next);
	}
	// Reset values of the Table
	for(i = 0; i < Table.TempEnd; ++i)
		Table.Vars[i].value = 0;
	return;
}

void Solve_Truth_Table(struct Gate *First, struct VarTable Table)
{
	int i,start = (Table.InputEnd-1);
	// Do the case where all Inputs are 0
	DoCircuit(First, Table);

	// Then enter the loop where we start putting 1s for inputs while working from right to left.
	// So if we had x, y, z as inputs, z would become 1 and then y and then x.
	for(i = start; i >= 0; --i){
		
		// If the current variable is a 0, flip it to 1 and reset back to the first var.
		if(Table.Vars[i].value == 0){
			Table.Vars[i].value = 1;
			i = start + 1;
		}
		
		// If the current variable is a 1, we can flip the bit and move to next var as if we were to "carry" in addition
		else if(Table.Vars[i].value == 1)
			Table.Vars[i].value = 0;
		
		// If we did flip a bit and restart, that means we have yet to get to the last bit so do the circuit.
		if(i == start + 1){
			int j;
			for(j = Table.InputEnd; j < Table.TempEnd; ++j)
				Table.Vars[j].value = 0;
			DoCircuit(First, Table); 
		}

	}
	return;
}

// ====================================== End Of Main Functions ====================================================

int main(int argc, char *argv[])
{
	if(argc != 2) {
		puts("Error: Missing Inputs. Usage: ./truthtable [pathtocircuit]");
		return 1;
	}

	FILE *fp = fopen(argv[1], "r");
	if(!fp) {
		printf("error opening file");
		return 1;
	}
	
	struct VarTable Table;
	ReadIOVars(&Table, &fp);
	//puts("Read IO Vars");
	
	Search_For_Temps(&Table, fp);
	//puts("Read all temps");
	
	//PrintTableVars(Table);
	
	struct Gate *First = malloc(sizeof(struct Gate));
	int binary[] = {0 , 1, -1};
	rewind(fp);
	CreateGates(&First, Table, binary, fp);
	//PrintGates(First);
	//PrintTableValues(Table);
	SortGates(&First, Table);
	//puts("After Sorting...");
	//PrintGates(First);
	Solve_Truth_Table(First, Table);	

	fclose(fp);
	FreeTable(Table);
	FreeGates(First);
	return 0;
}
