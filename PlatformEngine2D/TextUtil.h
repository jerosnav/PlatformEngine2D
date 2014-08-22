#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>


class TextUtil
{
public:

	// iosBase: std::dec, etc
	template <class T>
	static std::string to_string(T t, std::ios_base & (*f)(std::ios_base&))
	{
	  std::ostringstream oss;
	  oss << f << t;
	  return oss.str();
	}

	static std::string GetPath( const std::string& sFilePath )
	{
		std::string retStr;
		retStr = sFilePath;
		std::replace(retStr.begin(), retStr.end(), '/', '\\');

		if( sFilePath.find('\\') != std::string::npos )
		{
			retStr = sFilePath.substr(0, sFilePath.rfind('\\')+1);
		}
		else if( sFilePath.find('/') != std::string::npos )
		{
			retStr = sFilePath.substr(0, sFilePath.rfind('/')+1);
		}
		else
		{
			retStr = ".\\";
		}

		return retStr;
	}

	// sPath = ".\\foo\\"
	// sRelativePath = ".\foo.bar"
	// return: ".\\foo\\foo.bar"
	static std::string CombinePath( const std::string& sPath, const std::string& sRelativePath )
	{
		std::string retStr = sRelativePath;

		int i = 0;
		char c = retStr[0];
		while( (i < retStr.size()) && ( (c == '\\') || (c == '/') ) )
		{
			++i;
			c = retStr[i];
		}
		retStr = retStr.substr(i);

		retStr = GetPath( sPath ) + retStr;

		std::replace(retStr.begin(), retStr.end(), '/', '\\');

		return retStr;
	}
};