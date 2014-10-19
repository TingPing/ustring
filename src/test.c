#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ustring.h"

static void
test_ustrlen (void)
{
	assert (ustrlen ("©你好abc") == 6);
	assert (ustrlen ("") == 0);
	ustrlen (NULL);
	
	puts ("ustrlen() passed.");
}

static void
test_ucharlen (void)
{
	assert (ucharlen ("你好") == 3);
	assert (ucharlen ("") == 1);
	ucharlen (NULL);

	puts ("ucharlen() passed.");
}

static void
test_ustrlcpy (void)
{
	char tmp[5];
	size_t ret;

	ret = ustrlcpy (tmp, "©你好abc", sizeof(tmp));
	assert (ret > sizeof(tmp)); // is truncated
	assert (strcmp (tmp, "©") == 0);

	assert (ustrlcpy (tmp, "", 1) == 0);

	// Test invalid inputs
	ustrlcpy (NULL, "a", 1);
	ustrlcpy (tmp, NULL, 1);
	ustrlcpy (tmp, "a", 0);
	
	puts ("ustrlcpy() passed.");
}

static void
test_ustrlcat (void)
{
	char tmp[8] = "你";
	size_t ret;

	ret = ustrlcat (tmp, "好好", sizeof(tmp));
	assert (ret > sizeof(tmp));
	assert (strcmp (tmp, "你好") == 0);

	ustrlcat (NULL, "a", 1);
	ustrlcat (tmp, NULL, 1);
	ustrlcat (tmp, "a", 0);
	
	puts ("ustrlcat() passed.");
}

int
main (void)
{
	test_ucharlen ();
	test_ustrlen ();
	test_ustrlcpy ();
	test_ustrlcat ();

	puts ("All tests passed.");
	return EXIT_SUCCESS;
}
