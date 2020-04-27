#include <stdio.h>
#include <stdlib.h>

// Allows us to simply assign a value to our gate (0-8) that will decide its behavior.
typedef enum { PASS, NOT, AND, NAND, NOR, OR, XOR, DECODER, MULTIPLEXER } kind_t;

// The master Variable table is made up of Variables. A single variable contains the variable name,
// the index of it in the current Table, and the value it is currently holding.
struct Variable {
	int value;
	int index;
	char *VarName;
};

// The master Variable Table. Will hold all our variables for our gate to refer to with indexes that hold
// where our inputs, outputs, and temporary variables end.
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
 * type : 0-8 value that will allow us to quickly decifer what gate it is.
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
		free(temp->inparam);
		free(temp->outparam);
		free(temp);
	}
}

/** Math Power Function. Base ^ Exponent */
int Pow(int Base, int Exponent)
{
	if(Exponent == 0)
		return 1;

	int i, total = Base;
	for(i = 1; i < Exponent; ++i)
		total *= Base;

	return total;
}

/** Print the values of inputs & outputs of the table. */
void PrintTableValues(struct VarTable Table)
{
	int i;
	for(i = 0; i < Table.InputEnd; ++i)
		printf("%d ", Table.Vars[i].value);

	printf("| ");

	for(; i < Table.OutputEnd; ++i){
		if(i+1 == Table.OutputEnd) 
			printf("%d",Table.Vars[i].value);
		
		else
			printf("%d ", Table.Vars[i].value);
		
	}
	printf("\n");
}

// ====================================== End Of Utility Functions ====================================================

/** Read the first two lines of the input file, getting all the basic Input/Output Variables adding them to the Table. */
void ReadIOVars(struct VarTable *Table, FILE *fp)
{
	// Scan the 'INPUT' string and the number of input variables
	char IOBUF[7];
	int i, NumOfInputs, NumOfOutputs;

	fscanf(fp, "%s", IOBUF);
	fscanf(fp, "%d", &NumOfInputs);
	
	Table->InputEnd = NumOfInputs;
	Table->Vars = malloc(sizeof(struct Variable) * NumOfInputs);
	
	// Read the Input Vars and store them in the Variable struct
	for(i = 0; i < NumOfInputs; ++i) {
		Table->Vars[i].VarName = malloc(sizeof(char) * 17);
		fscanf(fp, "%16s", Table->Vars[i].VarName);
		Table->Vars[i].index = i;
		Table->Vars[i].value = 0;
	}

	// Scan the 'OUTPUT' string and scan how many total output variables there are 
	fscanf(fp, "%s", IOBUF);
	fscanf(fp, "%d", &NumOfOutputs);

	// Allocate more space for the output variables
	Table->Vars = realloc(Table->Vars, sizeof(struct Variable) * (NumOfOutputs + Table->InputEnd));
	Table->OutputEnd = NumOfOutputs + Table->InputEnd;

	// Read the Output Vars and store them in the Variable struct
	for(; i < Table->OutputEnd; ++i) {
		Table->Vars[i].VarName = malloc(sizeof(char) * 17);
		fscanf(fp, "%16s", Table->Vars[i].VarName);
		Table->Vars[i].index = i;
		Table->Vars[i].value = 0;
	}
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
}

/** Create the Logic Gates and link them together in a linked list style. */
void CreateGates(struct Gate **First, struct VarTable Table, int *binary, FILE *fp)
{
	struct Gate **Indirect = First;
	char BUFFER[17], SKIP[8192];
	kind_t type;
	int inputs, i, j;

	// Skip the first two lines of the file.
	fgets(SKIP, 8192, fp);
	fgets(SKIP, 8192, fp);

	while(fscanf(fp, "%16s", BUFFER) != EOF) {
		// Find what type of gate it is and assign the type
		switch(BUFFER[0]){

			case 'D':
				type = DECODER;
				break;

			case 'M':
				type = MULTIPLEXER;
				break;

			case 'N':
				if(BUFFER[2] == 'T')
					type = NOT;
				else if(BUFFER[2] == 'N')
					type = NAND;
				else
					type = NOR;
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
			fscanf(fp, "%d", &inputs);
			if(type == DECODER){
				(*Indirect)->NumOfIn = inputs;
				(*Indirect)->NumOfOut = Pow(2, inputs);
			}
			else {
				(*Indirect)->NumOfIn = (inputs + Pow(2,inputs));
				(*Indirect)->NumOfOut = 1;
			}
		}

		// Begin allocating space for our input parameters. The array will be int pointers so that we can refer to the
		// master variable table and quickly pull what value is currently stored there.
		(*Indirect)->inparam = malloc(sizeof(int*) * ((*Indirect)->NumOfIn));

		// Scan through the file and store those variables
		for(i = 0; i < (*Indirect)->NumOfIn; ++i){
			fscanf(fp, "%16s", BUFFER);
			// If it is a '0' , '1' , or '_' then point to the special case "binary" array. This array contains 0, 1,
			// and -1 for their respective symbols.
			if(BUFFER[0] == '0')
				(*Indirect)->inparam[i] = &(binary[0]);

			else if(BUFFER[0] == '1')
				(*Indirect)->inparam[i] = &(binary[1]);

			else if (BUFFER[0] == '_')
				(*Indirect)->inparam[i] = &(binary[2]);

			else {
				for(j = 0; j < Table.TempEnd; ++j) {
					// Look for the Variable, if we find it, set the pointer and stop.
					if(!StrComp(BUFFER, Table.Vars[j].VarName))
						(*Indirect)->inparam[i] = &(Table.Vars[j].value);
				}
			}
		}

		// Allocate space for pointers to our output variables using the same ideas as above.
		(*Indirect)->outparam = malloc(sizeof(int*) * ((*Indirect)->NumOfOut));
		for(i = 0; i < (*Indirect)->NumOfOut; ++i){
			fscanf(fp, "%16s", BUFFER);

			if(BUFFER[0] == '0')
				(*Indirect)->outparam[i] = &(binary[0]);

			else if(BUFFER[0] == '1')
				(*Indirect)->outparam[i] = &(binary[1]);

			else if (BUFFER[0] == '_')
				(*Indirect)->outparam[i] = &(binary[2]);
		
			else {
				for(j = 0; j < Table.TempEnd; ++j) {
					// Look for the Variable, if we find it, set the pointer and stop.
					if(!StrComp(BUFFER, Table.Vars[j].VarName)){
						(*Indirect)->outparam[i] = &(Table.Vars[j].value);
						break;
					}
				}
			}
		}

		// Just to make life simpler, I want the # of selectors of the Multiplexer, not total amount of inputs.
		// Right now NumOfIn = inputs + Pow(2,inputs), so get rid of that power so NumOfIn - Pow = inputs.
		if(type == MULTIPLEXER)
			(*Indirect)->NumOfIn -= Pow(2,inputs);
		
		// Once finished, move on to the next directive.
		Indirect = &((*Indirect)->next);
	}
}

/** This function runs through the gates and attempts to solve for the truthtable. */
void DoCircuit(struct Gate *First, struct VarTable Table)
{
	// Keep looping until we have no more gates.
	// If the output param holds a value of -1, discard the output.
	while(First != NULL){
		switch(First->type){

			case PASS:
				if(First->outparam[0][0] != -1)
					First->outparam[0][0] = First->inparam[0][0];

				break;
			
			case NOT:
				if(First->outparam[0][0] != -1)
					First->outparam[0][0] = (First->inparam[0][0] == 1) ? 0 : 1;

				break;

			case AND:
				if(First->inparam[0][0] == 1 && First->inparam[1][0] == 1)
					First->outparam[0][0] = 1;
				else
					if(First->outparam[0][0] != -1)
						First->outparam[0][0] = 0;

				break;

			case NAND:
				if(First->inparam[0][0] == 1 && First->inparam[1][0] == 1)
					First->outparam[0][0] = 0;
				else
					if(First->outparam[0][0] != -1)
						First->outparam[0][0] = 1;

				break;

			case NOR:
				if(First->inparam[0][0] == 1 || First->inparam[1][0] == 1)
					First->outparam[0][0] = 0;
				else
					if(First->outparam[0][0] != -1)
						First->outparam[0][0] = 1;

				break;

			case OR:
				if(First->inparam[0][0] == 1 || First->inparam[1][0] == 1)
					First->outparam[0][0] = 1;
				else
					if(First->outparam[0][0] != -1)
						First->outparam[0][0] = 0;

				break;

			case XOR:
				if( (First->inparam[0][0] == 1 && First->inparam[1][0] == 0) || 
					(First->inparam[0][0] == 0 && First->inparam[1][0] == 1))
						First->outparam[0][0] = 1;
				else
					if(First->outparam[0][0] != -1)
						First->outparam[0][0] = 0;

				break;

			case DECODER:
			{
				int i, incrementer = 1, bit = 0;

				for(i = 0; i < First->NumOfIn; ++i){
					if(First->inparam[i][0] == 1)
						bit += Pow(2, First->NumOfIn - incrementer);

					++incrementer;
				}

				if(First->outparam[bit][0] != -1)
					First->outparam[bit][0] = 1;

				break;
			}	

			case MULTIPLEXER:
			{
				int i, selectorindex = (Pow(2, First->NumOfIn));
				int totalinputs = selectorindex + First->NumOfIn;
				int row = 0, incrementer = 1;

				for(i = selectorindex; i < totalinputs; ++i){
					if(First->inparam[i][0] == 1)
						row += Pow(2, First->NumOfIn - incrementer);

					++incrementer;
				}

				if(First->outparam[0][0] != -1)
					First->outparam[0][0] = First->inparam[row][0];

				break;
			}
		}
		// Go to the following gate.
		First = First->next;
	}
	// Print the truth table for the current inputs.
	PrintTableValues(Table);
}

void SortGates(struct Gate **First, struct VarTable Table)
{
	int i, j, found, NumOfIn;
	int *TempAddr, *TableAddr;

	while(*First != NULL) {
		found = 0;
		// check for Multiplexer special case in which NumOfIn isn't simply the number attached to the gate.
		NumOfIn = ((*First)->type == 8) ? ((*First)->NumOfIn + Pow(2, (*First)->NumOfIn)) : (*First)->NumOfIn;
		
		// Iterate through all the inputs of the gate checking if they are temp vars.
		for(i = 0; i < NumOfIn; ++i){
			TempAddr = (*First)->inparam[i];

			// Go through all the Temporary Variables
			for(j = Table.OutputEnd; j < Table.TempEnd; ++j){
				TableAddr = &(Table.Vars[j].value);
				
				// If the pointers match, this must mean the input is a temporary variable.
				if(TempAddr == TableAddr){

					//Flag the temp Variable
					Table.Vars[j].value = 1;
					struct Gate **swap = &((*First)->next);
					
					// Go through the remaining gates and swap with the gate if the gate contains the flagged temporary 
					// variable in its output.
					while(*swap != NULL){
						int k;
						
						// Check if the current gate holds that flagged variable. If the gate does have that variable
						// in it's output, that must mean our "current" gate uses it as an input before "swap" assigns
						// it a value. Therefore flag it, and prepare to swap it.
						for(k = 0; k < ((*swap)->NumOfOut); ++k){
							if((*swap)->outparam[k][0] == 1){
								found = 1;
								break;
							}
						}
						
						// If found, swap the gates, else go to the next gate.
						if(found){
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
					// Set the flagged variable back to 0.
					Table.Vars[j].value = 0;
					
					// If we did do a swap, we are going to want to restart the process on the swapped gate.
					// We do this by setting loop conditions to false using the variables.
					if(found){
						j = Table.TempEnd;
						i = NumOfIn;
					}
				}
			}
		}
		// Only procced to the next gate if we didn't swap.
		if(!found)
			First = &((*First)->next);
	}
}

/** Function to iterate our inputs. Every iteration calls DoCircuit. */
void Solve_Truth_Table(struct Gate *First, struct VarTable Table)
{
	int i, start = (Table.InputEnd-1);
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
}

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

	// Initalize the Master Variable Table.
	struct VarTable Table;

	// Read in the Input/Output Variables and add them to the table.
	ReadIOVars(&Table, fp);
	
	// Get all the temporary variables and add them to the table.
	Search_For_Temps(&Table, fp);
	
	// Create the "First" gate and the constants values array
	struct Gate *First = NULL;
	int binary[] = {0 , 1, -1};

	rewind(fp);

	// Go through the file and start making the gates based on directives.
	CreateGates(&First, Table, binary, fp);
	fclose(fp);

	// Go through the gates and make sure they are in the right order.
	// (A temporary variable isn't being used before it is assigned a value.)
	SortGates(&First, Table);

	// Solve the circuit...
	Solve_Truth_Table(First, Table);	

	// ... free memory, and gracefully finish.
	FreeTable(Table);
	FreeGates(First);
	return 0;
}
