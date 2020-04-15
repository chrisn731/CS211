#include <stdio.h>
#include <stdlib.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

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
	int	NumOfParam;
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
			if( !(BUFFER[0] == '1' || BUFFER[0] == '0' || BUFFER[0] == '_') )
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
}

void CreateGates(struct Gate **First)
{
	return;
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
	CreateGates(&First);
	fclose(fp);

}
