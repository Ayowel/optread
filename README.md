# OPTREAD ![Issues](https://img.shields.io/github/issues/Ayowel/travis-test.svg) ![License](https://img.shields.io/github/license/Ayowel/optread.svg) ![Status](https://img.shields.io/badge/status-in%20development-red.svg) ![Travis](https://travis-ci.org/Ayowel/optread.svg?branch=master)

## Description

This repository features a small C library to parse arguments received as parameters of a program.

## Usage

As of now, the only way to use the library is to call the `optread_crawl` function:

```C
#include <optread.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
  /* Base structure used to extract information */
  optread_crawl_state *state = calloc(1, sizeof(optread_crawl_state));

  while(optread_crawl(argc-1, argv+1, state)) /* Returns 1 while a parameter is found */
  {
    switch(state->type)
    {
      /*
       * Possible values:
       * * OPTREAD_TYPE_NO_PARAMETER, should not appear before end of loop
       * * OPTREAD_TYPE_NO_OPTION, if only a parameter was found (state->param)
       * * OPTREAD_TYPE_SHORT_OPTION, if the option is a character (use state->option.c, state->param might equal 0
       * * 
       */
      case OPTREAD_TYPE_NO_OPTION:
        printf("Received parameter %s\n", state->param);
        break;
      case OPTREAD_TYPE_SHORT_OPTION:
        printf("Received short parameter '%c'", state->option.c);
        if(state->param != 0)
        {
          printf(" with value '%s'\n", state->param);
          state->state = OPTREAD_STATE_ARGUMENT_USED; /* do NOT set if the parameter of the option is not used */
        }
        else
        {
          printf("\n");
        }
        break;
      case OPTREAD_TYPE_LONG_OPTION:
        printf("Received long parameter '%s'", state->option.s);
        if(state->param != 0)
        {
          printf(" with value '%s'\n", state->param);
          state->state = OPTREAD_STATE_ARGUMENT_USED; /* do NOT set if the parameter of the option is not used */
        }
        else
        {
          printf("\n");
        }
        break;
    }
  }
  printf("All done\n");
  free(state);
  return 0;
}
```

The code makes use of defines to generate the minimum amount of code necessary. Rebuild to fit yout needs.

### Advanced use

![Caution](https://img.shields.io/badge/!-caution-red.svg) Use with care, Changing those defines SHOULD work but is not guaranteed as of now.

* **OPTREAD\_SHORT\_PREFIX** String used to detect a short option, defaults to "-"
* **OPTREAD\_LONG\_PREFIX** String used to detect a long option, defaults to "--"

![Caution](https://img.shields.io/badge/!-caution-red.svg) Use with care, Changing/creating those defines MIGHT work but is by no mean guaranteed as of now. Using those might cause compile-time and/or runtime errors.

Non-exhaustive list:

* **OPTREAD\_SHORT\_DISABLE** Disables the use of short prefixes if defined
* **OPTREAD\_LONG\_DISABLE** Disables the use of long prefixes if defined
* **OPTREAD\_SHORT\_OPTION\_CHAINING\_DISABLE** Using -azer will only detect 'a' as an option and bypass the rest
* **OPTREAD\_LONG\_VALUE\_CHAINING\_ENABLE** Allows the user to write option/value pairs like so: "--myarg=myvalue"
* **OPTREAD\_LONG\_CHAINING\_SEPARATOR** Change long chaining split string in "--myarg=myvalue" (defaults to "=")

## Contribute

Want to add/update something ? Feel free to create issues and/or Pull Requests but keep some stuff in mind:

* Keep the code compatible with C90
* Indents are tabs
* If the travis build fails, the code won't be merged
    * If the travis build fails due to new tests that should work and that no code has been added, the PR will be merged
* If a build generates a warning, the code won't be merged
* If a part of code should not be executed in a certain configuration, it should not be present in the compiled file

Check out the TODO list hereafter, that way you'll know what is to come / get ideas to develop upon.

## TODO

- [ ] Set param to 0 if already used in short option chaining
    - Prevents the use of the same argument in different contexts due to a user error
- [ ] Check that optread.c builds to optread.o with all accepted configurations
    - Self-explanatory
    - Minus defines that provide strings. It requires testing, but there is no need to test all possible strings: we still need the build to end.
- [ ] Handle end of options string ("--")
    - Allow the disabling of the option if OPTREAD\_DISABLE\_OPTIONS\_OPTION\_DISABLE is defined (or another somewhat meaningful name)
    - Any subsequent option should be treated as a regular argument
- [ ] Add tests with other configurations (continuous)
    - Be it in terms of parameters of defines at build
- [ ] Improve the testing process
    - Allow the use of different configurations with a single .c
    - Create input/expected output pairs to check conformity instead of in-code raw tests
- [ ] Improve Makefile management
    - Split it in several Makefiles
    - Make it more clean
    - Kick me for doing silly stuff in there
- [ ] Use make to generate documentation
    - Add a 'doc' option that builds at least html doc
- [ ] Implement `void optread_scan(int, char**, optread_config*)`
    - Takes argc, argv and a configuration as argument
    - configuration should be set with lists of parameters and callback functions
    - Callback function prototype: `char callback(uint8_t opt_type, optread_optname option, char* param, void* context)`
- [ ] Enything you feel is usefull, really.

