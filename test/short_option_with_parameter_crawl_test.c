#include <assert.h>
#include <optread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!
 * @brief Ensure that parameters are identified as such with short options
 */
int main()
{
	int argc;
	const int param_num = 2;
	char argv_base[2][10] = {"-o", "I'm an arg"};
	char **argv = calloc(param_num, sizeof(char*));
	optread_crawl_state *state = calloc(1, sizeof(optread_crawl_state));
	int i, len;
	int cmp_result;
	argc=param_num;

	/* Set argv from reference (argv_base) */
	for(i=0; i<param_num; i++)
	{
		len = strlen(argv_base[i])+1;
		/* strdup not defined with -ansi */
		argv[i] = calloc(len, sizeof(char));
		memcpy(argv[i], argv_base[i], len*sizeof(char));
	}

	i=0;
	while(optread_crawl(argc, argv, state))
	{
		/* The correct option type is returned */
		assert(state->type == OPTREAD_TYPE_SHORT_OPTION);
		/* The correct parameter string is found */
		assert(state->option.c == argv_base[i][1]);
		cmp_result = strcmp(state->param, argv_base[i+1]);
		assert(0 == cmp_result);
		/* Tell optread_crawl that the argument was used */
		state->state = OPTREAD_STATE_ARGUMENT_USED;
		i++;
	}
	assert(state->type == OPTREAD_TYPE_NO_PARAM);
	assert(i==1);

	/* free memory */
	for(i=0; i<param_num; i++)
	{
		free(argv[i]);
	}
	free(argv);
	free(state);

	return 0;
}

