#include <stdio.h>
#include <stdlib.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

struct Variable {
	char	*VarName;
	int	index;
};

struct VarTable {
	struct Variable	**Vars;
	int		InputEnd;
	int		OutputEnd;
	int		TempEnd;
};

struct Gate {
	kind_t		type;
	int			*param;
	struct Gate	*next;
	int			NumOfParam;
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

	// Print Vars for debugging
	puts("All found vars:");
	for(i = 0; i < Table->OutputEnd; ++i)
		printf("%s\n", Table->Vars[i]->VarName);

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

	fclose(fp);


}
