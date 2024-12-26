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
	}

	kb::Object Find(std::string str) noexcept
	{
		JSONDoc* jsondoc = new JSONDoc();

		//this try catch doesnt work, so lets hope you will spell it correctly
		try
		{
			auto a = docum[str];
			auto b = a.raw_json();
			auto c = b.value();
			auto d = c.data();
			jsondoc->Parse(std::string(d));
		}
		catch (error_code e)
		{
			sq_geterrorfunc(kb::DefaultVM::Get())(kb::DefaultVM::Get(), "Got %d while trying to get %s.", e, str);
			
			//sq_close(kb::DefaultVM::Get());
			
		}
		return kb::Object(jsondoc, kb::DefaultVM().Get(), true);
	}
	
	// Returns current json key's value as string.
	// If its not a string, will return empty string.
	std::string GetString()
	{
		if (buff[0] == '"')
		{
			//char* buff2_a = new char[buff.size()];
			std::string buff2_a;
			buff2_a.resize(buff.size());
			buff.copy(buff2_a.data(), buff.size());
			//std::string buff2_b(buff2_a, buff.size());
			buff2_a.erase(remove(buff2_a.begin(), buff2_a.end(), '\"'), buff2_a.end());
			buff2_a.resize(docum.raw_json().value().length() - 2); //assuming that there is 
			return (buff2_a);
		}
		return "";
	}

	std::string GetJson()
	{
		std::string buff2_a;
		buff2_a.resize(buff.size());
		buff.copy(buff2_a.data(), buff.size());
		buff2_a.resize(docum.raw_json().value().length());
		return buff2_a;
	}

	/// EXCEPTIONS ARE PEACE OF SHIT
	///bool IsString()
	///{
	///	return docum.is_string();
	///}
	///
	///bool IsInt()
	///{
	///	return docum.is_integer();
	///}

	// Returns current json key's value as int64. If its not an int64, i will kill you, your family, your dog and all of your friends.
	// On fail it will be 0. Handle it as you want, i dont care.
	int64_t GetInt()
	{
		return atoll(buff.c_str());
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

private:
	FILE* f = NULL;	

	std::string buff;
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
	jsondoc.Func("get_string", &JSONDoc::GetString);
	jsondoc.Func("get_int", &JSONDoc::GetInt);
	jsondoc.Func("get_json", &JSONDoc::GetJson);

	//jsondoc.Func("is_int", &JSONDoc::IsInt);
	//jsondoc.Func("is_string", &JSONDoc::IsString);

	kb::RootTable().Bind("json_doc", jsondoc);

	return SQ_OK;
}

void sqmodule_destruct(HSQUIRRELVM vm) {

}