#include <stdio.h>
#include <stdlib.h>

typedef enum { PASS, NOT, AND, NAND, NOR, OR, XOR, DECODER, MULTIPLEXER } kind_t;

// The master Variable table is made up of Variables. A single variable contains the variable name,
// the index of it in the current Table, and the value it is currently holding.
struct Variable {
	char	*VarName;
	int	index;
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
	struct Gate	*next;
	int	**inparam;
	int **outparam;
	int	NumOfIn;
	int NumOfOut;
	kind_t type;
};

// Circuit Methods to be implmented
/*
int NOT(int input1){}
int PASS(int input1){}
int AND(int input1, int input2){}
int OR(int input1, int input2){}
int NAND(int input1, int input2){}
int NOR(int input1, int input2){}
int XOR(int input1, int input2){}
*/

// ====================================== Utility Functions ====================================================

/** String Compare Method */
int StrComp(char *tocomp, char *given)
{
	int i = 0;
	while(tocomp[i] != '\0'){
		if(tocomp[i] != given[i])
			return 0;
		++i;
	}
	return 1;
}

/** String Copy */
void StrCopy(char *src, char *dest)
{
	int i = 0;
	while(src[i] != '\0'){
		dest[i] = src[i];
		++i;
	}
	dest[i] = '\0';
}

/** Math Power Function. Base ^ Exponent */
int Pow(int Base, int Exponent)
{
	int i, total = Base;
	for(i = 0; i < Exponent; ++i) total *= Base;
	return total;
}

/** Print the Variable Table */
void PrintTableVars(struct VarTable Table)
{
	int i;
	puts("All found Varibales:");
	for(i = 0; i < Table.TempEnd; ++i)
		printf("%s\n", Table.Vars[i].VarName);
}

void PrintTableValues(struct VarTable Table)
{
	int i;
	puts("VarTable Values:");
	for(i = 0; i < Table.InputEnd; ++i)
		printf("%d ", Table.Vars[i].value);

	printf("|");

	for(; i < Table.OutputEnd; ++i)
		printf("%d ", Table.Vars[i].value);
}

void PrintGates(struct Gate *First)
{
	while(First != NULL){
		printf("NumofIn: %d NumofOut: %d, type %d\n", First->NumOfIn, First->NumOfOut, First->type);
		First = First->next;
	}
}

// ====================================== End Of Utility Functions ====================================================

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

void Search_For_Temps(struct VarTable *Table, FILE *fp)
{
	char BUFFER[17];
	int NumOfIn = 2, NumOfOut = 1;
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
			if(BUFFER[0] == 'D') {
				NumOfOut = Pow(2, NumOfIn);
			}
			else {
				NumOfOut = 1;
				NumOfIn += Pow(2, NumOfIn);
			}
		}
		// If its not the previous options it must have 2 Inputs and 1 Output
		
		
		// Go through our current Variables and see if we already have that Variable.
		// If not, append it to our table.
		int i, j;
		for(i = 0; i < (NumOfIn + NumOfOut); ++i) {
			fscanf(fp, "%16s", BUFFER);
			if( !(BUFFER[0] == '1' || BUFFER[0] == '0' || BUFFER[0] == '_') ){
				for(j = 0; j < Table->TempEnd; ++j){
					if(StrComp(BUFFER, Table->Vars[j].VarName))
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

void CreateGates(struct Gate **First, struct VarTable Table, int *binary, FILE *fp)
{
	struct Gate **Indirect = First;
	char BUFFER[17];
	kind_t type;
	// Skip the first two lines.
	char SKIP[16384];
	fgets(SKIP, 16384, fp);
	fgets(SKIP, 16384, fp);
	while(fscanf(fp, "%16s", BUFFER) != EOF) {
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

		if(type < 2){
			(*Indirect)->NumOfIn = 1;
			(*Indirect)->NumOfOut = 1;
		}
		else if(type < 7){
			(*Indirect)->NumOfIn = 2;
			(*Indirect)->NumOfOut = 1;
		}
		else {
			int inputs;
			fscanf(fp, "%d", &inputs);
			if(type == DECODER){
				(*Indirect)->NumOfIn = inputs;
				(*Indirect)->NumOfOut = Pow(2, inputs);
			}
			else {
				(*Indirect)->NumOfIn = (inputs + Pow(2, inputs));
				(*Indirect)->NumOfOut = 1;
			}
		}

		(*Indirect)->inparam = malloc(sizeof(int*) * ((*Indirect)->NumOfIn));
		int i, j;
		for(i = 0; i < (*Indirect)->NumOfIn; ++i){
			fscanf(fp, "%16s", BUFFER);
			if(BUFFER[0] == '0') (*Indirect)->inparam[i] = &(binary[0]);
			else if(BUFFER[0] == '1') (*Indirect)->inparam[i] = &(binary[1]);
			else if (BUFFER[0] == '_') (*Indirect)->inparam[i] = &(binary[2]);
			else {
				for(j = 0; j < Table.TempEnd; ++j) {
					if(StrComp(BUFFER, Table.Vars[j].VarName))
						(*Indirect)->inparam[i] = &(Table.Vars[j].value);
				}
			}
		}
		(*Indirect)->outparam = malloc(sizeof(int*) * ((*Indirect)->NumOfOut));
		for(i = 0; i < (*Indirect)->NumOfOut; ++i){
			fscanf(fp, "%16s", BUFFER);
			if(BUFFER[0] == '0') (*Indirect)->outparam[i] = &(binary[0]);
			else if(BUFFER[0] == '1') (*Indirect)->outparam[i] = &(binary[1]);
			else if (BUFFER[0] == '_') (*Indirect)->outparam[i] = &(binary[2]);
			else {
				for(j = 0; j < Table.TempEnd; ++j) {
					if(StrComp(BUFFER, Table.Vars[j].VarName))
						(*Indirect)->outparam[i] = &(Table.Vars[j].value);
				}
			}
		}
		Indirect = &((*Indirect)->next);
	}
		return;
}

void SolveCircuit(struct Gate *First, struct VarTable Table)
{
	
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
	
	struct VarTable Table;
	ReadIOVars(&Table, &fp);
	puts("Read IO Vars");
	
	Search_For_Temps(&Table, fp);
	puts("Read all temps");
	
	PrintTableVars(Table);
	
	struct Gate *First = malloc(sizeof(struct Gate));
	int binary[] = {0 , 1, -1};
	rewind(fp);
	CreateGates(&First, Table, binary, fp);
	PrintGates(First);
	
	
	fclose(fp);

	return 0;
}
