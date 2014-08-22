#pragma once

#include <irrlicht.h>

#include <string>
#include <map>

class Properties
{

private:

	std::map< std::string, std::string > m_dicTileProperties;
	
	std::map< std::string, std::string >::const_iterator m_iter;

public:

	Properties(void) :
		m_dicTileProperties(),
		m_iter( m_dicTileProperties.begin() )
	{}

	void AddProperty( const std::string& sPropertyName_, const std::string& value_);
	void SetProperty( const std::string& sPropertyName_, const std::string& value_);
	
	std::string GetProperty( const std::string& sPropertyName_);
	std::string GetPropertyCount() { m_dicTileProperties.size(); }

	// Call before GetNextProperty to start from first property
	std::string GetFirstProperty() { m_iter = m_dicTileProperties.begin(); return ( m_iter != m_dicTileProperties.end() )? m_iter->first : ""; }

	// Call after GetFirstProperty to get next property. If last property was taken, "" is returned
	std::string GetNextProperty() 
	{ 
		if( m_iter != m_dicTileProperties.end() )
		{
			++m_iter; 
		}
		return ( m_iter != m_dicTileProperties.end() )? m_iter->first : "";
	}
};

