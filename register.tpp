#include"register.hpp"

template<class T>
int Register<T>::size()
{
	return definitions.size();
}

template<class T>
auto Register<T>::get(int id) -> Definition
{
	return definitions[id];
}

template<typename T>
template<typename V>
std::optional<typename Register<T>::Definition> Register<T>::find(const V& value, V Definition::* field)
{
	auto definition = std::ranges::find(definitions, value, field);
	if(definition == definitions.end()){
		return {};
	}
	return *definition;
}

template<class T>
auto Register<T>::define(Definition definition) -> Definition
{
	definition.id = definitions.size();
	definitions.push_back(definition);
	return definition;
}

template<class T>
template<class D>
auto Register<T>::define(D definition) -> Definition
{
	return define(Definition(definition));
}

template<class T>
template<class D>
auto Register<T>::define() -> Definition
{
	return define(D());
}

template<typename T>
template<class D>
Register<T>::Defined<D>::Defined(): Definition(define<D>()){}