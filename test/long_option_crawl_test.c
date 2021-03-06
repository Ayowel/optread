#include <assert.h>
#include <optread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!
 * @brief Check behavior when using long arguments
 */
int main()
{
	int argc;
	const int param_num = 1;
	char argv_base[1][15] = {"--my-option"};
	char **argv = calloc(param_num, sizeof(char*));
	optread_crawl_state *state = calloc(1, sizeof(optread_crawl_state));
	int i, len;
	int cmp_result;
	argc=param_num;

	/* Set argv from reference (argv_base) */
	for(i=0; i<param_num; i++)
	{
		/* strdup not defined with -ansi */
		len = strlen(argv_base[i])+1;
		argv[i] = calloc(len, sizeof(char));
		memcpy(argv[i], argv_base[i], len*sizeof(char));
	}

	i=0;
	while(optread_crawl(argc, argv, state))
	{
		/* The correct option type is returned */
		assert(state->type == OPTREAD_TYPE_LONG_OPTION);
		/* The correct parameter string is found */
		cmp_result = strcmp(state->option.s, argv_base[i]+2);
		assert(0 == cmp_result);
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

