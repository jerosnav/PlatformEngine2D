#pragma once

#include <irrlicht.h>

class AlgorithmUtil
{

public:

	typedef bool (*TCompFP)(void *, void *);

	template <class T>
	static irr::core::list<T> SortList(irr::core::list<T>& _list, TCompFP _compFunc)
	{
		if( _list.size() == 0 )
		{
			return _list;
		}

		/* build up the sorted array from the empty list */
		irr::core::list<T> ret_srtList;

		irr::core::list<T>::Iterator iter( _list.getLast() );
		ret_srtList.push_back( *iter);
		_list.erase( iter );

		/* take items off the input list one by one until empty */
		bool isGetFirst = true; // improve performance avoiding while iters
		while ( _list.size() > 0 )
		{
			iter = isGetFirst? _list.begin() : _list.getLast();
			T element = *iter;
			_list.erase( iter );
			irr::core::list<T>::Iterator iter2 = ret_srtList.begin();
			irr::core::list<T>::Iterator iter2_end = ret_srtList.end();
			while( (iter2 != iter2_end) && !_compFunc( element, *iter2 ) ){ ++iter2; }
			if(iter2 != iter2_end)
			{
				ret_srtList.insert_before( iter2, element);
			}
			else
			{
				ret_srtList.push_back(element);
				isGetFirst = !isGetFirst;
			}
		}
		return ret_srtList;
	}
};