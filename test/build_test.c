#include <optread.h>
#include <stdio.h>
#include <stdlib.h>

/*!
 * @brief Tests the behavior
 * @test Run to check the behavior when running arguments without options
 */
int main()
{
	optread_crawl_state *state = calloc(1, sizeof(optread_crawl_state));
	printf("%ld\n", sizeof(optread_crawl_state));
	free(state);

	return 0;
	(void) state;
}

