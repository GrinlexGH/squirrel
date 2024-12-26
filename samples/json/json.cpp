#include <kalibri.hpp>
#include <iostream>
#include <inttypes.h>
#include <sqstdio.h>
#include "simdjson.h"
using namespace simdjson;

class JSONDoc
{
public:
	JSONDoc() {};
	JSONDoc(const JSONDoc& jdoc) {};
	JSONDoc(std::string str) { Open(str); };
	void Open(std::string str) 
	{
		if (f != NULL)
			Close();

		f = fopen(str.c_str(), "r+");	
		if (f == NULL) {
			sq_geterrorfunc(kb::DefaultVM::Get())(kb::DefaultVM::Get(), "Could't open %s file.\n", str.c_str());
			return;
		}

		char ch;
		std::string buff2;
		while ((ch = fgetc(f)) != EOF) {
			buff2 += ch;
		}

		//json = padded_string(buff);
		//docum = parser.iterate(json);

		Parse(buff2);
		Close();
	}

	kb::Object Find(std::string str)
	{
		JSONDoc* jsondoc = new JSONDoc();
		auto a = docum[str];
		auto b = a.raw_json();
		auto c = b.value();
		auto d = c.data();
		jsondoc->Parse(std::string(c));	
		return kb::Object(jsondoc, kb::DefaultVM().Get(), true);
	}
	

	~JSONDoc()
	{
		Close();
	}


	void Close()
	{
		if(f !=  NULL)
			fclose(f);
	}



	//additional funcs
	void Parse(std::string str)
	{
		buff = str;
		json = padded_string(buff);
		docum = parser.iterate(json);
	}

	int result = SQ_OK;
	std::string buff;
private:
	FILE* f = NULL;	

	ondemand::parser parser;

	padded_string json;
	ondemand::document docum;
};

SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api) {
	kb::DefaultVM::Set(vm);

	kb::Class<JSONDoc> jsondoc(vm, "json_doc");
	jsondoc.Ctor();
	jsondoc.Ctor<std::string>();
	jsondoc.Func("open", &JSONDoc::Open);
	jsondoc.Func("close", &JSONDoc::Close);
	jsondoc.Func("find", &JSONDoc::Find);
	jsondoc.Var("buff", &JSONDoc::buff);

	kb::RootTable().Bind("json_doc", jsondoc);

	return SQ_OK;
}

void sqmodule_destruct(HSQUIRRELVM vm) {

}