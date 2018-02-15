#include <assert.h>
#include <optread.h>
#include <stdio.h>
#include <stdlib.h>

/*!
 * @brief Tests the behavior
 * @test Run to check the behavior when running arguments without options
 */
int main()
{
	char retval;
	optread_crawl_state *state = calloc(1, sizeof(optread_crawl_state));

	retval = optread_crawl(0, 0, state);
	assert(retval == 0);
	assert(state->type == OPTREAD_TYPE_NO_PARAM);
	assert(state->param == 0);
	assert(state->option.s == 0);

	return 0;
}

