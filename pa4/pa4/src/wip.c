#include <stdio.h>
#include <stdlib.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

struct Variable {
	char	*VarName;
	int	index;
};

struct VarTable {
	struct Variable	**Vars;
	int	InputEnd;
	int	OutputEnd;
	int	TempEnd;
};

struct Gate {
	struct Gate	*next;
	int	*param;
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
		printf("%s\n", Table.Vars[i]->VarName);
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
	Table->Vars = malloc(sizeof(struct Variable*) * NumOfInputs);
	
	// Read the Input Vars and store them in the Variable struct
	for(i = 0; i < NumOfInputs; ++i) {
		char *var = malloc(sizeof(char) * 17);
		fscanf(*fp, "%16s", var);
		struct Variable *temp = malloc(sizeof(struct Variable));
		temp->VarName = var;
		temp->index = i;
		Table->Vars[i] = temp;
	}

	// Scan the 'OUTPUT' string
	fscanf(*fp, "%s", IOBUF);

	fscanf(*fp, "%d", &NumOfInputs);
	Table->Vars = realloc(Table->Vars, sizeof(struct Variable*) * (NumOfInputs + Table->InputEnd));
	Table->OutputEnd = NumOfInputs + Table->InputEnd;

	// Read the Output Vars and store them in the Variable struct
	for(; i < Table->OutputEnd; ++i) {
		char *var = malloc(sizeof(char) * 17);
		fscanf(*fp, "%16s", var);
		struct Variable *temp = malloc(sizeof(struct Variable));
		temp->VarName = var;
		temp->index = i;
		Table->Vars[i] = temp;
	}
}

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
			if(BUFFER[0] == 'D') {
				NumOfOut = Pow(2, NumOfIn);
			}
			else {
				NumOfOut = 1;
				NumOfIn += Pow(2, NumOfIn);
			}
		}
		// If its not the previous options it must have 2 Inputs and 1 Output
		else {
			NumOfIn = 2;
			NumOfOut = 1;
		}
		// Go through our current Variables and see if we already have that Variable.
		// If not, append it to our table.
		int i, j;
		for(i = 0; i < (NumOfIn + NumOfOut); ++i) {
			fscanf(fp, "%16s", BUFFER);
			if(BUFFER[0] == '1' || BUFFER[0] == '0' || BUFFER[0] == '_')
				continue;
			else {
				for(j = 0; j < Table->TempEnd; ++j){
					if(StrComp(BUFFER, Table->Vars[j]->VarName))
						break;
				}
				if(j == Table->TempEnd) {
					char *var = malloc(sizeof(BUFFER));
					StrCopy(BUFFER, var);
					struct Variable *temp = malloc(sizeof(struct Variable));
					temp->VarName = var;
					temp->index = Table->TempEnd;
					Table->Vars = realloc(Table->Vars, sizeof(struct Variable*) * (1 + Table->TempEnd));
					Table->Vars[Table->TempEnd] = temp;
					++(Table->TempEnd);
				}
			}
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
	
	struct VarTable Table;
	ReadIOVars(&Table, &fp);
	Search_For_Temps(&Table, fp);
	PrintTableVars(Table);
	fclose(fp);


}
