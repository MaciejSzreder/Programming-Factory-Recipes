#pragma once

#include<vector>
#include<optional>

template<class T>
struct Register
{
	struct Definition;

	inline static std::vector<Definition> definitions;
	
	static int size();
	static Definition get(int id);

	template<typename V>
	static std::optional<Definition> find(const V& value, V Definition::* field);

	static Definition define(Definition definition);
	template<class D>
	static Definition define(D definition);
	template<class D>
	static Definition define();
	template<class D>
	struct Defined: Definition
	{
		Defined();
	};
};

#include"register.tpp"