#include <string>
#include <fstream>

extern "C" {
#include "postgres.h"
#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
	PG_MODULE_MAGIC;
#endif

	/* Add a prototype marked PGDLLEXPORT */
	PGDLLEXPORT Datum add_one(PG_FUNCTION_ARGS);
	PGDLLEXPORT Datum save_to_file(PG_FUNCTION_ARGS);

	PG_FUNCTION_INFO_V1(add_one);
	PG_FUNCTION_INFO_V1(save_to_file);
}

Datum
add_one(PG_FUNCTION_ARGS)
{
	int32   arg = PG_GETARG_INT32(0);

	PG_RETURN_INT32(arg + 1);
}

Datum save_to_file(PG_FUNCTION_ARGS)
{
	bytea *array = PG_GETARG_BYTEA_PP(0);
	text* fileNameText = PG_GETARG_TEXT_PP(1);

	size_t filenameLen = VARSIZE(fileNameText) - VARHDRSZ;
	std::string fileName(static_cast<const char*>(VARDATA(fileNameText)), filenameLen);

	const auto arraySize = VARSIZE(array) - VARHDRSZ;
	std::ofstream file(fileName, std::ios::binary);

	file.write(static_cast<const char*>(VARDATA(array)), arraySize);
	
	PG_RETURN_NULL();
}
