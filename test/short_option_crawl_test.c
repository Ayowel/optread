#include <assert.h>
#include <optread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!
 * @brief Tests the behavior
 * @test Run to check the behavior when running arguments without options
 */
int main()
{
	int argc;
	const int param_num = 1;
	char argv_base[1][10] = {"-o"};
	char **argv = calloc(param_num, sizeof(char*));
	optread_crawl_state *state = calloc(1, sizeof(optread_crawl_state));
	int i, len;
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
		assert(state->param == 0);
		i++;
	}
	assert(state->type == OPTREAD_TYPE_NO_PARAM);
	assert(i==param_num);

	/* free memory */
	for(i=0; i<param_num; i++)
	{
		free(argv[i]);
	}
	free(argv);
	free(state);

	return 0;
}

