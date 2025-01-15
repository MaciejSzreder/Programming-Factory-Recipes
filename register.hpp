#pragma once

#include<vector>

template<class T>
struct Register
{
	struct Definition;

	inline static std::vector<Definition> definitions;
	
	static int size()
	{
		return definitions.size();
	}

	static get(int id){
		return definitions[id];
	}

	static Definition define(Definition definition)
	{
		definition.id = definitions.size();
		definitions.push_back(definition);
		return definitions;
	}

	template<typename V>
	static std::optional<Definition> find(const V& value, V Definition::* field)
	{
		auto definition = std::ranges::find(definitions, value, field);
		if(definition == definitions.end()){
			return {};
		}
		return *definition;
	}

	template<class D>
	Definition define(D definition)
	{
		return add(Definition(definition));
	}

	template<class D>
	Definition define()
	{
		return add(D());
	}

	template<class D>
	struct Defined
	{
		Defined()
		{
			Register<T>::Definition definition = D();
			definition.id = definitions.size();
			definitions.push_back(definition);
		}
	};
};