#include <stdio.h>
#include <stdlib.h>

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

struct Variable {
	char	*VarName;
	int		index;
};

struct VarTable {
	struct Variable		**Vars;
	int 				InputEnd;
	int					OutputEnd;
	int					TempEnd;
};

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

void ReadCircuit(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if(!fp) {
		printf("Error Opening file");
		exit(EXIT_FAILURE);
	}

	// Scan the 'INPUT' string
	char IOBUF[7];
	fscanf(fp, "%s", IOBUF);
	int i, NumOfInputs;
	fscanf(fp, "%d", &NumOfInputs);
	
	struct VarTable Table;
	Table.InputEnd = NumOfInputs;
	Table.Vars = malloc(sizeof(struct Variable*) * NumOfInputs);

	for(i = 0; i < NumOfInputs; ++i) {
		char *var = malloc(sizeof(char) * 17);
		fscanf(fp, "%16s", var);
		struct Variable *temp = malloc(sizeof(struct Variable));
		temp->VarName = var;
		temp->index = i;
		Table.Vars[i] = temp;
	}

	// Scan the 'OUTPUT' string
	fscanf(fp, "%s", IOBUF);

	fscanf(fp, "%d", &NumOfInputs);
	Table.Vars = realloc(Table.Vars, sizeof(struct Variable*) * (NumOfInputs + Table.InputEnd));
	Table.OutputEnd = NumOfInputs + Table.InputEnd;

	for(; i < Table.OutputEnd; ++i) {
		char *var = malloc(sizeof(char) * 17);
		fscanf(fp, "%16s", var);
		struct Variable *temp = malloc(sizeof(struct Variable));
		temp->VarName = var;
		temp->index = i;
		Table.Vars[i] = temp;
	}

	puts("All found vars:");
	for(i = 0; i < Table.OutputEnd; ++i)
		printf("%s\n", Table.Vars[i]->VarName);


	fclose(fp);

}

int main(int argc, char *argv[])
{
	if(argc != 2) {
		puts("Error: Missing Inputs. Usage: ./truthtable [pathtocircuit]");
		return 1;
	}

	ReadCircuit(argv[1]);	
	
}
