#pragma once
#include <squirrel.h>

#include "kbtable.h"
#include "kbclass.h"
#include "kbfunction.h"
#include "kbconst.h"
#include "kbutil.h"
#include "kbscript.h"
#include "kbarray.h"

#ifndef SQLIBAPI
#define SQLIBAPI extern
#endif

class SQILibInterface
{
public:
	virtual const SQChar* GetLibName() = 0;
	virtual const SQChar* GetLibAuthor() = 0;

	virtual int GetLibVersion() = 0;
	virtual const SQChar* GetLibVersionChar() = 0;

	virtual const SQChar* GetLibInfo() = 0;

	virtual SQRESULT Initialize(HSQUIRRELVM v) = 0;
};

extern "C" {
	SQLIBAPI SQILibInterface* GetInterface();
}

using GetInterface_t = SQILibInterface* (*)();
