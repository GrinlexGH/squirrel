#include <kalibri.hpp>
#include <iostream>
#include <inttypes.h>
#include <sqstdio.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

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
		
		data = json::parse(f);		
	}

	kb::Object Find(std::string str) noexcept
	{
		JSONDoc* jsondoc = new JSONDoc();

		//this try catch doesnt work, so lets hope you will spell it correctly
		jsondoc->data = data[str];
		return kb::Object(jsondoc, kb::DefaultVM().Get(), true);
	}
	
	// Returns current json key's value as string.
	// If its not a string, will return empty string.
	std::string GetString()
	{
		try {
			return data.get<std::string>();
		}
		catch (const json::out_of_range& e) {
			return "";
		}
		catch (const json::parse_error& e) {
			return "";
		}
		catch (const nlohmann::detail::type_error& e)
		{
			return "";
		}
	}

	std::string GetJson()
	{
		return data.dump();
	}

	// Returns current json key's value as int64. If its not an int64, i will kill you, your family, your dog and all of your friends.
	// On fail it will be 0. Handle it as you want, i dont care.
	int64_t GetInt()
	{
		return data.get<int64_t>();
	}

	~JSONDoc()
	{
		Close();
		data.clear();
	}


	void Close()
	{
		if(f !=  NULL)
			fclose(f);
	}



	//additional funcs
	void Parse(std::string str)
	{
		data = json::parse(str);
	}

private:
	FILE* f = NULL;	
	json data;
};

SQRESULT sqmodule_load(HSQUIRRELVM vm, HSQAPI api) {
	kb::DefaultVM::Set(vm);

	kb::Class<JSONDoc> jsondoc(vm, "json_doc");
	jsondoc.Ctor();
	jsondoc.Ctor<std::string>();
	jsondoc.Func("open", &JSONDoc::Open);
	jsondoc.Func("parse", &JSONDoc::Parse);
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