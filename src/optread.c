/*!
 * optread version 0.1
 * Licence Creative Commons BY
 * @author Ayowel
 */

#include "optread.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*!
 * Use to iterate over a list of parameters received
 *
 * @param argc [in] Number of parameters to parse
 * @param argv [in/out] Array of parameters to parse
 * @param state [in/out] Extracted data & return value
 * @note argv is only an input if {@link OPTREAD_REFUSE_ARGUMENT_LIST_OWNERSHIP} is defined
 */
char optread_crawl(int argc, char** argv, struct optread_crawl_state* state)
{
	size_t length;
	#if defined(OPTREAD_LONG_VALUE_CHAINING_ENABLE) && ! defined(OPTREAD_LONG_DISABLE)
	char* subref;
	#endif

	#if defined(OPTREAD_SHORT) && defined(OPTREAD_SHORT_OPTION_CHAINING)
	if(state->pos.info & OPTREAD_STATE_SHORT_CHAINING)
	{
		if(argv[state->pos.index][state->pos.subindex] == '\0')
		{
			state->pos.index++;
			state->pos.subindex = 0;
			state->pos.info &= ~OPTREAD_STATE_SHORT_CHAINING;
		}
		else
		{
			if(state->state & OPTREAD_STATE_ARGUMENT_USED)
			{
				state->param = 0;
				state->state &= ~OPTREAD_STATE_ARGUMENT_USED;
			}
			state->option.c = argv[state->pos.index][state->pos.subindex];
			state->pos.subindex++;
			state->state = OPTREAD_STATE_DEFAULT;
			return 1;
		}
	}
	#endif

	if((state->state & OPTREAD_STATE_ARGUMENT_USED)
		&& ! (state->pos.info & OPTREAD_STATE_INNER_CHAINING)
		)
	{
		state->state &= ~OPTREAD_STATE_ARGUMENT_USED;
		state->pos.info &= ~OPTREAD_STATE_INNER_CHAINING;
		state->pos.index++;
	}

	if (state->pos.index >= argc) {
		state->type = OPTREAD_TYPE_NO_PARAM;
		state->param = 0;
		memset(&(state->option), 0, sizeof(optread_optname));
		return 0;
	}

	length = strlen(argv[state->pos.index]);

	#ifndef OPTREAD_LONG_DISABLE
	if (length > sizeof(OPTREAD_LONG_PREFIX)-1
		&& 0 == memcmp(argv[state->pos.index], OPTREAD_LONG_PREFIX, sizeof(OPTREAD_LONG_PREFIX)-1)
		)
	{
		#ifdef OPTREAD_LONG_VALUE_CHAINING_ENABLE
		subref = strstr(argv[state->pos.index]+sizeof(OPTREAD_LONG_PREFIX)-1, OPTREAD_LONG_CHAINING_SEPARATOR);
		if(subref != 0) {
			state->option.s = argv[state->pos.index]+sizeof(OPTREAD_LONG_PREFIX)-1;
			state->param = subref + sizeof(OPTREAD_LONG_CHAINING_SEPARATOR)-1;
			state->type = OPTREAD_TYPE_LONG_OPTION;
			state->state = OPTREAD_STATE_DEFAULT;
			#ifndef OPTREAD_REFUSE_ARGUMENT_LIST_OWNERSHIP
			*subref = '\0';
			#endif
			state->pos.index++;
			return 1;
		}
		#endif
		#if defined(OPTREAD_LONG_VALUE_CHAINING_ENABLE) && ! defined(OPTREAD_LONG_VALUE_CHAINING_ENFORCE)
		else
		#endif
		#if ! defined(OPTREAD_LONG_VALUE_CHAINING_ENFORCE)
		{
			state->option.s = argv[state->pos.index]+sizeof(OPTREAD_LONG_PREFIX)-1;
			state->param = argc > state->pos.index+1 ? argv[state->pos.index+1] : 0;
			state->type = OPTREAD_TYPE_LONG_OPTION;
			state->state = OPTREAD_STATE_DEFAULT;
			state->pos.index++;
			return 1;
		}
		#endif
	}
	#endif
	#ifndef OPTREAD_SHORT_DISABLE
	/* Look for a short parameter */
	if (length >= sizeof(OPTREAD_SHORT_PREFIX)
		&& 0 == memcmp(argv[state->pos.index], OPTREAD_SHORT_PREFIX, sizeof(OPTREAD_SHORT_PREFIX)-1)
		)
	{
		state->param = argc > state->pos.index+1 ? argv[state->pos.index+1] : 0;
		#ifdef OPTREAD_SHORT_OPTION_CHAINING
		state->pos.subindex = sizeof(OPTREAD_SHORT_PREFIX);
		state->option.c = argv[state->pos.index][sizeof(OPTREAD_SHORT_PREFIX)-1];
		state->type = OPTREAD_TYPE_SHORT_OPTION;
		state->pos.info = OPTREAD_STATE_SHORT_CHAINING;
		#else
		state->pos.index++;
		#endif
		return 1;
	}
	#endif

	/* Return as regular parameter if nothing was found */
	state->type = OPTREAD_TYPE_NO_OPTION;
	memset(&(state->option), 0, sizeof(optread_optname));
	state->param = argv[state->pos.index];
	state->state = OPTREAD_STATE_DEFAULT;
	state->pos.index++;
	return 1;
}

