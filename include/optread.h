#ifndef __OPTREAD_H__
#define __OPTREAD_H__

#include <stdint.h>

#ifdef OPTREAD_DOCUMENT
	/* Used for documentation purpose.
	Only define user-accessible macros not defined or not accessible*/
	/*!
	 * @addtogroup USER_ACCESSIBLE
	 * @{
	 */
	/*! @brief Disables short options if defined */
	#define OPTREAD_SHORT_DISABLE
	/*! @brief Disables long options if defined */
	#define OPTREAD_LONG_DISABLE
	/*!
	 * @brief Allows the use of a single param to set both option & it's value
	 * @see OPTREAD_SHORT_OPTION_CHAINING_DISABLE
	 * @see OPTREAD_LONG_VALUE_CHAINING_ENFORCE
	 */
	#define OPTREAD_LONG_VALUE_CHAINING_ENABLE
	/*!
	 * @brief Treat parameters that do not provide an option/value pair as regular parameters
	 * @warning Behavior is undefined if { @link OPTREAD_LONG_VALUE_CHAINING_ENFORCE } is defined but { @link OPTREAD_LONG_VALUE_CHAINING_ENABLE } is not
	 * @see OPTREAD_LONG_VALUE_CHAINING_ENABLE
	 */
	#define OPTREAD_LONG_VALUE_CHAINING_ENFORCE
	/*!
	 * @brief Forbid the use of more than one short option in a single parameter
	 * @see OPTREAD_SHORT_DISABLE
	 */
	#define OPTREAD_SHORT_OPTION_CHAINING_DISABLE
	/*!
	 * @brief Forbid the change of argv strings
	 * @warning When long option value chaining is enabled, parsing the option might require extra work on the developper's side
	 */
	 #define OPTREAD_REFUSE_ARGUMENT_LIST_OWNERSHIP
	/*! @} */
#endif
#if defined(OPTREAD_DOCUMENT) || !defined(OPTREAD_SHORT_DISABLE)

	#define OPTREAD_SHORT
	#ifndef OPTREAD_SHORT_PREFIX
		/*!
		 * @brief String used as a prefix for short options
		 * @see OPTREAD_SHORT_DISABLE
		 * @ingroup USER_ACCESSIBLE
		 */
		#define OPTREAD_SHORT_PREFIX "-"
	#endif
#endif
#if defined(OPTREAD_DOCUMENT) || !defined(OPTREAD_LONG_DISABLE)
	#define OPTREAD_LONG
	#ifndef OPTREAD_LONG_PREFIX
		/*!
		 * @brief String used as a prefix for short options
		 * @see OPTREAD_SHORT_DISABLE
		 * @ingroup USER_ACCESSIBLE
		 */
		#define OPTREAD_LONG_PREFIX "--"
	#endif
#endif

#if defined(OPTREAD_LONG_VALUE_CHAINING_ENABLE) || defined(OPTREAD_DOCUMENT)
	#define OPTREAD_LONG_VALUE_CHAINING
	#ifndef OPTREAD_LONG_CHAINING_SEPARATOR
		/*!
		 * @brief String used as a prefix for long options
		 * @ingroup USER_ACCESSIBLE
		 */
		#define OPTREAD_LONG_CHAINING_SEPARATOR "="
	#endif
#endif

#if defined(OPTREAD_DOCUMENT) || !defined(OPTREAD_DISABLE_OPTIONS_OPTION_DISABLE)
	#ifndef OPTREAD_DISABLE_OPTIONS_OPTION
		/*!
		 * @brief String used to treat following options as parameters
		 * @ingroup USER_ACCESSIBLE
		 */
		#define OPTREAD_DISABLE_OPTIONS_OPTION "--"
	#endif
#endif

#ifndef OPTREAD_SHORT_OPTION_CHAINING_DISABLE
	#define OPTREAD_SHORT_OPTION_CHAINING
#endif

#if defined(OPTREAD_SHORT_DISABLE) && defined(OPTREAD_LONG_DISABLE)
#error "OPTREAD_SHORT_DISABLE and OPTREAD_LONG_DISABLE may not be defined at the same time."
#endif
#if defined(OPTREAD_SHORT_OPTION_CHAINING_DISABLE) && defined(OPTREAD_SHORT_VALUE_CHAINING)
#error "OPTREAD_SHORT_OPTION_CHAINING_DISABLE and OPTREAD_SHORT_VALUE_CHAINING may not be defined at the same time."
#endif

/* Values used for state management */
/*! @brief Internal use. Indicates that we're currently working on a chain of short parameters (e.g.: ls -al) */
#define OPTREAD_STATE_SHORT_CHAINING 1
/*! @brief User-settable. Indicates that the value associated with the option has been consumed */
#define OPTREAD_STATE_ARGUMENT_USED 2
/*! @brief Internal use. Indicates that the the option/value pair we're working with were part of the same argument (e.g.: gcc -std=c++11) */
#define OPTREAD_STATE_INNER_CHAINING 4
/*! @brief Internal use. Indicates that options should be treated as regular parameters */
#define OPTREAD_STATE_DISABLE_OPTIONS 8
/*! @brief Internal use. Default state */
#define OPTREAD_STATE_DEFAULT 0
/* End */

/*!
 * @brief Used to hold an option information
 * @todo Ensure there is a real benefit to the use of this union
 * @deprecated This structure might be changed in the near future, use { @link OPTREAD_OPTNAME_TOSTR } for compatibility
 */
union optread_optname
{
	/*! @brief Short option */
	char c;
	/*! @brief Long option */
	char* s;
};
typedef union optread_optname optread_optname;

/*!
 * @brief Macro set-up for backward-compatibility with expected changes of { @link optread_optname }
 * @param opt Instance of { @link optread_optname }
 * @param select Equals { @link OPTREAD_TYPE_SHORT_OPTION } or { @link OPTREAD_TYPE_LONG_OPTION }
 * @return char* A pointer to a char
 */
#define OPTREAD_OPTNAME_TOSTR(opt, select) ((select)==OPTREAD_TYPE_SHORT_OPTION?&(opt.c):(opt.s))

/*! @brief Internal use. Stores crawl information for an iteration */
struct optread_crawl_position {
	/*! @brief String to work with */
	int index;
	/*!
	 * @brief Position in the current string
	 * @note Only used for short arg chaining atm
	 * @todo Remove from struct when unnecessary
	 */
	unsigned int subindex;
	/*!
	 * @brief Stores info to handle subsequent crawl calls
	 * @see OPTREAD_STATE_SHORT_CHAINING
	 * @see OPTREAD_STATE_INNER_CHAINING
	 */
	uint8_t info;
};
typedef struct optread_crawl_position optread_crawl_position;

/*!
 * @brief No parameter was found
 * @see optread_crawl_state.type
 */
#define OPTREAD_TYPE_NO_PARAM 0
/*!
 * @brief A parameter not associated with an option was found
 * @see optread_crawl_state.type
 */
#define OPTREAD_TYPE_NO_OPTION 1
/*!
 * @brief A short option was found
 * @see optread_crawl_state.type
 */
#define OPTREAD_TYPE_SHORT_OPTION 2
/*!
 * @brief Indicates a long option
 * @see optread_crawl_state.type
 */
#define OPTREAD_TYPE_LONG_OPTION 4
/*!
 * @brief Indicates a list of options
 * @note Might be removed, not used atm.
 */
#define OPTREAD_TYPE_LIST_OPTION 8
/*!
 * @brief Indicates an option type, test against {@link OPTREAD_TYPE_SHORT_OPTION}, {@link OPTREAD_TYPE_LONG_OPTION} or {@link OPTREAD_TYPE_LIST_OPTION}
 * @see OPTREAD_TYPE_SHORT_OPTION
 * @see OPTREAD_TYPE_LONG_OPTION
 * @see OPTREAD_TYPE_LIST_OPTION
 */
#define OPTREAD_TYPE_MASK_OPTION (OPTREAD_TYPE_SHORT_OPTION | OPTREAD_TYPE_LONG_OPTION | OPTREAD_TYPE_LIST_OPTION)

/*!
 * @brief Stores the state while iterating over input parameters
 * @note Instanciate with calloc
 * @see optread_crawl
 */
typedef struct optread_crawl_state {
	/*! @brief Internal use. Used to resume crawl */
	optread_crawl_position pos;
	/*!
	 * @brief Information on last crawl result
	 * @see OPTREAD_TYPE_NO_PARAM
	 * @see OPTREAD_TYPE_NO_OPTION
	 * @see OPTREAD_TYPE_SHORT_OPTION
	 * @see OPTREAD_TYPE_LONG_OPTION
	 */
	uint8_t type;
	/*!
	 * @brief Whether the option argument was used in the previous iteration
	 *
	 * Change if not coherent with actual use before next call to { @link optread_crawl }.
	 * Default value depending on the use of { @link OPTREAD_EXPAND_OPTIONS_DISABLE }.
	 * @see OPTREAD_STATE_ARGUMENT_USED
	 */
	uint8_t state;
	/*!
	 * @brief Option if one was found
	 * @see optread_crawl_state.type
	 */
	optread_optname option;
	/*!
	 * @brief Parameter value found. Might be equal to 0 if none were found
	 * @note Look-up { @link optread_crawl_state.type }'s value to know which element to use
	 */
	char *param;
}optread_crawl_state;

/*!
 * @brief find next available parameter
 * @param argc [in] Number of arguments received
 * @param argv [in] List of strings to go over
 * @param state [in/out] Crawl state
 * @return Whether a parameter was found (1) or the end or the list was reached (0)
 */
char optread_crawl(int argc, char** argv, struct optread_crawl_state* state);

#endif
