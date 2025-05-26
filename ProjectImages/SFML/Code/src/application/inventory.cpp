#include "pch.h"
#include "inventory.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

Inventory::Inventory() = default;
Inventory::~Inventory() = default;

void Inventory::Load( const std::string& path )
{
	std::ifstream stream( path );
	std::string fileData( ( std::istreambuf_iterator<char>( stream ) ), std::istreambuf_iterator<char>() );

	Json::Document document;
	document.Parse( fileData.c_str() );
	assert( !document.HasParseError() );

	assert( document.IsObject() );

	// name
	assert( document.HasMember( "name" ) );
	assert( document[ "name" ].IsString() );
	m_ownerName = document[ "name" ].GetString();

	// maxSpace
	assert( document.HasMember( "maxSpace" ) );
	assert( document[ "maxSpace" ].IsInt() );
	m_maxSpace = document[ "maxSpace" ].GetInt();

	// currentMoney
	assert( document.HasMember( "currentMoney" ) );
	assert( document[ "currentMoney" ].IsInt() );
	m_currentMoney = document[ "currentMoney" ].GetInt();

	// items
	assert( document.HasMember( "items" ) );
	const Json::Value& arrayObject = document[ "items" ];
	assert( arrayObject.IsArray() );
	for( Json::SizeType i = 0; i < arrayObject.Size(); i++ )
	{
		Item newItem;

		// name
		assert( arrayObject[i].HasMember( "name" ) );
		assert( arrayObject[ i ][ "name" ].IsString() );
		newItem.m_name = arrayObject[ i ][ "name" ].GetString();

		// cost
		assert( arrayObject[ i ].HasMember( "cost" ) );
		assert( arrayObject[ i ][ "cost" ].IsInt() );
		newItem.m_cost = arrayObject[ i ][ "cost" ].GetInt();
		
		// size
		assert( arrayObject[ i ].HasMember( "size" ) );
		assert( arrayObject[ i ][ "size" ].IsInt() );
		newItem.m_size = arrayObject[ i ][ "size" ].GetInt();

		// rarity
		assert( arrayObject[ i ].HasMember( "rarity" ) );
		assert( arrayObject[ i ][ "rarity" ].IsInt() );
		int rarity = arrayObject[ i ][ "rarity" ].GetInt();
		assert( rarity < static_cast< int >( RarityLevel::MAX ) );
		newItem.m_rarity = static_cast< RarityLevel >( rarity );

		// count
		assert( arrayObject[ i ].HasMember( "count" ) );
		assert( arrayObject[ i ][ "count" ].IsInt() );
		newItem.m_count = arrayObject[ i ][ "count" ].GetInt();

		// items
		assert( arrayObject[ i ].HasMember( "attributes" ) );
		const Json::Value& internalArrayObject = arrayObject[ i ][ "attributes" ];
		assert( internalArrayObject.IsArray() );
		for( Json::SizeType i = 0; i < internalArrayObject.Size(); i++ )
		{
			assert( internalArrayObject[ i ].IsString() );
			newItem.m_attributes.push_back( internalArrayObject[ i ].GetString() );
		}

		m_items.push_back( std::move( newItem ) );
	}
}

void Inventory::Save(const std::string& path) {
    using namespace rapidjson;

    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    // owner name
    document.AddMember("name", Value(m_ownerName.c_str(), allocator), allocator);

    // maxSpace
    document.AddMember("maxSpace", m_maxSpace, allocator);

    // currentMoney
    document.AddMember("currentMoney", m_currentMoney, allocator);

    // items array
    Value items(kArrayType);
    for (const auto& item : m_items) {
        Value itemObject(kObjectType);
        itemObject.AddMember("name", Value(item.m_name.c_str(), allocator), allocator);
        itemObject.AddMember("cost", item.m_cost, allocator);
        itemObject.AddMember("size", item.m_size, allocator);
        itemObject.AddMember("rarity", static_cast<int>(item.m_rarity), allocator);
        itemObject.AddMember("count", item.m_count, allocator);

        Value attributes(kArrayType);
        for (const auto& attribute : item.m_attributes) {
            attributes.PushBack(Value(attribute.c_str(), allocator), allocator);
        }
        itemObject.AddMember("attributes", attributes, allocator);

        items.PushBack(itemObject, allocator);
    }
    document.AddMember("items", items, allocator);

    // JSON document to string
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    document.Accept(writer);

    // string to file
    std::ofstream stream(path);
    if (!stream.is_open()) {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return;
    }
    stream << buffer.GetString();
    stream.close();
}

