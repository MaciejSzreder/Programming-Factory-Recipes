#pragma once

#include<vector>
#include<functional>

namespace utils
{

template<class C,class F>
auto map(C collection, F function)
{
	std::vector<decltype(std::invoke(function,*std::begin(collection)))> result;

	for(auto &element: collection){
		result.push_back(std::invoke(function,element));
	}
	return result;
}

}