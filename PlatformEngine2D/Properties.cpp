#include "Properties.h"

// TODO: deprecated
void Properties::AddProperty( const std::string& sPropertyName_, const std::string& value_)
{
	SetProperty( sPropertyName_, value_);
}

void Properties::SetProperty( const std::string& sPropertyName_, const std::string& value_)
{
	m_dicTileProperties[ sPropertyName_ ] = value_;
}

std::string Properties::GetProperty( const std::string& sPropertyName_)
{
	return m_dicTileProperties.find(sPropertyName_) != m_dicTileProperties.end()? m_dicTileProperties[ sPropertyName_ ] : "";
}